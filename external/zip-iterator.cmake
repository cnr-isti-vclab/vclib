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

set(VCLIB_ZIP_ITERATOR_DIR ${CMAKE_CURRENT_LIST_DIR}/zip-iterator-master)

if (VCLIB_ALLOW_BUNDLED_ZIP_ITERATOR AND
		EXISTS ${VCLIB_ZIP_ITERATOR_DIR}/zip_tuple.hpp)
	message(STATUS "- Zip-Iterator - using bundled source")
else()
	message(
		FATAL_ERROR
		"Zip-Iterator is required -
		VCLIB_ALLOW_BUNDLED_ZIP_ITERATOR must be enabled and found.")
endif()

set(ZIP_ITERATOR_INCLUDE_DIRS ${VCLIB_ZIP_ITERATOR_DIR})

add_library(vclib-external-zip-iterator INTERFACE)

target_include_directories(vclib-external-zip-iterator INTERFACE
	${ZIP_ITERATOR_INCLUDE_DIRS})

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-zip-iterator)
