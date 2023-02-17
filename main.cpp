#include <stdio.h>
#include <atomic>
#include <queue>
#include <thread>
#include <mutex>
#include <VwGigE.API.h>
#include "toojpeg/toojpeg.h"
#include "ctpl_stl.h"
#include "error.hpp"


using namespace VWSDK;


#define IMAGE_WIDTH  640
#define IMAGE_HEIGHT 480
#define FRAMES 800
#define WRITE_THREADS 8


ctpl::thread_pool write_pool(WRITE_THREADS);


#define RELEASE_RESOURCES
#define CHECK_ERROR(result, activity) \
do { \
	if (result != RESULT_SUCCESS) { \
		fprintf(stderr, "[FATAL] Failed to %s: %s (%d)\n", activity, result_to_string(result), result); \
		RELEASE_RESOURCES; \
		exit(1); \
	} \
} while (0)


static void write(unsigned char byte, void *userdata) { fputc(byte, (FILE*) userdata); }


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


void write_image(int thread, const char *prefix, Image img) {
	char filename[32];
	snprintf(filename, 32, "%s%03d.jpg", prefix, img.index);
	FILE *writer = fopen(filename, "wb");
	if (writer != NULL) {
		TooJpeg::writeJpeg(write, writer, img.data, img.w, img.h, true, 90, false, "hello");
		fclose(writer);
	}
	else {
		fprintf(stderr, "[ERROR] Could not open \"%s\"\n", filename);
	}

	free(img.data);
}


atomic_long snaps = 0;
std::queue<Image> images;
const char *prefix;

void image_callback(OBJECT_INFO *object, IMAGE_INFO *image) {
	if (snaps >= FRAMES) { return; }
	Image img;
	img.index = snaps;
	img.ConvertInfo(image);
	write_pool.push(write_image, prefix, img);
	printf("pushed %d\n", (unsigned int ) snaps);
	snaps += 1;
}


int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "[FATAL] No file prefix specified!\n");
		exit(1);
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
		exit(1);
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
		image_callback
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
	while(snaps < FRAMES) { /* wait for frames */ }
	RELEASE_RESOURCES;

	// wait for writes to complete
	printf("Acquiring finished, waiting for filesystem...\n");
	write_pool.stop(true);

	printf("Done!\n");
	return 0;
}
