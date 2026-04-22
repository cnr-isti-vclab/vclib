/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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
#include <vclib/mesh.h>
#include <vclib/meshes.h>
#include <vclib/space.h>

#include <iostream>

int main()
{
    std::cout << "Loading mesh..." << std::endl;

    vcl::LoadSettings loadSettings;
    vcl::MeshInfo info;
    loadSettings.loadTextureImages = true;
    auto mesh = vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/greek_helmet.obj", info, loadSettings);
    vcl::updateBoundingBox(mesh);
    vcl::updatePerVertexAndFaceNormals(mesh);

    // test normals normalization
    //for (auto& norm : mesh.vertices() | vcl::views::normals)
    //    norm *= 2.0;

    std::cout << "Converting mesh..." << std::endl;

    vcl::SaveSettings saveSettings;
    saveSettings.binary = false;
    vcl::saveGltf(mesh, VCLIB_EXAMPLE_MESHES_PATH "/gltf/greek_helmet_export_gltf.gltf", saveSettings);

    return 0;
}
