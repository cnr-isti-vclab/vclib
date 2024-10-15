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

find_package(Qt5 COMPONENTS Core Xml OpenGL Gui Widgets QUIET)
find_package(Qt6 COMPONENTS Core Xml OpenGL Gui Widgets OpenGLWidgets QUIET)

if (Qt6_FOUND OR Qt5_FOUND)
    set(QT_FOUND TRUE)
    if(Qt6_FOUND)
        set(QT_VER 6)
    else()
        set(QT_VER 5)
    endif()
endif()

if (VCLIB_ALLOW_SYSTEM_QT)
    if (QT_FOUND)
        message(STATUS "- Qt${QT_VER} - using system-provided library")

        add_library(vclib-3rd-qt INTERFACE)
        target_compile_definitions(vclib-3rd-qt INTERFACE
            VCLIB_WITH_QT)

        # prefer Qt6
        if (Qt6_FOUND)
            target_link_libraries(vclib-3rd-qt INTERFACE Qt6::Core Qt6::Widgets Qt6::Xml)

            if (OpenGL_FOUND)
                target_link_libraries(vclib-3rd-qt INTERFACE Qt6::OpenGL Qt6::OpenGLWidgets)
            endif()
        endif()

        if (Qt5_FOUND AND NOT Qt6_FOUND)
            target_link_libraries(vclib-3rd-qt INTERFACE Qt5::Core Qt5::Widgets Qt5::Xml)

            if (OpenGL_FOUND)
                target_link_libraries(vclib-3rd-qt INTERFACE Qt5::OpenGL)
            endif()
        endif()

        list(APPEND VCLIB_RENDER_3RDPARTY_LIBRARIES vclib-3rd-qt)
    else()
        message(STATUS "- Qt - not found, skipping")
    endif()
endif()


