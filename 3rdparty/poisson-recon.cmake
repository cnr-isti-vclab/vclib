# VCLib - Visual Computing Library
# Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at https://mozilla.org/MPL/2.0/.

set(POISSON_RECON_HASH_COMMIT 262b0f539d404057d1f36e1adc07fc9388678899)

if(VCLIB_ALLOW_DOWNLOAD_POISSON_RECON)
    message(STATUS "- PoissonRecon - using downloaded source")

    FetchContent_Declare(
        poissonrecon
        GIT_REPOSITORY https://github.com/mkazhdan/PoissonRecon
        GIT_TAG ${POISSON_RECON_HASH_COMMIT}
        EXCLUDE_FROM_ALL
    )

    FetchContent_MakeAvailable(poissonrecon)

    if(NOT EXISTS ${poissonrecon_SOURCE_DIR}/include/poisson_recon)
        file(MAKE_DIRECTORY ${poissonrecon_SOURCE_DIR}/include)

        # Create the poisson_recon directory
        file(MAKE_DIRECTORY ${poissonrecon_SOURCE_DIR}/include/poisson_recon)

        # Find all .h and .inl files in the original Src directory and copy them
        file(
            GLOB_RECURSE poisson_recon_headers
            "${poissonrecon_SOURCE_DIR}/Src/*.h"
            "${poissonrecon_SOURCE_DIR}/Src/*.inl"
        )

        foreach(header_file ${poisson_recon_headers})
            # Get the relative path from Src directory
            string(
                REGEX REPLACE "${poissonrecon_SOURCE_DIR}/Src/"
                ""
                relative_path
                ${header_file}
            )
            # Create the target directory structure if needed
            get_filename_component(target_dir ${relative_path} DIRECTORY)
            if(NOT "${target_dir}" STREQUAL "")
                file(
                    MAKE_DIRECTORY
                        ${poissonrecon_SOURCE_DIR}/include/poisson_recon/${target_dir}
                )
            endif()
            # Copy the file
            file(
                COPY ${header_file}
                DESTINATION
                    ${poissonrecon_SOURCE_DIR}/include/poisson_recon/${target_dir}
            )
        endforeach()
    endif()

    add_library(vclib-3rd-poisson-recon INTERFACE)

    target_include_directories(
        vclib-3rd-poisson-recon
        INTERFACE ${poissonrecon_SOURCE_DIR}/include
    )

    target_compile_definitions(
        vclib-3rd-poisson-recon
        INTERFACE VCLIB_WITH_POISSON_RECON
    )

    # Install
    if(VCLIB_ALLOW_INSTALL_POISSON_RECON)
        install(
            DIRECTORY ${poissonrecon_SOURCE_DIR}/include/poisson_recon
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
    endif()

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-poisson-recon)
else()
    message(STATUS "- PoissonRecon - not found, skipping")
endif()
