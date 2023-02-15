#include <stdio.h>
#include <VwGigE.API.h>


using namespace VWSDK;


int main()
{
	VWGIGE_HANDLE driver = NULL;
	RESULT result = OpenVwGigE(&driver);
	if (result != RESULT_SUCCESS) {
		fprintf(stderr, "ERROR: failed to create GigE driver! (%d)\n", result);
		return 1;
	}

	result = VwDiscovery(driver);
	if (result != RESULT_SUCCESS) {
		fprintf(stderr, "ERROR: failed to discover cameras! (%d)\n", result);
		return 3;
	}

	UINT numCameras = 1;
	result = VwGetNumCameras(driver, &numCameras);
	if (result != RESULT_SUCCESS) {
		fprintf(stderr, "ERROR: failed to count number of available cameras! (%d)\n", result);
		return 2;
	}

	printf("found %d cameras\n", numCameras);
	
	CloseVwGigE(driver);
	return 0;
}
