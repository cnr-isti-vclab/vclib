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

cmake_minimum_required(VERSION 3.13)
project(vclib-render-external)

include(FetchContent)

# OpenGL
option(VCLIB_ALLOW_SYSTEM_OPENGL "Allow use of system-provided OpenGL" ON)

# Qt
option(VCLIB_ALLOW_SYSTEM_QT "Allow use of system-provided Qt" ON)

# QGLViewer
option(VCLIB_ALLOW_SYSTEM_QGLVIEWER "Allow use of system-provided QGLViewer" ON)

set(VCLIB_RENDER_EXTERNAL_LIBRARIES "")

# === OPTIONAL === #

### OpenGL
include(opengl.cmake)

### Qt
include(qt.cmake)

### QGLViewer
include(qglviewer.cmake)

set(VCLIB_RENDER_EXTERNAL_LIBRARIES ${VCLIB_RENDER_EXTERNAL_LIBRARIES} PARENT_SCOPE)
