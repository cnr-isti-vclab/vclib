# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

include(FetchContent)

set(BGFX_VERSION 1.172.9657-622)
set(BGFX_PRECOMPILED_TAG v1.1.1)

function(_vclib_download_precompiled_bgfx OUT_DIR OUT_PLATFORM)
    set(${OUT_DIR} "" PARENT_SCOPE)
    set(${OUT_PLATFORM} "" PARENT_SCOPE)

    set(_PLATFORM "")
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
            set(_PLATFORM "linux-x64")
        elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
            set(_PLATFORM "linux-arm64")
        endif()
    elseif(APPLE)
        set(_PLATFORM "macos-universal")
    elseif(WIN32)
        if(
            MSVC
            AND (
                CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64"
                OR CMAKE_SIZEOF_VOID_P EQUAL 8
            )
        )
            get_property(
                _IS_MULTI_CONFIG
                GLOBAL
                PROPERTY GENERATOR_IS_MULTI_CONFIG
            )
            if(NOT _IS_MULTI_CONFIG AND CMAKE_BUILD_TYPE STREQUAL "Debug")
                set(_PLATFORM "windows-x64-Debug")
            else()
                set(_PLATFORM "windows-x64")
            endif()
        endif()
    endif()

    if(NOT _PLATFORM)
        message(
            STATUS
            "- bgfx - no precompiled binary available for this platform / configuration"
        )
        return()
    endif()

    set(_ARCHIVE_NAME "bgfx-install-${_PLATFORM}-v${BGFX_VERSION}.tar.gz")
    set(_URL
        "https://github.com/alemuntoni/install-bgfx-cmake/releases/download/${BGFX_PRECOMPILED_TAG}/${_ARCHIVE_NAME}"
    )

    message(
        STATUS
        "- bgfx - downloading precompiled binaries (${_PLATFORM}) from ${BGFX_PRECOMPILED_TAG}..."
    )
    FetchContent_Declare(vclib_bgfx_precompiled URL "${_URL}")
    FetchContent_MakeAvailable(vclib_bgfx_precompiled)

    FetchContent_GetProperties(vclib_bgfx_precompiled SOURCE_DIR _SOURCE_DIR)

    if(NOT EXISTS "${_SOURCE_DIR}")
        return()
    endif()

    # Ensure binaries have execution permissions on POSIX
    if(UNIX)
        file(GLOB _BINS "${_SOURCE_DIR}/bin/*")
        foreach(_BIN IN LISTS _BINS)
            if(NOT IS_DIRECTORY "${_BIN}")
                file(
                    CHMOD "${_BIN}"
                    PERMISSIONS
                        OWNER_READ
                        OWNER_WRITE
                        OWNER_EXECUTE
                        GROUP_READ
                        GROUP_EXECUTE
                        WORLD_READ
                        WORLD_EXECUTE
                )
            endif()
        endforeach()
    endif()

    # Sanity check: test that shaderc can execute (checks glibc / runtime compatibility)
    find_program(
        _TEST_SHADERC
        NAMES shaderc shaderc.exe
        PATHS "${_SOURCE_DIR}/bin"
        NO_DEFAULT_PATH
    )

    set(_SANITY_OK TRUE)
    if(_TEST_SHADERC)
        execute_process(
            COMMAND "${_TEST_SHADERC}" -v
            RESULT_VARIABLE _SHADERC_RES
            OUTPUT_QUIET
            ERROR_QUIET
            TIMEOUT 10
        )
        if(NOT _SHADERC_RES EQUAL 0)
            message(
                STATUS
                "- bgfx - precompiled shaderc failed sanity check: falling back to building from source"
            )
            set(_SANITY_OK FALSE)
        endif()
    else()
        message(
            STATUS
            "- bgfx - shaderc binary not found in precompiled archive: falling back to building from source"
        )
        set(_SANITY_OK FALSE)
    endif()
    unset(_TEST_SHADERC CACHE)

    if(_SANITY_OK)
        set(${OUT_DIR} "${_SOURCE_DIR}/lib/cmake/bgfx" PARENT_SCOPE)
        set(${OUT_PLATFORM} "${_PLATFORM}" PARENT_SCOPE)
    endif()
endfunction()

if(VCLIB_ALLOW_SYSTEM_BGFX)
    find_package(bgfx QUIET)
endif()

# If not found on system and precompiled is allowed, attempt to download and use precompiled binaries
if(NOT bgfx_FOUND AND VCLIB_ALLOW_PRECOMPILED_BGFX)
    _vclib_download_precompiled_bgfx(_BGFX_PRECOMPILED_DIR _BGFX_PLATFORM)
    if(_BGFX_PRECOMPILED_DIR)
        find_package(
            bgfx
            CONFIG
            PATHS "${_BGFX_PRECOMPILED_DIR}"
            NO_DEFAULT_PATH
        )
        if(bgfx_FOUND)
            set(VCLIB_DOWNLOADED_PRECOMPILED_BGFX ON)
            if(
                NOT CMAKE_MAP_IMPORTED_CONFIG_DEBUG
                AND NOT _BGFX_PLATFORM MATCHES "Debug"
            )
                set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG
                    Release
                    RelWithDebInfo
                    MinSizeRel
                    ""
                )
            endif()
        endif()
    endif()
endif()

if(bgfx_FOUND)
    if(VCLIB_DOWNLOADED_PRECOMPILED_BGFX)
        message(
            STATUS
            "- bgfx - using downloaded precompiled library (${_BGFX_PLATFORM})"
        )
    else()
        message(STATUS "- bgfx - using system-provided library")
    endif()
    set(VCLIB_USED_SYSTEM_BGFX ON CACHE INTERNAL "")

    add_library(vclib-3rd-bgfx INTERFACE)

    target_link_libraries(
        vclib-3rd-bgfx
        INTERFACE
            bgfx::bx
            bgfx::bgfx
            bgfx::bimg
            bgfx::bimg_decode
            bgfx::bimg_encode
            vclib-3rd-stb
    )

    target_include_directories(
        vclib-3rd-bgfx
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/iconfontheaders/include>
    )
    target_include_directories(
        vclib-3rd-bgfx
        INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/sdf/include>
    )

    # make sure that the imported targets are global
    set_target_properties(bgfx::bin2c PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::texturec PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::texturev PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::geometryc PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::geometryv PROPERTIES IMPORTED_GLOBAL TRUE)
    set_target_properties(bgfx::shaderc PROPERTIES IMPORTED_GLOBAL TRUE)

    # get bgfx include path
    get_target_property(
        BGFX_INCLUDE_PATH
        bgfx::bgfx
        INTERFACE_INCLUDE_DIRECTORIES
    )

    # for each path in BGFX_INCLUDE_PATH:
    foreach(PATH ${BGFX_INCLUDE_PATH})
        if(EXISTS ${PATH}/bgfx_shader.sh)
            set(BGFX_SHADER_INCLUDE_PATH ${PATH})
        elseif(EXISTS ${PATH}/bgfx/bgfx_shader.sh)
            set(BGFX_SHADER_INCLUDE_PATH ${PATH}/bgfx)
        endif()
    endforeach(PATH ${BGFX_INCLUDE_PATH})

    set_target_properties(
        vclib-3rd-bgfx
        PROPERTIES BGFX_SHADER_INCLUDE_PATH "${BGFX_SHADER_INCLUDE_PATH}"
    )

    list(APPEND VCLIB_RENDER_OPTIONAL_SYSTEM_LIBRARIES vclib-3rd-bgfx)

elseif(VCLIB_ALLOW_DOWNLOAD_BGFX)

    message(STATUS "- bgfx - using downloaded source")

    # leave the option to build bgfx examples, but set it to OFF by default
    option(BGFX_BUILD_EXAMPLES "Build bgfx examples" OFF)

    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    set(BGFX_OPENGL_VERSION 44)
    if(BUILD_SHARED_LIBS)
        set(BGFX_LIBRARY_TYPE SHARED CACHE STRING "bgfx library type" FORCE)
    endif()

    set(BIMG_DECODE ON CACHE BOOL "" FORCE)
    set(BIMG_CUBEMAP ON CACHE BOOL "" FORCE)

    set(BGFX_EXCLUDE_FROM_ALL_OPTION "")
    if(NOT VCLIB_ALLOW_INSTALL_BGFX)
        set(BGFX_EXCLUDE_FROM_ALL_OPTION EXCLUDE_FROM_ALL)
    endif()

    FetchContent_Declare(
        bgfx.cmake
        GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake
        GIT_TAG v${BGFX_VERSION}
        ${BGFX_EXCLUDE_FROM_ALL_OPTION}
    )

    vclib_begin_3rdparty_install_scope()

    FetchContent_MakeAvailable(bgfx.cmake)

    vclib_end_3rdparty_install_scope()

    FetchContent_GetProperties(bgfx SOURCE_DIR BGFX_SOURCE_DIR)

    # there are some warnings that we need to ignore
    vclib_disable_target_warnings(spirv-cross) #bgfx 3rdparty
    vclib_disable_target_warnings(l-smash) #bgfx 3rdparty

    vclib_disable_target_warnings(bimg_encode) #bimg 3rdparty

    add_library(vclib-3rd-bgfx INTERFACE)

    target_link_libraries(
        vclib-3rd-bgfx
        INTERFACE bx bgfx bimg bimg_decode bimg_encode vclib-3rd-stb
    )

    target_include_directories(
        vclib-3rd-bgfx
        INTERFACE $<BUILD_INTERFACE:${BGFX_SOURCE_DIR}/3rdparty>
    )

    set_target_properties(
        vclib-3rd-bgfx
        PROPERTIES BGFX_SHADER_INCLUDE_PATH ${BGFX_SOURCE_DIR}/src
    )

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-bgfx)
else()
    message(
        FATAL_ERROR
        "bgfx is required - be sure to clone recursively the vclib repository."
    )
endif()

if(TARGET vclib-3rd-bgfx)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/bgfx_config.cmake)

    install(
        FILES ${CMAKE_CURRENT_LIST_DIR}/cmake/bgfx_config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vclib
    )
    install(
        FILES ${CMAKE_CURRENT_LIST_DIR}/cmake/vclib_shader_combinations.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/vclib
    )

    if(UNIX AND NOT APPLE)
        # bgfx.cmake installs a prebuilt x86-64 libdxcompiler.so on Linux into bin/.
        # On non-x86_64 architectures (e.g. ARM64) this is a foreign-architecture binary
        # that breaks dpkg-shlibdeps during DEB packaging. Furthermore, DirectX shader
        # compilation is not used on Linux by VCLib.
        install(
            CODE
                [[
            file(GLOB_RECURSE _DXCOMPILER_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/*libdxcompiler.so*")
            foreach(_F IN LISTS _DXCOMPILER_FILES)
                message(STATUS "Removing unneeded DirectX shader compiler library: ${_F}")
                file(REMOVE "${_F}")
            endforeach()
        ]]
        )
    endif()
endif()
