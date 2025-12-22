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

    // Examples to test

    enum GLTFExamples {
        METAL_ROUGH_SPHERES_NO_TEXTURES = 0,
        METAL_ROUGH_SPHERES,
        NORMAL_TENGENT_MIRROR_TEST,
        COMPARE_AMBIENT_OCCLUSION,
        DAMAGED_HELMET,
        COUNT
    };

    static const std::string GLTFExampleFilenames[COUNT] = {
        "/gltf/MetalRoughSpheresNoTextures/MetalRoughSpheresNoTextures.gltf",
        "/gltf/MetalRoughSpheres/MetalRoughSpheres.gltf",
        "/gltf/NormalTangentMirrorTest/NormalTangentMirrorTest.gltf",
        "/gltf/CompareAmbientOcclusion/CompareAmbientOcclusion.gltf",
        "/gltf/DamagedHelmet/DamagedHelmet.gltf"};

    uint selectedExample = METAL_ROUGH_SPHERES;

    std::vector<vcl::TriMesh> meshes = vcl::loadMeshes<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH + GLTFExampleFilenames[selectedExample]);

    auto printTextureInfo = [&](const Material&       mat,
                                Material::TextureType type) {
        using enum Material::TextureType;
        const vcl::TextureDescriptor& texture  = mat.textureDescriptor(type);
        std::string                   typeName = "baseColor";
        if (type == METALLIC_ROUGHNESS) {
            typeName = "metallicRoughness";
        }
        else if (type == NORMAL) {
            typeName = "normal";
        }
        else if (type == OCCLUSION) {
            typeName = "occlusion";
        }
        else if (type == EMISSIVE) {
            typeName = "emissive";
        }

        std::cout << "  " << typeName << "Texture: ";
        if (!texture.isNull()) {
            std::cout << texture.path() << std::endl;
            std::cout << "    minFilter: " << int(texture.minFilter())
                      << std::endl;
            std::cout << "    magFilter: " << int(texture.magFilter())
                      << std::endl;
            std::cout << "    wrapU: " << int(texture.wrapU()) << std::endl;
            std::cout << "    wrapV: " << int(texture.wrapV()) << std::endl;
        }
        else {
            std::cout << "null" << std::endl;
        }
    };

    for (const auto& mesh : meshes) {
        using enum Material::TextureType;
        std::cout << "Mesh: " << mesh.name() << std::endl;
        for (const auto& mat : mesh.materials()) {
            std::cout << " Material: " << mat.name() << std::endl;
            std::cout << "  baseColorFactor: " << mat.baseColor() << std::endl;
            std::cout << "  metallicFactor: " << mat.metallic() << std::endl;
            std::cout << "  roughnessFactor: " << mat.roughness() << std::endl;
            std::cout << "  emissiveFactor: " << mat.emissiveColor()
                      << std::endl;
            std::cout << "  doubleSided: " << mat.doubleSided() << std::endl;
            std::cout << "  alphaMode: " << int(mat.alphaMode()) << std::endl;
            std::cout << "  alphaCutoff: " << mat.alphaCutoff() << std::endl;
            std::cout << "  normalScale: " << mat.normalScale() << std::endl;
            std::cout << "  occlusionStrength: " << mat.occlusionStrength()
                      << std::endl;
            printTextureInfo(mat, BASE_COLOR);
            printTextureInfo(mat, METALLIC_ROUGHNESS);
            printTextureInfo(mat, NORMAL);
            printTextureInfo(mat, OCCLUSION);
            printTextureInfo(mat, EMISSIVE);
            std::cout << "  ------------------------" << std::endl;
        }
        std::cout << "------------------------" << std::endl;
    }

    return showMeshesOnDefaultViewer(argc, argv, std::move(meshes), true, VCLIB_ASSETS_PATH "/uffizi.hdr");
}
