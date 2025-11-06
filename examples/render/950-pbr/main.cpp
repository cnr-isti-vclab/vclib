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
    std::string e3 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/EmissiveStrengthTest/EmissiveStrengthTest.gltf";
    std::string e4 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/BoxVertexColors/BoxVertexColors.gltf";
    std::string e5 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/CesiumMilkTruck/CesiumMilkTruck.gltf";
    std::string e6 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/BoxTextured/BoxTextured.gltf";
    std::string e7 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/DamagedHelmet/DamagedHelmet.gltf";

    // Examples to test

    // Metallic-Roughness
    std::string t1 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/MetalRoughSpheresNoTextures/MetalRoughSpheresNoTextures.gltf";

    // Emissive - to test also with lights off comment the #define USE_LIGHTS in vclib/render/shaders/vclib/bgfx/shaders_common.sh
    std::string t2 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/EmissiveTest/emissive_grid.glb";

    // Vertex Color - vertex color has the green channel growing on the y axis, 
    // base color has the blue channel growing on the x axis
    std::string t3 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/VertexColorTest/vertex_color_grid.glb";

    // Double Sided - modified version of OrientationTest with double sided enabled on the cube material, not the arrows'
    std::string t4 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/DoubleSidedTest/OrientationTest.gltf";

    // Alpha Mode - opaque
    std::string t5 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/AlphaModeTest/opaque_grid.glb";

    // Alpha Mode - mask, the alpha channel in the base color grows on the x axis
    // while it's random between 0.5 and 1 in the vertex color
    // the cutoff is set to 0.3 in the material hence the first 3 of the 10 columns should be basically invisible
    // while the others are progressively more visible because of the increasing alpha in the base color
    std::string t6 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/AlphaModeTest/mask_grid.glb";

    // Alpha Mode - blend, generated with the same criteria as t6
    // the only change is the alpha mode set to BLEND
    std::string t7 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/AlphaModeTest/blend_grid.glb"; 

    // Similar to the previous one, but with varying metallic and roughness
    std::string t8 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/AlphaModeTest/metal_rough_blend_grid.glb"; 

    // Multiple materials per mesh - the upper part is a double sided red metal,
    // the lower part a semitransparent blue matte
    std::string t9 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/MultipleMaterialsTest/MultipleMaterialsTest.gltf";

    std::vector<MaterialTriMesh> meshes = vcl::loadMeshes<MaterialTriMesh>(e7);

    auto printTextureInfo = [&](const Material& mat, Material::Textures type) {
        const vcl::Texture& texture = mat.texture(type);
        std::string typeName = "baseColor";
        if(type == Material::Textures::METALLIC_ROUGHNESS) {
            typeName = "metallicRoughness";
        } else if(type == Material::Textures::NORMAL) {
            typeName = "normal";
        } else if(type == Material::Textures::OCCLUSION) {
            typeName = "occlusion";
        } else if(type == Material::Textures::EMISSIVE) {
            typeName = "emissive";
        }

        std::cout << "  " << typeName << "Texture: ";
        if (!texture.isNull()) {
            std::cout << texture.path() << std::endl;
            std::cout << "    image size: "
                      << texture.image().width() << " x "
                      << texture.image().height()
                      << std::endl;
        }
        else {
            std::cout << "null" << std::endl;
        }
    };

    for(const auto& mesh : meshes) {
        std::cout << "Mesh: " << mesh.name() << std::endl;
        for(const auto& mat : mesh.materials()) {
            std::cout << " Material: " << mat.name() << std::endl;
            std::cout << "  baseColorFactor: " << mat.baseColor() << std::endl;
            std::cout << "  metallicFactor: " << mat.metallic() << std::endl;
            std::cout << "  roughnessFactor: " << mat.roughness() << std::endl;
            std::cout << "  emissiveFactor: " << mat.emissiveColor() << std::endl;
            std::cout << "  doubleSided: " << mat.doubleSided() << std::endl;
            std::cout << "  alphaMode: " << int(mat.alphaMode()) << std::endl;
            std::cout << "  alphaCutoff: " << mat.alphaCutoff() << std::endl;
            printTextureInfo(mat, Material::Textures::BASE_COLOR);
            printTextureInfo(mat, Material::Textures::METALLIC_ROUGHNESS);
            printTextureInfo(mat, Material::Textures::NORMAL);
            printTextureInfo(mat, Material::Textures::OCCLUSION);
            printTextureInfo(mat, Material::Textures::EMISSIVE);
            std::cout << "  ------------------------" << std::endl;
        }
        std::cout << "------------------------" << std::endl;
    }

    return showMeshesOnDefaultViewer(argc, argv, std::move(meshes));
}
