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

# to install QGLViewer on Mac:
# - download libQGLViewer
# - be sure to have a Qt environment set up
# - cd libQGLViewer/QGLViewer
# - qmake LIB_DIR=/Library/Frameworks
# - make
# - sudo make install

find_package(QGLViewer QUIET)

if (VCLIB_ALLOW_SYSTEM_QGLVIEWER)
    if (VCLIB_ALLOW_SYSTEM_QT AND QT_FOUND AND OpenGL_FOUND)
        if (QGLViewer_FOUND)
            message(STATUS "- QGLViewer - using system-provided library")

            add_library(vclib-external-qglviewer INTERFACE)

            target_include_directories(vclib-external-qglviewer INTERFACE ${QGLVIEWER_INCLUDE_DIR})
            target_link_libraries(vclib-external-qglviewer INTERFACE ${QGLVIEWER_LIBRARY})
            target_compile_definitions(vclib-external-qglviewer INTERFACE VCLIB_WITH_QGLVIEWER)

            list(APPEND VCLIB_RENDER_EXTERNAL_LIBRARIES vclib-external-qglviewer)
        else()
            message(STATUS "- QGLViewer - not found, skipping")
        endif()
    else()
        # message indicating why we jumped QGLViewer
        set (MISSING_LIB)
        if (NOT OpenGL_FOUND)
            list(APPEND MISSING_LIB OpenGL)
        endif()
        if (NOT QT_FOUND OR NOT VCLIB_ALLOW_SYSTEM_QT)
            list(APPEND MISSING_LIB Qt)
        endif()

        message(STATUS "- QGLViewer - ignored: missing ${MISSING_LIB}")
    endif()
endif()
