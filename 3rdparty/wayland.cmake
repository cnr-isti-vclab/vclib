# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

find_package(Wayland QUIET)

if (LINUX AND VCLIB_RENDER_WITH_WAYLAND)
    if (Wayland_FOUND)
        message(STATUS "- Wayland - using system-provided library")

        add_library(vclib-3rd-wayland INTERFACE)

        target_link_libraries(vclib-3rd-wayland INTERFACE Wayland::Wayland)
        target_compile_definitions(vclib-3rd-wayland INTERFACE VCLIB_RENDER_WITH_WAYLAND)

        list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-wayland)
    else()
        message(STATUS "- Wayland - not found, skipping")
    endif()
endif()
