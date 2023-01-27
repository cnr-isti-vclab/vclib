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

if (VCLIB_BUILD_TESTS)
	set(VCLIB_DOCTEST_DIR ${CMAKE_CURRENT_LIST_DIR}/doctest-2.4.9)

	if (VCLIB_ALLOW_BUNDLED_DOCTEST AND EXISTS ${VCLIB_DOCTEST_DIR}/doctest/doctest.h)
		message(STATUS "- Doctest - using bundled source")
	else()
		message(
			FATAL_ERROR
			"Doctest is required to build tests - VCLIB_ALLOW_BUNDLED_DOCTEST must be enabled and found.")
	endif()

	set(DOCTEST_INCLUDE_DIRS ${VCLIB_DOCTEST_DIR})

	add_library(vclib-external-doctest SHARED src/doctest_src.cpp)

	target_include_directories(vclib-external-doctest PUBLIC ${DOCTEST_INCLUDE_DIRS})

	list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-doctest)
endif()
