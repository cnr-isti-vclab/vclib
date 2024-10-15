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

cmake_minimum_required(VERSION 3.24)
project(vclib-external)

include(FetchContent)

# === OPTIONAL === #

#VCG
option(VCLIB_ALLOW_DOWNLOAD_VCG "Allow use of downloaded VCG source" ON)
option(VCLIB_ALLOW_SYSTEM_VCG "Allow use of system-provided VCG" ON)


# === Fetch dependencies === #

set(VCLIB_EXTERNAL_3RDPARTY_LIBRARIES "")

### VCG
include(vcg.cmake)

set(VCLIB_EXTERNAL_3RDPARTY_LIBRARIES ${VCLIB_EXTERNAL_3RDPARTY_LIBRARIES} PARENT_SCOPE)
