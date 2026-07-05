# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(CGAL_VER 6.1.1)

find_package(Threads REQUIRED QUIET)
find_package(CGAL QUIET)

if(VCLIB_ALLOW_SYSTEM_CGAL AND CGAL_FOUND)
    message(STATUS "- CGAL - using system-provided library")
    set(VCLIB_USED_SYSTEM_CGAL ON CACHE INTERNAL "")

    add_library(vclib-3rd-cgal INTERFACE)
    target_link_libraries(vclib-3rd-cgal INTERFACE CGAL::CGAL)

    list(APPEND VCLIB_EXTERNAL_OPTIONAL_SYSTEM_LIBRARIES vclib-3rd-cgal)

    target_compile_definitions(vclib-3rd-cgal INTERFACE VCLIB_WITH_CGAL)

elseif(VCLIB_ALLOW_DOWNLOAD_CGAL)
    message(STATUS "- CGAL - using downloaded source")

    FetchContent_Declare(
        cgal
        URL
            https://github.com/CGAL/cgal/releases/download/v${CGAL_VER}/CGAL-${CGAL_VER}.zip
        SOURCE_SUBDIR "include" # trick to avoid add_subdir of cgal directory
    )

    FetchContent_MakeAvailable(cgal)

    add_library(vclib-3rd-cgal INTERFACE)
    target_include_directories(
        vclib-3rd-cgal
        INTERFACE "${cgal_SOURCE_DIR}/include"
    )

    if(WIN32)
        # populate in ${cgal_SOURCE_DIR} the auxiliary libraries for GMP and MPFR, which are required by CGAL
        FetchContent_Declare(
            gmp-mpfr
            URL
                https://github.com/CGAL/cgal/releases/download/v${CGAL_VER}/CGAL-${CGAL_VER}-win64-auxiliary-libraries-gmp-mpfr.zip
            SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/gmpmpfr_temp"
        )

        FetchContent_MakeAvailable(gmp-mpfr)

        file(
            COPY "${CMAKE_CURRENT_BINARY_DIR}/gmpmpfr_temp/"
            DESTINATION "${cgal_SOURCE_DIR}/auxiliary"
        )

        add_library(mpfr SHARED IMPORTED GLOBAL)
        add_library(gmp SHARED IMPORTED GLOBAL)
        add_library(gmpxx SHARED IMPORTED GLOBAL)

        set_property(
            TARGET mpfr
            PROPERTY
                IMPORTED_IMPLIB "${cgal_SOURCE_DIR}/auxiliary/gmp/lib/mpfr.lib"
        )
        set_property(
            TARGET mpfr
            PROPERTY
                IMPORTED_LOCATION
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/bin/mpfr-6.dll"
        )
        target_include_directories(
            mpfr
            INTERFACE "${cgal_SOURCE_DIR}/auxiliary/gmp/include"
        )

        set_property(
            TARGET gmp
            PROPERTY
                IMPORTED_IMPLIB "${cgal_SOURCE_DIR}/auxiliary/gmp/lib/gmp.lib"
        )
        set_property(
            TARGET gmp
            PROPERTY
                IMPORTED_LOCATION
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/bin/gmp-10.dll"
        )
        target_include_directories(
            gmp
            INTERFACE "${cgal_SOURCE_DIR}/auxiliary/gmp/include"
        )

        set_property(
            TARGET gmpxx
            PROPERTY
                IMPORTED_IMPLIB "${cgal_SOURCE_DIR}/auxiliary/gmp/lib/gmpxx.lib"
        )
        set_property(
            TARGET gmpxx
            PROPERTY
                IMPORTED_LOCATION
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/bin/gmpxx-4.dll"
        )
        target_include_directories(
            gmpxx
            INTERFACE "${cgal_SOURCE_DIR}/auxiliary/gmp/include"
        )

        target_link_libraries(
            vclib-3rd-cgal
            INTERFACE gmp gmpxx mpfr Threads::Threads
        )
    endif()

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-cgal)

    target_compile_definitions(vclib-3rd-cgal INTERFACE VCLIB_WITH_CGAL)

    # Install
    if(VCLIB_ALLOW_INSTALL_CGAL)
        install(
            DIRECTORY "${cgal_SOURCE_DIR}/include/CGAL"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
        if(WIN32)
            # Install headers
            install(
                DIRECTORY "${cgal_SOURCE_DIR}/auxiliary/gmp/include/"
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
            # Install .lib files
            install(
                FILES 
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/lib/gmp.lib"
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/lib/gmpxx.lib"
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/lib/mpfr.lib"
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
            )
            # Install .dll files
            install(
                FILES 
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/bin/gmp-10.dll"
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/bin/gmpxx-4.dll"
                    "${cgal_SOURCE_DIR}/auxiliary/gmp/bin/mpfr-6.dll"
                DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
        endif()
    endif()

else()
    message(STATUS "- CGAL - not found, skipping")
endif()
