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

VCLIB_EXAMPLE_MESHES_PATH = "../../assets/example_meshes"
VCLIB_RESULTS_PATH = "../../assets/results/python"

# This example shows how to load and save meshes using the VCLib IO module.
# VCLib supports loading and saving the following mesh formats:
# - OBJ
# - PLY
# - OFF
# - STL

# /****** Load ******/

# To load a mesh you can both declare the mesh object and pass it to the
# load function, or you can create the mesh object using the load function.

# declare and load:
mesh0 = vcl.TriMesh()
vcl.load(mesh0, VCLIB_EXAMPLE_MESHES_PATH + "/bunny.obj")

# create and load:
mesh1 = vcl.load_tri_mesh(VCLIB_EXAMPLE_MESHES_PATH + "/bunny.obj")

# The loaded mesh is adapted to the type of mesh you want to use (e.g.
# loading a file that contains a polygonal mesh in a TriMesh object).

# To know what data was loaded, you can pass a MeshInfo object to the
# load function.

info = vcl.MeshInfo()
vcl.load(mesh0, VCLIB_EXAMPLE_MESHES_PATH + "/bunny.obj", info)
print("Mesh has per face color: " + str(info.has_per_element_component(info.FACE, info.COLOR)))

# You can control settings fro loading a mesh using the load function arguments.
# They allow to chooese whether to enable optional components (see example
# TODO) and whether to load texture images or not.
vcl.load(mesh1, VCLIB_EXAMPLE_MESHES_PATH + "/bunny_textured.ply", load_texture_images = True)

# /****** Save ******/

# To save a mesh you can use the save function. The function will
# automatically detect the file format based on the file extension.
vcl.save(mesh0, VCLIB_RESULTS_PATH + "/000_mesh-io_bunny.obj")
