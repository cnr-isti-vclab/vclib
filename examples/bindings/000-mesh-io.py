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

# This example assumes that vclib is installed and available in the PYTHONPATH.

import vclib as vcl
import os

current_file_path = os.path.abspath(__file__)
current_file_path = os.path.dirname(current_file_path)

VCLIB_EXAMPLE_MESHES_PATH = current_file_path + "/../../assets/example_meshes"
VCLIB_RESULTS_PATH = current_file_path + "/../../assets/results/python"

# This example shows how to load and save meshes using the VCLib IO module.
# VCLib supports loading and saving the following mesh formats:
# - OBJ
# - PLY
# - OFF
# - STL

# /****** Load ******/

# To load a mesh you can both declare the mesh object and pass it to the
# load_mesh function, or you can create the mesh object using the 
# load_[mesh_type] function.

# declare and load:
mesh0 = vcl.TriMesh()
vcl.load_mesh(mesh0, VCLIB_EXAMPLE_MESHES_PATH + "/bunny.obj")

# create and load:
mesh1 = vcl.load_tri_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/bunny_textured.ply")

# The loaded mesh is adapted to the type of mesh you want to use (e.g.
# loading a file that contains a polygonal mesh in a TriMesh object).

# To know what data was loaded, you can pass a MeshInfo object to the
# load function.

info = vcl.MeshInfo()
vcl.load_mesh(mesh0, VCLIB_EXAMPLE_MESHES_PATH + "/bunny.obj", info)
print("Mesh has per face color: " + str(info.has_per_face_color()))

# You can control settings fro loading a mesh using the load function arguments.
# They allow to choose whether to enable optional components (see example
# TODO) and whether to load texture images or not.
vcl.load_mesh(mesh1, VCLIB_EXAMPLE_MESHES_PATH + "/bunny_textured.ply", load_texture_images = True)

# /****** Save ******/

# To save a mesh you can use the save function. The function will
# automatically detect the file format based on the file extension.
vcl.save_mesh(mesh0, VCLIB_RESULTS_PATH + "/000_mesh-io_bunny.obj")

# You can also specify what kind of data you want to save in the file,
# using a MeshInfo object

save_info = vcl.MeshInfo()
save_info.set_per_vertex_position()
save_info.set_faces(False) # do not save faces - only point cloud

vcl.save_mesh(mesh0, VCLIB_RESULTS_PATH + "/000_mesh-io_bunny-cloud.obj", info = save_info)

# You can also specify other settings for saving
save_info.set_faces(True)
save_info.set_per_face_vertex_references(True)

# Save in ply ASCII format
vcl.save_mesh(mesh0, VCLIB_RESULTS_PATH + "/000_mesh-io_bunny-ascii.ply", info = save_info, binary = False)
