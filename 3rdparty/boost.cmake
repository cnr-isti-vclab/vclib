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

find_package(Boost QUIET)

if (VCLIB_ALLOW_SYSTEM_BOOST AND TARGET Boost::boost)
    message(STATUS "- boost - using system-provided library")

    add_library(vclib-3rd-boost INTERFACE)
    target_link_libraries(vclib-3rd-boost INTERFACE Boost::boost)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-boost)

    target_compile_definitions(vclib-3rd-boost INTERFACE
        VCLIB_WITH_BOOST)
else()
    message(STATUS "- boost - not found, skipping")
endif()
