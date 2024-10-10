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

# Try to find libQGLViewer lib
#
# This module reads hints about search locations from the following
# environment or cmake variables:
#
# QGLVIEWER_ROOT
# QGLVIEWER_ROOT_DIR

find_path(QGLVIEWER_INCLUDE_DIR qglviewer.h
    /usr/include/QGLViewer
    /usr/local/include/QGLViewer
    /opt/local/include/QGLViewer
    /usr/local/include/QGLViewer
    /Library/Frameworks/QGLViewer.framework/Headers
    /sw/include/QGLViewer
    C:/libs/QGLViewer/include/QGLViewer
    ${QGLVIEWER_ROOT}
    ${QGLVIEWER_ROOT_DIR}
    ENV QGLVIEWER_ROOT
    ENV QGLVIEWER_ROOT_DIR)

if (Qt6_FOUND)
    set(QT_SUFFIX -qt6)
else()
    set(QT_SUFFIX -qt5)
endif()

if (WIN32)
    get_filename_component(QGLVIEWER_INCLUDE_DIR ${QGLVIEWER_INCLUDE_DIR} DIRECTORY)
endif()

find_library(QGLVIEWER_LIBRARY_RELEASE
    NAMES qglviewer QGLViewer QGLViewer2 qglviewer${QT_SUFFIX} QGLViewer${QT_SUFFIX}
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
        C:/libs/QGLViewer/lib
        ${QGLVIEWER_ROOT}
        ${QGLVIEWER_ROOT_DIR}
        ENV QGLVIEWER_ROOT
        ENV QGLVIEWER_ROOT_DIR
        ENV LD_LIBRARY_PATH
        ENV LIBRARY_PATH
    PATH_SUFFIXES QGLViewer QGLViewer/release)

find_library(QGLVIEWER_LIBRARY_DEBUG
    NAMES dqglviewer dQGLViewer QGLViewerd QGLViewerd2 dqglviewer${QT_SUFFIX} dQGLViewer${QT_SUFFIX}
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        /sw/lib
        C:/libs/QGLViewer/lib
        ${QGLVIEWER_ROOT}
        ${QGLVIEWER_ROOT_DIR}
        ENV QGLVIEWER_ROOT
        ENV QGLVIEWER_ROOT_DIR
        ENV LD_LIBRARY_PATH
        ENV LIBRARY_PATH
    PATH_SUFFIXES QGLViewer QGLViewer/debug
)

if(QGLVIEWER_LIBRARY_RELEASE)
    if(QGLVIEWER_LIBRARY_DEBUG)
        set(QGLVIEWER_LIBRARY optimized ${QGLVIEWER_LIBRARY_RELEASE} debug ${QGLVIEWER_LIBRARY_DEBUG})
    else()
        set(QGLVIEWER_LIBRARY ${QGLVIEWER_LIBRARY_RELEASE})
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QGLViewer DEFAULT_MSG QGLVIEWER_INCLUDE_DIR QGLVIEWER_LIBRARY)
