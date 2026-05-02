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

set(CGAL_VER 5.6)

find_package(Threads REQUIRED QUIET)
find_package(CGAL ${CGAL_VER} QUIET)

if (VCLIB_ALLOW_SYSTEM_CGAL AND CGAL_FOUND)
    message(STATUS "- CGAL - using system-provided library")

    add_library(vclib-3rd-cgal INTERFACE)
    target_link_libraries(vclib-3rd-cgal INTERFACE CGAL::CGAL)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-cgal)

    target_compile_definitions(vclib-3rd-cgal INTERFACE
        VCLIB_WITH_CGAL)

elseif(VCLIB_ALLOW_DOWNLOAD_CGAL)
    message(STATUS "- CGAL - using downloaded source")

    FetchContent_Declare(cgal
        GIT_REPOSITORY https://github.com/CGAL/cgal.git
        GIT_TAG        v${CGAL_VER}
        EXCLUDE_FROM_ALL)

    FetchContent_MakeAvailable(cgal)

    add_library(vclib-3rd-cgal INTERFACE)
    target_link_libraries(vclib-3rd-cgal INTERFACE cgal)

    if (WIN32)
        set(GMP_MPFR_URL
            https://github.com/CGAL/cgal/releases/download/v${CGAL_VER}/CGAL-${CGAL_VER}-win64-auxiliary-libraries-gmp-mpfr.zip)

        # download zip package from GMP_MPFR_URL
        file(DOWNLOAD ${GMP_MPFR_URL} ${CMAKE_CURRENT_BINARY_DIR}/gmp_mpfr.zip)

        # extract the zip in the cgal build directory
        file(ARCHIVE_EXTRACT INPUT ${CMAKE_CURRENT_BINARY_DIR}/gmp_mpfr.zip
            DESTINATION ${cgal_SOURCE_DIR})
    endif()

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-cgal)

    target_compile_definitions(vclib-3rd-cgal INTERFACE
        VCLIB_WITH_CGAL)

else()
    message(STATUS "- CGAL - not found, skipping")
endif()
