# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-external)

### vclib-external target
set(VCLIB_EXTERNAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (TARGET vclib-3rd-embree)
    file(GLOB_RECURSE HEADERS_EMBREE CONFIGURE_DEPENDS
        "include/vclib/embree/*.h")
    list(APPEND HEADERS ${HEADERS_EMBREE})
endif()

if (TARGET vclib-3rd-libigl)
    file(GLOB_RECURSE HEADERS_IGL CONFIGURE_DEPENDS
        "include/vclib/igl/*.h")
    list(APPEND HEADERS ${HEADERS_IGL})

    if (NOT TARGET vclib-3rd-cgal OR NOT TARGET vclib-3rd-boost)
        # remove include/vclib/igl/booleans.h
        list(FILTER HEADERS EXCLUDE REGEX ".*igl/booleans.h$")
    endif()
endif()

if (TARGET vclib-3rd-poisson-recon)
    file(GLOB_RECURSE HEADERS_POISSON CONFIGURE_DEPENDS "include/vclib/poisson_recon/*.h")
    list(APPEND HEADERS ${HEADERS_POISSON})
endif()

if (TARGET vclib-3rd-vcg)
    file(GLOB_RECURSE HEADERS_VCG CONFIGURE_DEPENDS "include/vclib/vcg/*.h")
    list(APPEND HEADERS ${HEADERS_VCG})
endif()

add_library(${PROJECT_NAME} INTERFACE ${HEADERS})
add_library(vclib::external ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${VCLIB_EXTERNAL_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_compile_definitions(${PROJECT_NAME} INTERFACE VCLIB_EXTERNAL_MODULE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    ${VCLIB_EXTERNAL_3RDPARTY_LIBRARIES} vclib::core)

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS})

# Install
if(VCLIB_INSTALL_MODULE_EXTERNAL)
    install(TARGETS ${PROJECT_NAME}
        EXPORT vclibTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(DIRECTORY ${VCLIB_EXTERNAL_INCLUDE_DIR}/vclib
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()
