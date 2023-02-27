find_path(
	VIS_INCLUDE_DIR VwGigE.API.h
	HINTS
		"CPP/Include"
		"C:/Vieworks Imaging Solution 7/SDK/CPP/Include"
)

find_library(
	VIS_GIGE_LIB 
	NAMES VwGigE.V7
	HINTS
		"CPP/LIB/x86"
		"CPP/DLL/x86"
		"C:/Vieworks Imaging Solution 7/SDK/CPP/LIB/x86"
		"C:/Vieworks Imaging Solution 7/SDK/CPP/DLL/x86"
)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VIS REQUIRED_VARS VIS_INCLUDE_DIR VIS_GIGE_LIB)
mark_as_advanced(VIS_INCLUDE_DIR VIS_GIGE_LIB)
