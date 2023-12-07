#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2023                                                    *
#* Visual Computing Lab                                                      *
#* ISTI - Italian National Research Council                                  *
#*                                                                           *
#* All rights reserved.                                                      *
#*                                                                           *
#* This program is free software; you can redistribute it and/or modify      *
#* it under the terms of the GNU General Public License as published by      *
#* the Free Software Foundation; either version 3 of the License, or         *
#* (at your option) any later version.                                       *
#*                                                                           *
#* This program is distributed in the hope that it will be useful,           *
#* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
#* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
#* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
#* for more details.                                                         *
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
