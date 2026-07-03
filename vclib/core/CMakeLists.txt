# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

project(vclib-core)

### vclib-core target
set(VCLIB_CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/vclib/*.h")

add_library(${PROJECT_NAME} INTERFACE ${HEADERS})
add_library(vclib::core ALIAS ${PROJECT_NAME})
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${VCLIB_CORE_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_definitions(${PROJECT_NAME} INTERFACE VCLIB_CORE_MODULE)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES VCLIB_CORE_INCLUDE_DIR ${VCLIB_CORE_INCLUDE_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE ${VCLIB_CORE_3RDPARTY_LIBRARIES}
)

# Ensure optional system libraries (e.g., Qt) are linked against the core target.
# This relies on BUILD_INTERFACE to avoid exposing absolute paths in the export.
foreach(lib IN LISTS VCLIB_CORE_OPTIONAL_SYSTEM_LIBRARIES)
    target_link_libraries(${PROJECT_NAME} INTERFACE $<BUILD_INTERFACE:${lib}>)
endforeach()

target_sources(${PROJECT_NAME} PRIVATE ${HEADERS})

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} INTERFACE _USE_MATH_DEFINES)
    target_compile_options(${PROJECT_NAME} INTERFACE "/bigobj")
endif()

# Install
if(VCLIB_INSTALL_MODULE_CORE)
    install(
        TARGETS ${PROJECT_NAME}
        EXPORT vclibTargets
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        DIRECTORY ${VCLIB_CORE_INCLUDE_DIR}/vclib
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()
