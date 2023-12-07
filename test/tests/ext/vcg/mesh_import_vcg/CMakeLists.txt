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

project(vclib-ext-mesh-import-from-vcg-test)

get_target_property(VCG_INCLUDE_DIRS vclib-external-vcg VCG_INCLUDE_DIRS)

# vcg generates a lot of warnings
set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

set(SOURCES
    mesh.h
    main.cpp
    ${VCG_INCLUDE_DIRS}/wrap/ply/plylib.cpp)

add_executable(vclib-ext-mesh-import-from-vcg-test ${SOURCES})

target_link_libraries(vclib-ext-mesh-import-from-vcg-test PUBLIC vclib vclib-test-common)

enable_testing()
add_test(NAME mesh-import-from-vcg COMMAND vclib-ext-mesh-import-from-vcg-test)
