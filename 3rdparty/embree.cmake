# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(EMBREE_MAJOR 4)
set(EMBREE_MINOR 3)
set(EMBREE_PATCH 3)
set(EMBREE_VER "${EMBREE_MAJOR}.${EMBREE_MINOR}.${EMBREE_PATCH}")

find_package(embree ${EMBREE_MAJOR} QUIET)

if (VCLIB_ALLOW_SYSTEM_EMBREE AND embree_FOUND)
    message(STATUS "- Embree - using system-provided library")

    add_library(vclib-3rd-embree INTERFACE)
    target_link_libraries(vclib-3rd-embree INTERFACE embree)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-embree)
    target_compile_definitions(vclib-3rd-embree INTERFACE VCLIB_WITH_EMBREE)
else()
    message(STATUS "- Embree - not found, skipping")
endif()
