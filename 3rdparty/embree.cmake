#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
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

set(EMBREE_MAJOR 4)
set(EMBREE_MINOR 3.3)
set(EMBREE_VER "${EMBREE_MAJOR}.${EMBREE_MINOR}")

find_package(embree ${EMBREE_MAJOR} QUIET)

if (VCLIB_ALLOW_SYSTEM_EMBREE AND embree_FOUND)
    message(STATUS "- Embree - using system-provided library")

    add_library(vclib-3rd-embree INTERFACE)
    target_link_libraries(vclib-3rd-embree INTERFACE embree)

    list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-embree)
    target_compile_definitions(vclib-3rd-embree INTERFACE VCLIB_WITH_EMBREE)
else()
    message(STATUS "- Embree - not found, skipping")
endif()
