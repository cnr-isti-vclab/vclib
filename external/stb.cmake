#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

set(VCLIB_STB_DIR ${CMAKE_CURRENT_LIST_DIR}/stb-master)

if (VCLIB_ALLOW_BUNDLED_STB AND EXISTS ${VCLIB_STB_DIR}/stb/stb_image.h)
	message(STATUS "- STB - using bundled source")


	set(STB_INCLUDE_DIRS ${VCLIB_STB_DIR})

	add_library(vclib-external-stb SHARED src/stb_src.cpp)

	target_include_directories(vclib-external-stb PUBLIC ${STB_INCLUDE_DIRS})
	target_compile_definitions(vclib-external-stb PUBLIC VCLIB_WITH_STB)

	list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-stb)
endif()
