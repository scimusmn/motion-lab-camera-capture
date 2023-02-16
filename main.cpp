#include <stdio.h>
#include <atomic>
#include <VwGigE.API.h>
#include "error.hpp"


using namespace VWSDK;


#define IMAGE_WIDTH  640
#define IMAGE_HEIGHT 480
#define FRAMES 800


#define RELEASE_RESOURCES
#define CHECK_ERROR(result, activity) \
do { \
	if (result != RESULT_SUCCESS) { \
		fprintf(stderr, "[FATAL] Failed to %s: %s (%d)\n", activity, result_to_string(result), result); \
		RELEASE_RESOURCES; \
		exit(1); \
	} \
} while (0)


atomic_int snaps = 0;
void image_callback(OBJECT_INFO *object, IMAGE_INFO *image)
{
	snaps += 1;
}


void configure_camera(VWGIGE_HANDLE driver, HCAMERA camera)
{
	RESULT result;

}



int main()
{
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

	// get info
	CAMERA_INFO_STRUCT camera_info;
	result = VwDiscoveryCameraInfo(driver, 0, &camera_info);
	CHECK_ERROR(result, "get camera info");
	printf("name: %s\nvendor: %s\nmodel: %s\nip: %s\nmac: %s\n\n",
		camera_info.name,
		camera_info.vendor,
		camera_info.model,
		camera_info.ip,
		camera_info.mac
	);

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

	// clean up
	RELEASE_RESOURCES;
	printf("Done!\n");
	return 0;
}
