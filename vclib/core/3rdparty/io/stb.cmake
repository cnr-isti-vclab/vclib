#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2025                                                    *
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

set(VCLIB_STB_DIR ${CMAKE_CURRENT_LIST_DIR}/stb-master)

if (VCLIB_ALLOW_BUNDLED_STB AND EXISTS ${VCLIB_STB_DIR}/stb/stb_image.h)
    message(STATUS "- STB - using bundled source")

    set(STB_INCLUDE_DIRS ${VCLIB_STB_DIR})

    add_library(vclib-3rd-stb INTERFACE)
    set_property(TARGET vclib-3rd-stb PROPERTY POSITION_INDEPENDENT_CODE ON)

    target_include_directories(vclib-3rd-stb INTERFACE ${STB_INCLUDE_DIRS})

    target_compile_definitions(vclib-3rd-stb INTERFACE
            VCLIB_WITH_STB)

    list(APPEND VCLIB_CORE_3RDPARTY_LIBRARIES vclib-3rd-stb)

    if (VCLIB_ALLOW_INSTALL_STB)
        install(DIRECTORY ${VCLIB_STB_DIR}/stb
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h")
    endif()
endif()
