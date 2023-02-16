#include <stdlib.h>
#include <stdarg.h>
#include <VwGigE.API.h>
#include "error.hpp"


using namespace VWSDK;


const char * result_to_string(RESULT result)
{
	switch(result) {
	case RESULT_SUCCESS:
		return "Success.";
	
	case RESULT_ERROR:
		return "Unspecified runtime error. ";
	
	case RESULT_ERROR_OPENED_ALREADY:
		return "The module handle to open already opened. ";
	
	case RESULT_ERROR_INVALID_HANDLE:
		return "Given handle does not support the operation; e.g. function call on wrong handle or NULL pointer. ";
	
	case RESULT_ERROR_TL_HANDLE:
		return "Internal error. GenTL Transport handle is invalid. ";
	
	case RESULT_ERROR_TLOPEN:
		return "Internal error. GenTL TLOpen function returned error. ";
	
	case RESULT_ERROR_IF_HANDLE:
		return "Internal error. GenTL Interface handle is invalid. ";
	
	case RESULT_ERROR_INITIALIZATION:
		return "Module was not initialized ";
	
	case RESULT_ERROR_INVALID_PARAMETER:
		return "One of the parameter given was not valid or out of range and none of the error codes about fits. ";
	
	case RESULT_ERROR_DISCOVERY:
		return "There was an error on the discovery. ";
	
	case RESULT_ERROR_NO_CAMERAS:
		return "There are no cameras. ";
	
	case RESULT_ERROR_CAMERA_NAME_DOES_NOT_EXIST:
		return "The camera name of which you are trying to open does not exist. ";
	
	case RESULT_ERROR_ABORTED_ALREADY:
		return "The Abort command was already completed. ";
	
	case RESULT_ERROR_ACCESS_DENIED:
		return "Access to the camera is denied. ";
	
	case RESULT_ERROR_RESOURCE_IN_USE:
		return "The handle or resource has already been used. ";
	
	case RESULT_ERROR_CANNOT_FIND_INTERFACE:
		return "Failed to find the interface. ";
	
	case RESULT_ERROR_XML_UNKNOWN_ARGUMENT:
		return "The argument value is out of range or cannot be recognized. ";
	
	case RESULT_ERROR_XML_NODE_ACCESS_FAILED:
		return "The Node map is abnormal or AccessMode for the node is NI/NA. ";
	
	case RESULT_ERROR_XML_NOT_EXIST_NODE:
		return "The node does not exist. ";
	
	case RESULT_ERROR_XML_ENTERED_NODE_DOESNT_HAVE_ANY_VALUE:
		return "The node doesn't have any value like COMMAND, CATEGORY. ";
	
	case RESULT_ERROR_XML_UNSUPPORTED_COMMAND:
		return "Unknown command. ";
	
	case RESULT_ERROR_INVALID_WIDTH:
		return "Width is not multiples of 4. ";
	
	case RESULT_ERROR_INVALID_ADDRESS:
		return "Unknown address. ";
	
	case RESULT_ERROR_VWINTERFACE_NO_NIC:
		return "There is no interface card. ";
	
	case RESULT_ERROR_VWINTERFACE_GETINTERFACENAME:
		return "Failed to retrieve a interface name. ";
	
	case RESULT_ERROR_VWINTERFACE_OPENINTERFACE:
		return "OpenInterface function returned an error. ";
	
	case RESULT_ERROR_VWINTERFACE_CLOSEINTERFACE:
		return "CloseInterface function returned an error. ";
	
	case RESULT_ERROR_VWINTERFACE_GETNUMDEVICES:
		return "GetNumDevices function returned an error. ";
	
	case RESULT_ERROR_VWINTERFACE_CANNOT_FIND_DEVICE:
		return "Failed to find a device. ";
	
	case RESULT_ERROR_VWCAMERA_INTERFACE_HANDLE:
		return "The interface handle is invalid. ";
	
	case RESULT_ERROR_VWCAMERA_CAMERAINDEX_OVER:
		return "The camera index is over the maximum number of cameras. ";
	
	case RESULT_ERROR_VWCAMERA_GETXML:
		return "There is a problem with the XML that was retrieved from the camera. ";
	
	case RESULT_ERROR_VWCAMERA_IMAGE_NOT4DIVIDE:
		return "Width must divide by 4. ";
	
	case RESULT_ERROR_VWCAMERA_IMAGE_NOT2DIVIDE:
		return "Width must divide by 2. ";
	
	case RESULT_ERROR_VWCAMERA_READ_ONLY:
		return "The node is read only. ";
	
	case RESULT_ERROR_VWCAMERA_EVENTCONTROL_DOESNOT_INIT:
		return "Event control function did not initialize. ";
	
	case RESULT_ERROR_VWCAMERA_GRAB_TIMEOUT:
		return "Time-out is occurred in grab routine. ";
	
	case RESULT_ERROR_VWCAMERA_CALLBACK_NOT_NULL:
		return "Callback function pointer is not null. ";
	
	case RESULT_ERROR_DEVCREATEDATASTREAM:
		return "Internal error. GenTL DevCreateDataStream function returned error. ";
	
	case RESULT_ERROR_DATASTREAM_MTU:
		return "Internal error. The MTU of the NIC is too small to get a image. ";
	
	case RESULT_ERROR_TLGETNUMINTERFACES:
		return "Internal error. GenTL TLGetNumInterfaces function returned error. ";
	
	case RESULT_ERROR_TLOPENINTERFACE:
		return "Internal error. GenTL TLOpenInterface function returns error. ";
	
	case RESULT_ERROR_TLCLOSEINTERFACE:
		return "Internal error. GenTL TLCloseInterface function returns error. ";
	
	case RESULT_ERROR_TLGETINTERFACENAME:
		return "Internal error. GenTL TLGetInterfaceName function returned error. ";
	
	case RESULT_ERROR_TLGETNUMDEVICES:
		return "Internal error. GenTL TLGetNumDevices function returns error. ";
	
	case RESULT_ERROR_TLGETDEVICENAME:
		return "Internal error. GenTL TLGetDeviceName function returns error. ";
	
	case RESULT_ERROR_TLOPENDEVICE:
		return "Internal error. GenTL TLOpenDevice function returns error. ";
	
	case RESULT_ERROR_INSUFFICIENT_RESOURCES:
		return "Insufficient system resources. Unable to allocate memory as many as defined the number of buffers. ";
	
	case RESULT_ERROR_MEMORY_ALLOCATION:
		return "Unable to allocate memory. ";
	
	case RESULT_ERROR_FILE_STREAM_OPEN_FAILURE:
		return "Failed to open File stream. ";
	
	case RESULT_ERROR_FILE_STREAM_READ_FAILURE:
		return "Failed to read File stream. ";
	
	case RESULT_ERROR_FILE_STREAM_WRITE_FAILURE:
		return "Failed to write File stream. ";
	
	case RESULT_ERROR_FILE_STREAM_CLOSE_FAILURE:
		return "Failed to close File stream. ";
	
	case RESULT_ERROR_FILE_STREAM_NOT_CORRECT_FILE_LENGTH:
		return "File length is incorrect. ";
	
	case RESULT_ERROR_EXCEPTION:
		return "An exception is occurred. ";
	
	default:
		return "(unknown error)";
	}
}
