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
    std::string e8 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/MetalRoughSpheres/MetalRoughSpheres.gltf";
    std::string e9 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/TextureSettingsTest/TextureSettingsTest.gltf";
    std::string e10 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/CompareEmissiveStrength/CompareEmissiveStrength.gltf";
    std::string e11 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/CompareNormal/CompareNormal.gltf";
    std::string e12 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/NormalTangentTest/NormalTangentTest.gltf";
    std::string e13 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/CompareAlphaCoverage/CompareAlphaCoverage.gltf";
    std::string e14 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/AlphaBlendModeTest/AlphaBlendModeTest.gltf";
    std::string e15 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/NormalTangentMirrorTest/NormalTangentMirrorTest.gltf";
    std::string e16 = VCLIB_EXAMPLE_MESHES_PATH "/gltf/Sponza/Sponza.gltf";


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

    std::vector<vcl::TriMesh> meshes = vcl::loadMeshes<vcl::TriMesh>(e7);

    auto printTextureInfo = [&](const Material& mat, Material::TextureType type) {
        const vcl::Texture& texture = mat.texture(type);
        std::string typeName = "baseColor";
        if(type == Material::TextureType::METALLIC_ROUGHNESS) {
            typeName = "metallicRoughness";
        } else if(type == Material::TextureType::NORMAL) {
            typeName = "normal";
        } else if(type == Material::TextureType::OCCLUSION) {
            typeName = "occlusion";
        } else if(type == Material::TextureType::EMISSIVE) {
            typeName = "emissive";
        }

        std::cout << "  " << typeName << "Texture: ";
        if (!texture.isNull()) {
            std::cout << texture.path() << std::endl;
            std::cout << "    image size: "
                      << texture.image().width() << " x "
                      << texture.image().height()
                      << std::endl;
            std::cout << "    minFilter: " << int(texture.minFilter()) << std::endl;
            std::cout << "    magFilter: " << int(texture.magFilter()) << std::endl;
            std::cout << "    wrapU: " << int(texture.wrapU()) << std::endl;
            std::cout << "    wrapV: " << int(texture.wrapV()) << std::endl;
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
            std::cout << "  normalScale: " << mat.normalScale() << std::endl;
            std::cout << "  occlusionStrength: " << mat.occlusionStrength() << std::endl;
            printTextureInfo(mat, Material::TextureType::BASE_COLOR);
            printTextureInfo(mat, Material::TextureType::METALLIC_ROUGHNESS);
            printTextureInfo(mat, Material::TextureType::NORMAL);
            printTextureInfo(mat, Material::TextureType::OCCLUSION);
            printTextureInfo(mat, Material::TextureType::EMISSIVE);
            std::cout << "  ------------------------" << std::endl;
        }
        std::cout << "------------------------" << std::endl;
    }

    return showMeshesOnDefaultViewer(argc, argv, std::move(meshes), true);
}
