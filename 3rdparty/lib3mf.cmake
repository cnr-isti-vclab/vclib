# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(LIB3MF_VERSION 2.5.0) 

if(VCLIB_ALLOW_SYSTEM_LIB3MF)
    find_package(lib3mf QUIET)
endif()

if(VCLIB_ALLOW_SYSTEM_LIB3MF AND (TARGET lib3mf::lib3mf OR TARGET lib3mf))
    message(STATUS "- lib3mf - using system-provided library")
    set(VCLIB_USED_SYSTEM_LIB3MF ON CACHE INTERNAL "")
    
    add_library(vclib-3rd-lib3mf INTERFACE)

    if(TARGET lib3mf::lib3mf)
        # We clear the interface options to prevent them from leaking into vclib targets
        set_target_properties(lib3mf::lib3mf PROPERTIES INTERFACE_COMPILE_OPTIONS "")
        target_link_libraries(vclib-3rd-lib3mf INTERFACE lib3mf::lib3mf)
    else()
        set_target_properties(lib3mf PROPERTIES INTERFACE_COMPILE_OPTIONS "")
        target_link_libraries(vclib-3rd-lib3mf INTERFACE lib3mf)
    endif()

    target_compile_definitions(vclib-3rd-lib3mf INTERFACE VCLIB_WITH_LIB3MF)
    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-lib3mf)

elseif(VCLIB_ALLOW_DOWNLOAD_LIB3MF)
    message(STATUS "- lib3mf - using downloaded source")

    set(LIB3MF_TESTS OFF CACHE BOOL "" FORCE)
    # We force static build to avoid polluting system with lib3mf.so and conflicting with OS packages
    set(LIB3MF_BUILD_SHARED OFF CACHE BOOL "" FORCE)

    set(LIB3MF_EXCLUDE_FROM_ALL_OPTION "")
    if(NOT VCLIB_ALLOW_INSTALL_LIB3MF)
        set(LIB3MF_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
    endif()

    FetchContent_Declare(
        lib3mf
        GIT_REPOSITORY https://github.com/3MFConsortium/lib3mf
        GIT_TAG v${LIB3MF_VERSION}
        ${LIB3MF_EXCLUDE_FROM_ALL_OPTION}
    )
    # Workaround for a bug in lib3mf CMakeLists.txt where it looks for lib3mf.pc
    # in CMAKE_BINARY_DIR during installation instead of CMAKE_CURRENT_BINARY_DIR.
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/lib3mf.pc")
        file(TOUCH "${CMAKE_BINARY_DIR}/lib3mf.pc")
    endif()

    vclib_begin_3rdparty_install_scope()

    FetchContent_MakeAvailable(lib3mf)

    vclib_end_3rdparty_install_scope()

    if(TARGET lib3mf)
        # lib3mf CMake populates PUBLIC compile options (e.g. /WX on Windows)
        # We clear the interface options to prevent them from leaking into vclib targets
        set_target_properties(lib3mf PROPERTIES INTERFACE_COMPILE_OPTIONS "")
    endif()

    add_library(vclib-3rd-lib3mf INTERFACE)
    target_link_libraries(vclib-3rd-lib3mf INTERFACE lib3mf)

    target_compile_definitions(vclib-3rd-lib3mf INTERFACE VCLIB_WITH_LIB3MF)
    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-lib3mf)

else()
    message(STATUS "- lib3mf - not found, skipping")
endif()
