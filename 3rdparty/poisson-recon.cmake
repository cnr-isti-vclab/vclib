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
        file(RENAME ${poissonrecon_SOURCE_DIR}/Src
            ${poissonrecon_SOURCE_DIR}/include/poisson_recon)
    endif()

    add_library(vclib-3rd-poisson-recon INTERFACE)

    target_include_directories(vclib-3rd-poisson-recon INTERFACE
        ${poissonrecon_SOURCE_DIR}/include)

    target_compile_definitions(vclib-3rd-poisson-recon INTERFACE
        VCLIB_WITH_POISSON_RECON)

    list(APPEND VCLIB_EXTERNAL_3RDPARTY_LIBRARIES vclib-3rd-poisson-recon)
else()
    message(STATUS "- PoissonRecon - not found, skipping")
endif()
