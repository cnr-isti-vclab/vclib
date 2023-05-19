#*****************************************************************************
#* VCLib                                                                     *
#* Visual Computing Library                                                  *
#*                                                                           *
#* Copyright(C) 2021-2022                                                    *
#* Alessandro Muntoni                                                        *
#* VCLab - ISTI - Italian National Research Council                          *
#*                                                                           *
#* All rights reserved.                                                      *
#****************************************************************************/

project(vclib-static-asserts)

set(HEADERS
	meshes.h
	loggers.h
	space.h
	meshes/edgemesh.h
	meshes/mesh_views.h
	meshes/pointcloud.h
	meshes/polymesh.h
	meshes/trimesh.h
)

set(SOURCES
	main.cpp
)

add_executable(vclib-static-asserts ${HEADERS} ${SOURCES})

target_link_libraries(vclib-static-asserts PUBLIC vclib)
