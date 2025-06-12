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

#ifndef VCL_IO_MESH_GLTF_LOAD_MESH_H
#define VCL_IO_MESH_GLTF_LOAD_MESH_H

#include <vclib/algorithms/mesh/update.h>
#include <vclib/concepts/mesh.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/misc/logger.h>
#include <vclib/space/core/texture.h>
#include <vclib/space/complex/mesh_info.h>

#include <tiny_gltf.h>

#include <regex>

namespace vcl::detail {

enum class GltfAttrType {POSITION, NORMAL, COLOR_0, TEXCOORD_0, INDICES};

void checkGltfPrimitiveMaterial(
    const tinygltf::Model& model,
    const tinygltf::Primitive& p,
    int& textureImg,
    bool& hasColor,
    vcl::Color& color)
{
    if (p.material >= 0) { //if the primitive has a material
        const tinygltf::Material& mat = model.materials[p.material];
        auto it = mat.values.find("baseColorTexture");
        if (it != mat.values.end()){ //the material is a texture
            auto it2 = it->second.json_double_value.find("index");
            if (it2 != it->second.json_double_value.end()){
                textureImg = it2->second; //get the id of the texture
            }
        }
        it = mat.values.find("baseColorFactor");
        if (it != mat.values.end()) { //vertex base color, the same for a primitive
            hasColor = true;
            const std::vector<double>& vc = it->second.number_array;
            for (unsigned int i = 0; i < 4; i++)
                color[i] = vc[i] * 255.0;
        }
    }
}

/**
 * @brief loads the attribute attr from the primitive p contained in the
 * gltf model. If the attribute is vertex position, sets also vertex pointers
 * vector ivp. For all the other parameters, ivp is a const input.
 *
 * If the primitive does not contain the attribute attr, nothing is done.
 * However, if the attribute is POSITION, then a MLException will be thrown.
 *
 *
 * @param m
 * @param model
 * @param p
 * @param attr
 * @param textID: id of the texture in case of the attr is TEXCOORD_0
 * @return true if the attribute has been loaded
 */
template<
    MeshConcept     MeshType,
    LoggerConcept   LogType>
bool loadAttribute(
    MeshType& m,
    uint startingVertex,
    const tinygltf::Model& model,
    const tinygltf::Primitive& p,
    GltfAttrType attr,
    int textID)
{
    return false;
}

/**
 * @brief loads the given primitive into the mesh
 * @param m
 * @param model
 * @param p
 */
template<
    MeshConcept     MeshType,
    Matrix44Concept MatrixType,
    LoggerConcept   LogType>
void loadGltfMeshPrimitive(
    MeshType&                  m,
    MeshInfo&                  info,
    const tinygltf::Model&     model,
    const tinygltf::Primitive& p,
    const MatrixType&          transf,
    LogType&                   log,
    const LoadSettings&        settings)
{
    int textureImg = -1; //id of the texture associated to the material
    bool vTex = false; // used if a material has a texture
    bool vCol = false; // used if a material has a base color for all the primitive
    vcl::Color col; //the base color, to be set to all the vertices
    checkGltfPrimitiveMaterial(model, p, textureImg, vCol, col);

    if constexpr (HasTexturePaths<MeshType>) {
        if (textureImg != -1) { //if we found a texture
            vTex = true;
            const tinygltf::Image& img = model.images[model.textures[textureImg].source];
            // add the path of the texture to the mesh
            std::string uri = img.uri;
            uri = std::regex_replace(uri, std::regex("\\%20"), " ");

            bool textureAdded = false;
            if constexpr (HasTextureImages<MeshType>) {
                if (img.image.size() > 0) {
                    if (img.bits == 8 || img.component == 4) {
                        if (uri.empty()) {
                            uri = "texture_" + std::to_string(textureImg);
                        }
                        vcl::Texture txt(Image(img.image.data(), img.width, img.height), uri);
                        m.pushTexture(txt);
                        textureAdded = true;
                    }
                }
            }
            if (!textureAdded) {
                // if the image is not valid, just add the path
                m.pushTexturePath(uri);
            }
        }
    }

    // load vertex position attribute
    // loadAttribute(m, model, p, GltfAttrType::POSITION);

    if (vCol) {
        if constexpr (HasPerVertexColor<MeshType>) {
            if (settings.enableOptionalComponents) {
                enableIfPerVertexColorOptional(m);
            }
            if (isPerVertexColorAvailable(m)) {
                for (auto& v : m.vertices())
                    v.color() = col;
            }
        }
    }

    if (HasTransformMatrix<MeshType>) {
        m.transformMatrix() = transf;
    }
    else {
        // if the mesh does not have a transform matrix, apply the transformation
        // matrix to the vertices
        vcl::applyTransformMatrix(m, transf);
    }
}

/**
 * @brief loads a mesh from gltf file.
 * It merges all the primitives in the loaded mesh.
 *
 * @param m: the mesh that will contain the loaded mesh
 * @param tm: tinygltf structure of the mesh to load
 * @param model: tinygltf file
 */
template<
    MeshConcept     MeshType,
    Matrix44Concept MatrixType,
    LoggerConcept   LogType>
void gltfLoadMesh(
    MeshType&              m,
    MeshInfo&              info,
    const tinygltf::Mesh&  tm,
    const tinygltf::Model& model,
    const MatrixType&      transf,
    LogType&               log,
    const LoadSettings& settings)
{
    if constexpr (HasName<MeshType>) {
        if (!tm.name.empty()) {
            m.name() = tm.name;
        }
    }

    log.startProgress("Reading primitives", tm.primitives.size());

    //for each primitive, load it into the mesh
    for (uint i = 0; const tinygltf::Primitive& p : tm.primitives){
        loadGltfMeshPrimitive(
            m,
            info,
            model,
            p,
            transf,
            log,
            settings);
        log.progress(i);
    }

    log.endProgress();

    log.log(
        "Loaded mesh '" + tm.name + "' with " +
        std::to_string(tm.primitives.size()) + " primitives.",
        LogType::LogLevel::MESSAGE_LOG);
}

} // namespace detail

#endif // VCL_IO_MESH_GLTF_LOAD_MESH_H
