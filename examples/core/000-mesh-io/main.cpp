/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/io.h>
#include <vclib/meshes.h>

// This example shows how to load and save meshes using the VCLib IO module.
// VCLib supports loading and saving the following mesh formats:
// - OBJ
// - PLY
// - OFF
// - STL

int main()
{
    /****** Load ******/

    // To load a mesh you can both declare the mesh object and pass it to the
    // load function, or you can create the mesh object using the load function.

    // declare and load:
    vcl::TriMesh mesh0;
    vcl::load(mesh0, VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    // create and load:
    auto mesh1 =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    // The loaded mesh is adapted to the type of mesh you want to use (e.g.
    // loading a file that contains a polygonal mesh in a TriMesh object).

    // To know what data was loaded, you can pass a MeshInfo object to the
    // load function.

    vcl::MeshInfo info;
    vcl::load(mesh0, VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj", info);

    std::cout << "Mesh has per face color: " << info.hasFaceColors()
              << std::endl;

    // You can control settings for loading a mesh using the LoadSettings object
    // It allows to choose whether to enable optional components (see example
    // TODO) and whether to load texture images or not.
    vcl::LoadSettings loadSettings;
    loadSettings.loadTextureImages = true;
    vcl::load(
        mesh1, VCLIB_EXAMPLE_MESHES_PATH "/bunny_textured.ply", loadSettings);

    /****** Save ******/

    // To save a mesh you can use the save function. The function will
    // automatically detect the file format based on the file extension.
    vcl::save(mesh0, VCLIB_RESULTS_PATH "/000_mesh-io_bunny.obj");

    // You can also specify what kind of data you want to save in the file,
    // using a MeshInfo object in the SaveSettings parameter
    vcl::MeshInfo saveInfo;
    saveInfo.setVertexCoords();
    saveInfo.setFaces(false); // do not save faces - only point cloud

    vcl::SaveSettings saveSettings;
    saveSettings.info = saveInfo;
    vcl::save(
        mesh0, VCLIB_RESULTS_PATH "/000_mesh-io_bunny-cloud.obj", saveSettings);

    // You can also specify other save settings trough the SaveSettings object.
    saveInfo.setFaceVRefs(true);
    saveSettings.info   = saveInfo;
    saveSettings.binary = false; // save in ASCII format
    vcl::save(
        mesh0, VCLIB_RESULTS_PATH "/000_mesh-io_bunny-ascii.ply", saveSettings);

    return 0;
}
