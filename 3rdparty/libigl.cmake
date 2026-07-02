# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(LIBIGL_VER 2.6.0)

find_package(libigl QUIET)

if(VCLIB_ALLOW_SYSTEM_LIBIGL AND libigl_FOUND)
    message(STATUS "- libigl - using system-provided library")

    add_library(vclib-3rd-libigl INTERFACE)
    target_link_libraries(
        vclib-3rd-libigl
        INTERFACE igl::core igl::copyleft::cgal
    )

    target_compile_definitions(vclib-3rd-libigl INTERFACE VCLIB_WITH_LIBIGL)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-libigl)

elseif(VCLIB_ALLOW_DOWNLOAD_LIBIGL)
    message(STATUS "- libigl - using downloaded source")

    FetchContent_Declare(
        libigl
        GIT_REPOSITORY https://github.com/libigl/libigl.git
        GIT_TAG v${LIBIGL_VER}
        EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(libigl)

    add_library(vclib-3rd-libigl INTERFACE)
    target_link_libraries(vclib-3rd-libigl INTERFACE igl::core)

    target_compile_definitions(vclib-3rd-libigl INTERFACE VCLIB_WITH_LIBIGL)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-libigl)
else()
    message(STATUS "- libigl - not found, skipping")
endif()
