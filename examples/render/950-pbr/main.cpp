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
    std::string e5 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/BoxVertexColors/BoxVertexColors.gltf";
    std::string e6 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/Sphere/opaque_grid.glb";
    std::string e7 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/Sphere/mask_grid.glb";
    std::string e8 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/Sphere/blend_grid.glb";
    std::string e9 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/EmissiveTest/emissive_grid.glb";
    std::string e10 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/VertexColorTest/vertex_color_grid.glb";
    std::string e11 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/DoubleSidedTest/OrientationTest.gltf";
    std::string e12 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/AlphaModeTest/opaque_grid.glb";
    std::string e13 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/AlphaModeTest/mask_grid.glb";
    std::string e14 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/AlphaModeTest/blend_grid.glb";    

    std::vector<MaterialTriMesh> meshes = vcl::loadMeshes<MaterialTriMesh>(e14);

    for(auto& mesh : meshes) {
        for (auto& mat : mesh.materials()) {
            std::cout << "baseColorFactor: " << mat.baseColor() << std::endl;
            std::cout << "metallicFactor: " << mat.metallic() << std::endl;
            std::cout << "roughnessFactor: " << mat.roughness() << std::endl;
            std::cout << "emissiveFactor: " << mat.emissiveColor() << std::endl;
            std::cout << "doubleSided: " << mat.doubleSided() << std::endl;
            std::cout << "alphaMode: " << int(mat.alphaMode()) << std::endl;
            std::cout << "alphaCutoff: " << mat.alphaCutoff() << std::endl;
            std::cout << "------------------------" << std::endl;
        }
    }

    return showMeshesOnDefaultViewer(argc, argv, std::move(meshes));
}
