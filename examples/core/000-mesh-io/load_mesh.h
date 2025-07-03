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

#ifndef LOAD_MESH_H
#define LOAD_MESH_H

#include <vclib/io.h>
#include <vclib/meshes.h>

auto loadMeshes()
{
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

    std::cout << "Mesh has per face color: " << info.hasPerFaceColor()
              << std::endl;

    // You can control settings for loading a mesh using the LoadSettings object
    // It allows to choose whether to enable optional components (see example
    // TODO) and whether to load texture images or not.
    vcl::LoadSettings loadSettings;
    loadSettings.loadTextureImages = true;
    vcl::load(
        mesh1, VCLIB_EXAMPLE_MESHES_PATH "/bunny_textured.ply", loadSettings);

    return std::make_tuple(mesh0, mesh1);
}

#endif // LOAD_MESH_H
