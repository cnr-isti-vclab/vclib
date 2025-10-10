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

#include "default_viewer.h"
#include "material_tri_mesh.h"

#include <vclib/io.h>

int main(int argc, char** argv)
{
    using namespace vcl;

    // Example meshes with materials
    std::string e1 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/OrientationTest/OrientationTest.gltf";
    std::string e2 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/MetalRoughSpheresNoTextures/MetalRoughSpheresNoTextures.gltf";
    std::string e3 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/Sphere/metallic_roughness_grid.glb";
    std::string e4 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/EmissiveStrengthTest/EmissiveStrengthTest.gltf";

    std::vector<MaterialTriMesh> meshes = vcl::loadMeshes<MaterialTriMesh>(e4);

    for(auto& mesh : meshes) {
        for (auto& mat : mesh.materials()) {
            std::cout << "baseColorFactor: " << mat.baseColor() << std::endl;
            std::cout << "metallicFactor: " << mat.metallic() << std::endl;
            std::cout << "roughnessFactor: " << mat.roughness() << std::endl;
            std::cout << "emissiveFactor: " << mat.emissiveColor() << std::endl << std::endl;
        }
    }

    return showMeshesOnDefaultViewer(argc, argv, std::move(meshes));
}
