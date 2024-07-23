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

set(POOLSTL_VERSION 0.3.5)
set(VCLIB_POOLSTL_DIR ${CMAKE_CURRENT_LIST_DIR}/poolSTL-${POOLSTL_VERSION})

if (VCLIB_ALLOW_BUNDLED_POOLSTL AND 
        EXISTS ${VCLIB_POOLSTL_DIR}/include/poolstl/poolstl.hpp)

    message(STATUS "- poolstl - using bundled source")

    set(POOLSTL_INCLUDE_DIRS ${VCLIB_POOLSTL_DIR}/include)

    add_library(vclib-external-poolstl INTERFACE)

    target_include_directories(vclib-external-poolstl INTERFACE 
        ${POOLSTL_INCLUDE_DIRS})

    list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-poolstl)
else()
    message(FATAL_ERROR "poolSTL is required to full support parallel algorithms - VCLIB_ALLOW_BUNDLED_POOLSTL must be enabled and found.")
endif()
