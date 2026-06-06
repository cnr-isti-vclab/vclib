#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2026                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the Mozilla Public License Version 2.0 as published *
#* by the Mozilla Foundation; either version 2 of the License, or            *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* Mozilla Public License Version 2.0                                        *
#* (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
#****************************************************************************/

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

if (TARGET vclib-3rd-vcg)
    file(GLOB_RECURSE HEADERS_VCG CONFIGURE_DEPENDS "include/vclib/vcg/*.h")
    list(APPEND HEADERS ${HEADERS_VCG})
endif()

add_library(${PROJECT_NAME} INTERFACE ${HEADERS})
add_library(vclib::external ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ${VCLIB_EXTERNAL_INCLUDE_DIR})
target_compile_definitions(${PROJECT_NAME} INTERFACE VCLIB_EXTERNAL_MODULE)

target_link_libraries(${PROJECT_NAME} INTERFACE
    ${VCLIB_EXTERNAL_3RDPARTY_LIBRARIES} vclib::core)

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS})

# Install
install(DIRECTORY ${VCLIB_EXTERNAL_INCLUDE_DIR}/vclib
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
