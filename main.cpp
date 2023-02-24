#include <stdio.h>
#include <atomic>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>
#include <VwGigE.API.h>
#include "toojpeg/toojpeg.h"
#include "ctpl_stl.h"
#include "error.hpp"


using namespace VWSDK;
using namespace std::chrono_literals;


#define IMAGE_WIDTH  640
#define IMAGE_HEIGHT 480
#define FRAME_COUNT 800
#define WRITE_THREADS 8


// thread pool for writing files
ctpl::thread_pool write_pool(WRITE_THREADS);
// callback for processing frames
void frame_callback(OBJECT_INFO *object, IMAGE_INFO *image);
// count of the number of processed frames
atomic_long frame_count = 0;
// file prefix (used for path information)
const char *prefix;


// macro that defines how to release GigE resources
// it is modified over the course of the program as resources are loaded
#define RELEASE_RESOURCES

// macro to check for errors
// it uses RELEASE_RESOURCES to free GigE resources in case of errors
#define CHECK_ERROR(result, activity) \
do { \
	if (result != RESULT_SUCCESS) { \
		fprintf(stderr, "[FATAL] Failed to %s: %s (%d)\n", activity, result_to_string(result), result); \
		RELEASE_RESOURCES; \
		return 1; \
	} \
} while (0)


// --===== main =====--

int main(int argc, char **argv) {
	// get the file prefix (used to prepend "tempXXX\" to the filename)
	if (argc < 2) {
		fprintf(stderr, "[FATAL] No file prefix specified!\n");
		return 1;
	}
	prefix = argv[1];

	// create GigE driver
	VWGIGE_HANDLE driver = NULL;
	RESULT result = OpenVwGigE(&driver);
	CHECK_ERROR(result, "create GigE driver");
	#undef RELEASE_RESOURCES
	#define RELEASE_RESOURCES CloseVwGigE(driver);

	// count available cameras
	UINT numCameras = 1;
	result = VwGetNumCameras(driver, &numCameras);
	CHECK_ERROR(result, "count available cameras");
	if (numCameras == 0) {
		fprintf(stderr, "[FATAL] No cameras were found!\n");
		RELEASE_RESOURCES;
		return 1;
	}

	// open camera
	HCAMERA camera = NULL;
	int user = 0;
	result = VwOpenCameraByIndex(
		driver,
		0, // index
		&camera,
		20, // number of buffers
		0, 0, 0, // width/height/packet size (zero for auto-detect)
		&user, // user pointer (CANNOT BE NULL OR THIS FUNCTION WILL ERROR FOR SOME REASON)
		frame_callback
	);
	CHECK_ERROR(result, "open camera");

	#undef RELEASE_RESOURCES
	#define RELEASE_RESOURCES \
		CameraClose(camera); \
		CloseVwGigE(driver);
	
	// configure camera
	result = CameraSetPixelFormat(camera, PIXEL_FORMAT_RGB8);
	CHECK_ERROR(result, "set camera pixel format");

	result = CameraSetWidth(camera, IMAGE_WIDTH);
	CHECK_ERROR(result, "set camera image width");

	result = CameraSetHeight(camera, IMAGE_HEIGHT);
	CHECK_ERROR(result, "set camera image height");

	// grab images
	result = CameraGrab(camera);
	CHECK_ERROR(result, "begin acquiring images");
	std::this_thread::sleep_for(5s);
	if (frame_count < 50) {
		// some sort of communication error has occurred
		fprintf(stderr, "!!! FAILED TO COMMUNICATE WITH CAMERA !!!\n");
		write_pool.stop(false);
		RELEASE_RESOURCES; // release camera & gigE driver
		return 1;
	}
	while (frame_count < FRAME_COUNT) {}
	RELEASE_RESOURCES; // release camera & gigE driver

	// wait for writes to complete
	printf("Acquiring finished, waiting for filesystem...\n");
	write_pool.stop(true);

	printf("Done!\n");
	return 0;
}


// --===== etc =====--

// image data struct
struct Image {
	unsigned int index;
	unsigned int w, h;
	unsigned char *data;

	void ConvertInfo(IMAGE_INFO *info) {
		w = info->width;
		h = info->height;
		data = new unsigned char[w * h * 3];
		memcpy(data, info->pImage, w * h * 3);
	}
};


// helper function for writing toojpeg output to disk
static void write(unsigned char byte, void *userdata) { fputc(byte, (FILE*) userdata); }

// write Image to disk, called from thread pool
void write_image(int unused, const char *prefix, Image img) {
	// compute filename as [prefix] + [number].jpg
	char filename[32];
	snprintf(filename, 32, "%s%03d.jpg", prefix, img.index);

	// open file
	FILE *writer = fopen(filename, "wb");
	if (writer != NULL) {
		// write jpeg data to disk
		TooJpeg::writeJpeg(write, writer, img.data, img.w, img.h, true, 90, false, "hello");
		fclose(writer);
	}
	else {
		fprintf(stderr, "[ERROR] Could not open \"%s\"\n", filename);
	}

	free(img.data);
}


// per-frame callback
void frame_callback(OBJECT_INFO *unused, IMAGE_INFO *image) {
	// check if we have alread collected the frames we need
	if (frame_count >= FRAME_COUNT) { return; }
	
	// copy the image data into an Image struct
	Image img;
	img.index = frame_count;
	img.ConvertInfo(image);

	// push to the thread pool
	write_pool.push(write_image, prefix, img);

	// increment frame count
	frame_count += 1;
}
