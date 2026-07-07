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

if(VCLIB_ALLOW_SYSTEM_EMBREE)
    find_package(embree ${EMBREE_MAJOR} QUIET)
endif()

if(NOT embree_FOUND AND WIN32 AND VCLIB_ALLOW_DOWNLOAD_EMBREE)
    include(FetchContent)
    
    FetchContent_Declare(
        vclib_embree_win
        URL "https://github.com/embree/embree/releases/download/v${EMBREE_VER}/embree-${EMBREE_VER}.x64.windows.zip"
    )
    
    FetchContent_MakeAvailable(vclib_embree_win)
    
    set(embree_DIR "${vclib_embree_win_SOURCE_DIR}/lib/cmake/embree-${EMBREE_VER}")
    find_package(embree ${EMBREE_MAJOR} QUIET)
    
    if(embree_FOUND)
        set(VCLIB_DOWNLOADED_EMBREE ON)
        if(VCLIB_INSTALL_MODULE_EXTERNAL)
            install(
                FILES 
                    "${vclib_embree_win_SOURCE_DIR}/bin/embree${EMBREE_MAJOR}.dll"
                    "${vclib_embree_win_SOURCE_DIR}/bin/tbb12.dll"
                DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
            install(
                FILES 
                    "${vclib_embree_win_SOURCE_DIR}/lib/embree${EMBREE_MAJOR}.lib"
                    "${vclib_embree_win_SOURCE_DIR}/lib/tbb12.lib"
                DESTINATION ${CMAKE_INSTALL_LIBDIR}
            )
            install(
                DIRECTORY "${vclib_embree_win_SOURCE_DIR}/include/embree4"
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            )
        endif()
    endif()
endif()

if(embree_FOUND)
    if(VCLIB_DOWNLOADED_EMBREE)
        message(STATUS "- Embree - using downloaded library")
        set(VCLIB_USED_SYSTEM_EMBREE OFF CACHE INTERNAL "")
    else()
        message(STATUS "- Embree - using system-provided library")
        set(VCLIB_USED_SYSTEM_EMBREE ON CACHE INTERNAL "")
    endif()

    add_library(vclib-3rd-embree INTERFACE)
    target_link_libraries(vclib-3rd-embree INTERFACE embree)

    # If we downloaded Embree, also expose its embedded TBB as an imported target
    # so that CMake's TARGET_RUNTIME_DLLS feature can detect and copy tbb12.dll automatically.
    if(VCLIB_DOWNLOADED_EMBREE AND WIN32)
        add_library(vclib-3rd-embree-tbb SHARED IMPORTED)
        set_target_properties(vclib-3rd-embree-tbb PROPERTIES
            IMPORTED_IMPLIB "${vclib_embree_win_SOURCE_DIR}/lib/tbb12.lib"
            IMPORTED_LOCATION "${vclib_embree_win_SOURCE_DIR}/bin/tbb12.dll"
        )
        target_link_libraries(vclib-3rd-embree INTERFACE vclib-3rd-embree-tbb)
    endif()

    list(APPEND VCLIB_EXTERNAL_OPTIONAL_SYSTEM_LIBRARIES vclib-3rd-embree)
    target_compile_definitions(vclib-3rd-embree INTERFACE VCLIB_WITH_EMBREE)
else()
    message(STATUS "- Embree - not found, skipping")
endif()
