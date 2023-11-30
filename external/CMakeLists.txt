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

cmake_minimum_required(VERSION 3.24)
project(vclib-render-external)

include(FetchContent)

# external libraries may have warnings
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

# Qt
option(VCLIB_ALLOW_SYSTEM_QT "Allow use of system-provided Qt" ON)

# bgfx
option(VCLIB_ALLOW_BUNDLED_BGFX "Allow use of bundled bgfx source" ON)

# QGLViewer
option(VCLIB_ALLOW_SYSTEM_QGLVIEWER "Allow use of system-provided QGLViewer" ON)

set(VCLIB_RENDER_EXTERNAL_LIBRARIES "")

# === OPTIONAL === #

### OpenGL
include(opengl.cmake)

### Qt
include(qt.cmake)

### bgfx
include(bgfx.cmake)

### QGLViewer
include(qglviewer.cmake)

set(VCLIB_RENDER_EXTERNAL_LIBRARIES
    ${VCLIB_RENDER_EXTERNAL_LIBRARIES} PARENT_SCOPE)
