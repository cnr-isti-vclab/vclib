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

if (VCLIB_BUILD_AND_RUN_TESTS)
    find_package(Catch2 3 QUIET)

    if(VCLIB_ALLOW_SYSTEM_CATCH2 AND TARGET Catch2::Catch2WithMain)
        message(STATUS "- Catch2 - using system-provided library")
    elseif (VCLIB_ALLOW_DOWNLOAD_CATCH2)
        message(STATUS "- Catch2 - using downloaded source")

        FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.3.2)

        FetchContent_MakeAvailable(Catch2)
    else()
        message(
            FATAL_ERROR
            "Catch2 is required to build tests - VCLIB_ALLOW_DOWNLOAD_CATCH2 must be enabled and found.")
    endif()

    add_library(vclib-external-catch2 INTERFACE)
    target_link_libraries(vclib-external-catch2 INTERFACE Catch2::Catch2WithMain)

    list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-catch2)
endif()
