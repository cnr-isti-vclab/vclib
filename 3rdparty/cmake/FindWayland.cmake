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

# Try to find Wayland.
#
# This module defines the following targets:
#   Wayland::Wayland
#   Wayland::Client
#   Wayland::Server
#   Wayland::EGL
#   Wayland::Cursor
#
# It also defines the following variables:
#   WAYLAND_FOUND, whether the system has Wayland
#   WAYLAND_INCLUDE_DIR, where to find wayland-client.h, wayland-server.h,
#                        wayland-egl.h and wayland-cursor.h
#   WAYLAND_LIBRARIES, the libraries to link against to use Wayland
#   WAYLAND_DEFINITIONS, compiler switches required for using Wayland
# It also defines the following variables, where [MOD] is either CLIENT, SERVER,
# EGL or CURSOR:
#   WAYLAND_[MOD]_FOUND, whether the system has wayland-[mod]
#   WAYLAND_[MOD]_INCLUDE_DIR, where to find wayland-[mod].h
#   WAYLAND_[MOD]_LIBRARIES, the libraries to link against to use wayland-[mod]

if(LINUX)
    if(WAYLAND_INCLUDE_DIR AND WAYLAND_LIBRARIES)
        set(WAYLAND_FIND_QUIETLY TRUE) # already cached
    endif()

    # Use pkg-config to get the directories and then use these values
    # in the FIND_PATH() and FIND_LIBRARY() calls
    find_package(PkgConfig)
    pkg_check_modules(PKG_WAYLAND QUIET
        wayland-client wayland-server wayland-egl wayland-cursor)

    set(WAYLAND_DEFINITIONS ${PKG_WAYLAND_CFLAGS})

    find_path(WAYLAND_CLIENT_INCLUDE_DIR
        NAMES wayland-client.h
        HINTS ${PKG_WAYLAND_INCLUDE_DIRS})

    find_path(WAYLAND_SERVER_INCLUDE_DIR
        NAMES wayland-server.h
        HINTS ${PKG_WAYLAND_INCLUDE_DIRS})

    find_path(WAYLAND_EGL_INCLUDE_DIR
        NAMES wayland-egl.h
        HINTS ${PKG_WAYLAND_INCLUDE_DIRS})

    find_path(WAYLAND_CURSOR_INCLUDE_DIR
        NAMES wayland-cursor.h
        HINTS ${PKG_WAYLAND_INCLUDE_DIRS})

    find_library(WAYLAND_CLIENT_LIBRARIES
        NAMES wayland-client
        HINTS ${PKG_WAYLAND_LIBRARY_DIRS})

    find_library(WAYLAND_SERVER_LIBRARIES
        NAMES wayland-server
        HINTS ${PKG_WAYLAND_LIBRARY_DIRS})

    find_library(WAYLAND_EGL_LIBRARIES
        NAMES wayland-egl
        HINTS ${PKG_WAYLAND_LIBRARY_DIRS})

    find_library(WAYLAND_CURSOR_LIBRARIES
        NAMES wayland-cursor
        HINTS ${PKG_WAYLAND_LIBRARY_DIRS})

    set(WAYLAND_INCLUDE_DIR
        ${WAYLAND_CLIENT_INCLUDE_DIR}
        ${WAYLAND_SERVER_INCLUDE_DIR}
        ${WAYLAND_EGL_INCLUDE_DIR}
        ${WAYLAND_CURSOR_INCLUDE_DIR})

    set(WAYLAND_LIBRARIES
        ${WAYLAND_CLIENT_LIBRARIES}
        ${WAYLAND_SERVER_LIBRARIES}
        ${WAYLAND_EGL_LIBRARIES}
        ${WAYLAND_CURSOR_LIBRARIES})

    list(REMOVE_DUPLICATES WAYLAND_INCLUDE_DIR)

    include(FindPackageHandleStandardArgs)

    find_package_handle_standard_args(WAYLAND_CLIENT
        REQUIRED_VARS
            WAYLAND_CLIENT_LIBRARIES
            WAYLAND_CLIENT_INCLUDE_DIR
        NAME_MISMATCHED)

    find_package_handle_standard_args(WAYLAND_SERVER
        REQUIRED_VARS
            WAYLAND_SERVER_LIBRARIES
            WAYLAND_SERVER_INCLUDE_DIR
        NAME_MISMATCHED)

    find_package_handle_standard_args(WAYLAND_EGL
        REQUIRED_VARS
            WAYLAND_EGL_LIBRARIES
            WAYLAND_EGL_INCLUDE_DIR
        NAME_MISMATCHED)

    find_package_handle_standard_args(WAYLAND_CURSOR
        REQUIRED_VARS
            WAYLAND_CURSOR_LIBRARIES
            WAYLAND_CURSOR_INCLUDE_DIR
        NAME_MISMATCHED)

    find_package_handle_standard_args(Wayland
        REQUIRED_VARS
            WAYLAND_LIBRARIES
            WAYLAND_INCLUDE_DIR
        NAME_MISMATCHED)

    # create targets under Wayland namespace
    if(WAYLAND_CLIENT_LIBRARIES)
        add_library(Wayland::Client INTERFACE IMPORTED)
        set_property(TARGET Wayland::Client
            PROPERTY
                INTERFACE_LINK_LIBRARIES ${WAYLAND_CLIENT_LIBRARIES})
        set_property(TARGET Wayland::Client
            PROPERTY
                INTERFACE_INCLUDE_DIRECTORIES ${WAYLAND_CLIENT_INCLUDE_DIR})
    endif()

    if(WAYLAND_SERVER_LIBRARIES)
        add_library(Wayland::Server INTERFACE IMPORTED)
        set_property(TARGET Wayland::Server
            PROPERTY
                INTERFACE_LINK_LIBRARIES ${WAYLAND_SERVER_LIBRARIES})
        set_property(TARGET Wayland::Server
            PROPERTY
                INTERFACE_INCLUDE_DIRECTORIES ${WAYLAND_SERVER_INCLUDE_DIR})
    endif()

    if(WAYLAND_EGL_LIBRARIES)
        add_library(Wayland::EGL INTERFACE IMPORTED)
        set_property(TARGET Wayland::EGL
            PROPERTY
                INTERFACE_LINK_LIBRARIES ${WAYLAND_EGL_LIBRARIES})
        set_property(TARGET Wayland::EGL
            PROPERTY
                INTERFACE_INCLUDE_DIRECTORIES ${WAYLAND_EGL_INCLUDE_DIR})
    endif()

    if(WAYLAND_CURSOR_LIBRARIES)
        add_library(Wayland::Cursor INTERFACE IMPORTED)
        set_property(TARGET Wayland::Cursor
            PROPERTY
                INTERFACE_LINK_LIBRARIES ${WAYLAND_CURSOR_LIBRARIES})
        set_property(TARGET Wayland::Cursor
            PROPERTY
                INTERFACE_INCLUDE_DIRECTORIES ${WAYLAND_CURSOR_INCLUDE_DIR})
    endif()

    if(WAYLAND_LIBRARIES)
        add_library(Wayland::Wayland INTERFACE IMPORTED)
        set_property(TARGET Wayland::Wayland
            PROPERTY
                INTERFACE_LINK_LIBRARIES ${WAYLAND_LIBRARIES})
        set_property(TARGET Wayland::Wayland
            PROPERTY
                INTERFACE_INCLUDE_DIRECTORIES ${WAYLAND_INCLUDE_DIR})
    endif()

    mark_as_advanced(
        WAYLAND_INCLUDE_DIR         WAYLAND_LIBRARIES
        WAYLAND_CLIENT_INCLUDE_DIR  WAYLAND_CLIENT_LIBRARIES
        WAYLAND_SERVER_INCLUDE_DIR  WAYLAND_SERVER_LIBRARIES
        WAYLAND_EGL_INCLUDE_DIR     WAYLAND_EGL_LIBRARIES
        WAYLAND_CURSOR_INCLUDE_DIR  WAYLAND_CURSOR_LIBRARIES)
endif()
