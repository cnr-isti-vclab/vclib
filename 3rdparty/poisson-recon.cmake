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

set(POISSON_RECON_HASH_COMMIT 262b0f539d404057d1f36e1adc07fc9388678899)

if(VCLIB_ALLOW_DOWNLOAD_POISSON_RECON)
    message(STATUS "- PoissonRecon - using downloaded source")

    FetchContent_Declare(poissonrecon
        GIT_REPOSITORY https://github.com/mkazhdan/PoissonRecon
        GIT_TAG ${POISSON_RECON_HASH_COMMIT}
        EXCLUDE_FROM_ALL)

    FetchContent_MakeAvailable(poissonrecon)

    if (NOT EXISTS ${poissonrecon_SOURCE_DIR}/include/poisson_recon)
        file(MAKE_DIRECTORY ${poissonrecon_SOURCE_DIR}/include)

        # Create the poisson_recon directory
        file(MAKE_DIRECTORY ${poissonrecon_SOURCE_DIR}/include/poisson_recon)

        # Find all .h and .inl files in the original Src directory and copy them
        file(GLOB_RECURSE poisson_recon_headers
            "${poissonrecon_SOURCE_DIR}/Src/*.h"
            "${poissonrecon_SOURCE_DIR}/Src/*.inl")

        foreach(header_file ${poisson_recon_headers})
            # Get the relative path from Src directory
            string(REGEX
                REPLACE "${poissonrecon_SOURCE_DIR}/Src/" ""
                relative_path ${header_file})
            # Create the target directory structure if needed
            get_filename_component(target_dir ${relative_path} DIRECTORY)
            if(NOT "${target_dir}" STREQUAL "")
                file(MAKE_DIRECTORY
                    ${poissonrecon_SOURCE_DIR}/include/poisson_recon/${target_dir})
            endif()
            # Copy the file
            file(COPY ${header_file}
                DESTINATION ${poissonrecon_SOURCE_DIR}/include/poisson_recon/${target_dir})
        endforeach()
    endif()

    add_library(vclib-3rd-poisson-recon INTERFACE)

    target_include_directories(vclib-3rd-poisson-recon INTERFACE
        ${poissonrecon_SOURCE_DIR}/include)

    target_compile_definitions(vclib-3rd-poisson-recon INTERFACE
        VCLIB_WITH_POISSON_RECON)

    # Install
    if (VCLIB_ALLOW_INSTALL_POISSON_RECON)
        install(DIRECTORY ${poissonrecon_SOURCE_DIR}/include/poisson_recon
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    endif()

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-poisson-recon)
else()
    message(STATUS "- PoissonRecon - not found, skipping")
endif()
