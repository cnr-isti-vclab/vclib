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
#* it under the terms of the GNU General Public License as published by      *
#* the Free Software Foundation; either version 3 of the License, or         *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
#* for more details.                                                         *
#****************************************************************************/

if (UNIX)
    find_package(Threads QUIET)
    find_package(TBB QUIET)

    if (VCLIB_ALLOW_SYSTEM_TBB)
        if (TARGET TBB::tbb AND TARGET Threads::Threads)
            message(STATUS "- TBB - using system-provided library")

            add_library(vclib-external-tbb INTERFACE)
            target_link_libraries(vclib-external-tbb INTERFACE TBB::tbb Threads::Threads)

            list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-tbb)
        else()
            message(STATUS "- TBB - not found, skipping")
        endif()
    endif()
endif()
