#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
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
