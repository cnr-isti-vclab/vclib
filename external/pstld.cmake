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

set(VCLIB_PSTLD_DIR ${CMAKE_CURRENT_LIST_DIR}/pstld-master)

if (VCLIB_ALLOW_BUNDLED_PSTLD AND EXISTS ${VCLIB_PSTLD_DIR}/pstld/pstld.h)
	message(STATUS "- pstld - using bundled source")
else()
	message(
		FATAL_ERROR
		"pstld is required on Apple Clang- VCLIB_ALLOW_BUNDLED_PSTLD must be enabled and found.")
endif()

set(PSTLD_INCLUDE_DIRS ${VCLIB_PSTLD_DIR})

add_library(vclib-external-pstld SHARED ${VCLIB_PSTLD_DIR}/pstld/pstld.cpp)

target_include_directories(vclib-external-pstld PUBLIC ${PSTLD_INCLUDE_DIRS})
target_compile_definitions(vclib-external-pstld PUBLIC PSTLD_HACK_INTO_STD)

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-pstld)
