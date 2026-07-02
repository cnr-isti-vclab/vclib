# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.24)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" VCLIB_VERSION)
string(STRIP "${VCLIB_VERSION}" VCLIB_VERSION)

project(vclib VERSION ${VCLIB_VERSION})

message(STATUS "VCLib version: ${PROJECT_VERSION}")

### Options

option(VCLIB_COMPILE_WARNINGS_AS_ERRORS "Compile warnings as errors" ON)

# Optional modules build options
option(
    VCLIB_BUILD_CORE_WITH_3RDPARTY_IO_LIBRARIES
    "Build the core module with 3rdparty I/O libraries"
    ON
)

option(VCLIB_BUILD_MODULE_EXTERNAL "Build the external module" OFF)
option(VCLIB_BUILD_MODULE_RENDER "Build the render module" OFF)

option(VCLIB_BUILD_MODULE_BINDINGS "Build the python bindings module" OFF)

# If true, examples and tests targets will be excluded from the build
option(
    VCLIB_EXCLUDE_EXAMPLES_AND_TESTS_TARGETS
    "Exclude examples and tests targets from the build"
    ON
)

# If true, the examples and tests of VCLib will be built just setting
# the INCLUDE_PATH of the core module (no cmake targets).
option(
    VCLIB_TESTS_AND_EXAMPLES_USE_CORE_HEADER_ONLY
    "If true, the examples and tests of VCLib-core will be built just setting
    the INCLUDE_PATH of the library (no cmake targets)."
    OFF
)

# If true, vclib will require Qt
option(
    VCLIB_REQUIRES_QT
    "If true, vclib will require Qt
    (if core with io or render modules enabled)"
    OFF
)

# Render specific options
if(VCLIB_BUILD_MODULE_RENDER)
    set(VCLIB_RENDER_BACKEND "bgfx" CACHE STRING "Select the rendering engine")
    set_property(CACHE VCLIB_RENDER_BACKEND PROPERTY STRINGS bgfx opengl2 none)
endif()

# Install modules options
option(VCLIB_INSTALL_MODULE_CORE "Install the core module" ON)

if(VCLIB_BUILD_MODULE_EXTERNAL)
    option(VCLIB_INSTALL_MODULE_EXTERNAL "Install the external module" ON)
endif()

if(VCLIB_BUILD_MODULE_RENDER)
    option(VCLIB_INSTALL_MODULE_RENDER "Install the render module" ON)
endif()

if(VCLIB_BUILD_MODULE_BINDINGS)
    option(
        VCLIB_INSTALL_MODULE_BINDINGS
        "Install the python bindings module"
        ON
    )
endif()

### Common settings
include(cmake/vclib_common_settings.cmake)

### Examples and Tests
include(cmake/examples_and_tests.cmake)

message(STATUS "VCLib: 3rdparty libraries")
add_subdirectory(3rdparty)

add_library(${PROJECT_NAME} INTERFACE)
add_library(vclib::vclib ALIAS ${PROJECT_NAME})

add_subdirectory(vclib/core ${VCLIB_CORE_EXCLUDE_FROM_ALL_OPTION})
if(VCLIB_INSTALL_MODULE_CORE)
    target_link_libraries(vclib INTERFACE vclib-core)
else()
    target_link_libraries(vclib INTERFACE $<BUILD_INTERFACE:vclib-core>)
endif()

if(VCLIB_BUILD_MODULE_EXTERNAL)
    add_subdirectory(vclib/external ${VCLIB_EXTERNAL_EXCLUDE_FROM_ALL_OPTION})
    if(VCLIB_INSTALL_MODULE_EXTERNAL)
        target_link_libraries(vclib INTERFACE vclib-external)
    else()
        target_link_libraries(vclib INTERFACE $<BUILD_INTERFACE:vclib-external>)
    endif()
endif()

if(VCLIB_BUILD_MODULE_RENDER)
    add_subdirectory(vclib/render ${VCLIB_RENDER_EXCLUDE_FROM_ALL_OPTION})
    if(VCLIB_INSTALL_MODULE_RENDER)
        target_link_libraries(vclib INTERFACE vclib-render)
    else()
        target_link_libraries(vclib INTERFACE $<BUILD_INTERFACE:vclib-render>)
    endif()
endif()

if(VCLIB_BUILD_MODULE_BINDINGS)
    add_subdirectory(vclib/bindings ${VCLIB_BINDINGS_EXCLUDE_FROM_ALL_OPTION})
endif()

set(VCLIB_EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(VCLIB_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

if(VCLIB_TESTS_AND_EXAMPLES_USE_CORE_HEADER_ONLY)
    message(
        STATUS
        "VCLib tests and examples will use core module in header only mode"
    )
    set(HEADER_ONLY_OPTION HEADER_ONLY)
    set(VCLIB_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vclib/core/include)
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES VCLIB_EXAMPLES_DIR ${VCLIB_EXAMPLES_DIR}
)

add_subdirectory(${VCLIB_EXAMPLES_DIR})

include(CTest)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES VCLIB_TESTS_DIR ${VCLIB_TESTS_DIR}
)

add_subdirectory(${VCLIB_TESTS_DIR})

# Install
if(
    VCLIB_INSTALL_MODULE_CORE
    OR VCLIB_INSTALL_MODULE_EXTERNAL
    OR VCLIB_INSTALL_MODULE_RENDER
)
    install(TARGETS ${PROJECT_NAME} EXPORT vclibTargets)

    install(
        EXPORT vclibTargets
        FILE vclibTargets.cmake
        NAMESPACE vclib::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vclib
    )

    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/vclibConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/vclibConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vclib
    )

    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/vclibConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/vclibConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/vclibConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vclib
    )
endif()

install(
    FILES ${CMAKE_CURRENT_LIST_DIR}/LICENSE
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/vclib
)
