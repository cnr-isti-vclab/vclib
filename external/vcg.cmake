#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/


if (VCLIB_ALLOW_SYSTEM_VCG)
	if (DEFINED VCG_INCLUDE_DIRS OR DEFINED VCG_DIR)
		if (DEFINED VCG_DIR)
			set(VCG_INCLUDE_DIRS ${VCG_DIR})
			set(VCG_INCLUDE_DIRS ${VCG_DIR} PARENT_SCOPE)
		endif()
		message(STATUS "- VCG - using system-provided library")

		add_library(vclib-external-vcg INTERFACE)
		target_include_directories(vclib-external-vcg INTERFACE
			${VCG_INCLUDE_DIRS})

		list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-vcg)
	else()
		message(STATUS "- VCG - not found, skipping")
	endif()
endif()
