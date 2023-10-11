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

set(VCLIB_ZIP_VIEWS_DIR ${CMAKE_CURRENT_LIST_DIR}/zip-views-1.0)

if (VCLIB_ALLOW_BUNDLED_ZIP_VIEWS AND
        EXISTS ${VCLIB_ZIP_VIEWS_DIR}/zip_view.hpp)
    message(STATUS "- ZipViews - using bundled source")
else()
    message(
        FATAL_ERROR
        "ZipViews is required -
        VCLIB_ALLOW_BUNDLED_ZIP_VIEWS must be enabled and found.")
endif()

set(ZIP_VIEWS_INCLUDE_DIRS ${VCLIB_ZIP_VIEWS_DIR})

set(ZIP_VIEW_BUILD_TEST OFF CACHE BOOL "Build ZipViews tests")
add_subdirectory(${VCLIB_ZIP_VIEWS_DIR})

add_library(vclib-external-zip-views INTERFACE)

target_link_libraries(vclib-external-zip-views INTERFACE zip-view)
target_compile_definitions(vclib-external-zip-views INTERFACE
    ZIP_VIEW_INJECT_STD_VIEWS_NAMESPACE)

list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-zip-views)
