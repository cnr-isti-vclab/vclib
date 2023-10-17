#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/


if (VCLIB_ALLOW_SYSTEM_VCG AND (DEFINED VCG_INCLUDE_DIRS OR DEFINED VCG_DIR))
    if (DEFINED VCG_DIR)
        set(VCG_INCLUDE_DIRS ${VCG_DIR})
        set(VCG_INCLUDE_DIRS ${VCG_DIR} PARENT_SCOPE)
    endif()
    message(STATUS "- VCG - using system-provided library")
    set(VCLIB_USES_VCG TRUE)
else()
    if (VCLIB_ALLOW_DOWNLOAD_VCG)
        message(STATUS "- VCG - using downloaded source")
        include(FetchContent)
        FetchContent_Declare(
            vcglib
            GIT_REPOSITORY https://github.com/cnr-isti-vclab/vcglib.git
            GIT_TAG        2022.02)

        FetchContent_MakeAvailable(vcglib)
        set(VCG_INCLUDE_DIRS ${vcglib_SOURCE_DIR})
    else()
        message(STATUS "- VCG - not found, skipping")
    endif()
endif()

if (VCLIB_USES_VCG)
    add_library(vclib-external-vcg INTERFACE)
    target_include_directories(vclib-external-vcg INTERFACE
        ${VCG_INCLUDE_DIRS})

    list(APPEND VCLIB_EXTERNAL_LIBRARIES vclib-external-vcg)
endif()
