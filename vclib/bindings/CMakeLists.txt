#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2024                                                    *
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

cmake_minimum_required(VERSION 3.24)
project(vclib-bindings)

set(MODULE_NAME vclib_bindings)

# 3rdparty libraries
message(STATUS "VCLib-Bindings: 3rd party libraries")
add_subdirectory(3rdparty)

set(VCLIB_BINDINGS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(VCLIB_MODULE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/vclib)

set(PYTHON_MODULES core)

if (VCLIB_BUILD_MODULE_EXTERNAL)
#    list(APPEND PYTHON_MODULES external)
endif()

if (VCLIB_BUILD_MODULE_PROCESSING)
    list(APPEND PYTHON_MODULES processing)
endif()

if (VCLIB_BUILD_MODULE_RENDER)
#    list(APPEND PYTHON_MODULES render)
endif()

# create __init__.py inside VCLIB_MODULE_BINARY_DIR
file(WRITE "${VCLIB_MODULE_BINARY_DIR}/__init__.py" "")

# for each module
foreach(MODULE ${PYTHON_MODULES})
    file(GLOB_RECURSE MODULE_HEADERS CONFIGURE_DEPENDS "include/vclib/bindings/${MODULE}/*.h")
    file(GLOB_RECURSE MODULE_SOURCES CONFIGURE_DEPENDS "src/vclib/bindings/${MODULE}/*.cpp")
    list(APPEND MODULE_SOURCES "src/vclib/bindings/${MODULE}.cpp")

    pybind11_add_module(${MODULE} ${MODULE_HEADERS} ${MODULE_SOURCES})

    target_include_directories(${MODULE} PRIVATE ${VCLIB_BINDINGS_INCLUDE_DIR})
    target_link_libraries(${MODULE} PRIVATE vclib::core vclib::${MODULE})

    # put the generated binary in a dir called vclib
    set_target_properties(${MODULE} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${VCLIB_MODULE_BINARY_DIR})

    # in the ${VCLIB_MODULE_BINARY_DIR}/__init__.py, append a line with:
    # from .${MODULE} import *
    file(APPEND "${VCLIB_MODULE_BINARY_DIR}/__init__.py" "from .${MODULE} import *\n")

    # Install
    install(TARGETS ${MODULE}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/python${VCLIB_PYTHON_VERSION}/site-packages/vclib
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

    # RPath for qt
    if (TARGET vclib-3rd-qt)
        if (APPLE)
            set_target_properties(${MODULE} PROPERTIES
                INSTALL_RPATH "@loader_path/../PyQt6/Qt6/lib")
        elseif(LINUX)
            set_target_properties(${MODULE} PROPERTIES
                INSTALL_RPATH "$ORIGIN/../PyQt6/Qt6/lib")
        endif()
    endif()
endforeach()

# Install
install(FILES "${VCLIB_MODULE_BINARY_DIR}/__init__.py"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/python${VCLIB_PYTHON_VERSION}/site-packages/vclib)


