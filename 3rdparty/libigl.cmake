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

set(LIBIGL_VER 2.6.0)

find_package(libigl QUIET)

if (VCLIB_ALLOW_SYSTEM_LIBIGL AND libigl_FOUND)
    message(STATUS "- libigl - using system-provided library")

    add_library(vclib-3rd-libigl INTERFACE)
    target_link_libraries(vclib-3rd-libigl INTERFACE
        igl::core igl::copyleft::cgal)

    target_compile_definitions(vclib-3rd-libigl INTERFACE
        VCLIB_WITH_LIBIGL)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-libigl)

elseif(VCLIB_ALLOW_DOWNLOAD_LIBIGL)
    message(STATUS "- libigl - using downloaded source")

    FetchContent_Declare(libigl
        GIT_REPOSITORY https://github.com/libigl/libigl.git
        GIT_TAG        v${LIBIGL_VER}
        EXCLUDE_FROM_ALL)

    FetchContent_MakeAvailable(libigl)

    add_library(vclib-3rd-libigl INTERFACE)
    target_link_libraries(vclib-3rd-libigl INTERFACE igl::core)

    target_compile_definitions(vclib-3rd-libigl INTERFACE
        VCLIB_WITH_LIBIGL)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-libigl)
else()
    message(STATUS "- libigl - not found, skipping")
endif()
