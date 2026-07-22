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

#ifndef VCL_IO_MESH_GLTF_SAVE_H
#define VCL_IO_MESH_GLTF_SAVE_H

#include <tiny_gltf.h>
#include <vclib/algorithms/mesh/import_export/export_buffer.h>
#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>

namespace vcl {

namespace detail {

inline static const std::string VCL_GLTF_ASSET_VERSION = "2.0";
inline static const std::string VCL_GLTF_GENERATOR_NAME =
    "vclib-tinygltf-exporter";

inline std::pair<uint, tinygltf::Buffer&> addGltfBuffer(
    tinygltf::Model& model,
    size_t           size)
{
    model.buffers.emplace_back();
    tinygltf::Buffer& buf   = model.buffers.back();
    uint              index = model.buffers.size() - 1;
    buf.data.resize(size);

    return {index, buf};
}

inline std::pair<uint, tinygltf::BufferView&> addGltfBufferView(
    tinygltf::Model&                   model,
    std::pair<uint, tinygltf::Buffer&> buffer,
    int                                bufferViewTarget = TINYGLTF_TARGET_ARRAY_BUFFER,
    uint                               byteOffset = 0,
    std::optional<uint>                byteLength = std::nullopt)
{
    model.bufferViews.emplace_back();
    tinygltf::BufferView& bufView = model.bufferViews.back();
    uint                  index   = model.bufferViews.size() - 1;
    bufView.buffer                = buffer.first;
    bufView.byteOffset            = byteOffset;
    bufView.byteLength            = byteLength.has_value() ? byteLength.value() : buffer.second.data.size();
    bufView.target                = bufferViewTarget;

    return {index, bufView};
}

inline std::pair<uint, tinygltf::Accessor&> addGltfAccessor(
    tinygltf::Model&                       model,
    std::pair<uint, tinygltf::BufferView&> bufferView,
    int                                    componentType,
    int                                    type,
    bool                                   normalized = false)
{
    model.accessors.emplace_back();
    tinygltf::Accessor& accessor = model.accessors.back();
    uint                index    = model.accessors.size() - 1;
    accessor.bufferView          = bufferView.first;
    accessor.componentType       = componentType;
    accessor.type                = type;
    // count = bytes / (comp_bytes * num_comp_in_type)
    accessor.count = bufferView.second.byteLength /
                     (tinygltf::GetComponentSizeInBytes(componentType) *
                      tinygltf::GetNumComponentsInType(type));
    accessor.normalized = normalized;

    return {index, accessor};
}

inline std::pair<uint, tinygltf::Primitive&> addGltfPrimitive(
    tinygltf::Mesh& mesh,
    int             posAccessorIndex,
    int             colAccessorIndex,
    int             normAccessorIndex,
    int             texCoordAccessorIndex,
    int             tangentAccessorIndex,
    int             mode)
{
    mesh.primitives.emplace_back();
    tinygltf::Primitive& primitive   = mesh.primitives.back();
    uint                 index       = mesh.primitives.size() - 1;
    primitive.mode                   = mode;
    primitive.attributes["POSITION"] = posAccessorIndex;

    if (colAccessorIndex >= 0)
        primitive.attributes["COLOR_0"] = colAccessorIndex;

    if (normAccessorIndex >= 0)
        primitive.attributes["NORMAL"] = normAccessorIndex;

    if (tangentAccessorIndex >= 0)
        primitive.attributes["TANGENT"] = tangentAccessorIndex;

    // if multiple textures per render pass become supported
    // multiple TEXCOORD must be set (TEXCOORD_1, etc...)
    if (texCoordAccessorIndex >= 0)
        primitive.attributes["TEXCOORD_0"] = texCoordAccessorIndex;

    return {index, primitive};
}

inline uint addGltfSampler(
    tinygltf::Model&         model,
    const TextureDescriptor& textureDescriptor)
{
    model.samplers.emplace_back();
    tinygltf::Sampler& sampler = model.samplers.back();
    uint               index    = model.samplers.size() - 1;
    sampler.minFilter = static_cast<int>(textureDescriptor.minFilter());
    sampler.magFilter = static_cast<int>(textureDescriptor.magFilter());
    sampler.wrapS     = static_cast<int>(textureDescriptor.wrapU());
    sampler.wrapT     = static_cast<int>(textureDescriptor.wrapV());

    return index;
}

inline uint addGltfImage(
    tinygltf::Model& model,
    const Image&     image,
    std::string      path,
    bool             saveTextureImages)
{
    model.images.emplace_back();
    tinygltf::Image& tImage = model.images.back();
    uint             index  = model.images.size() - 1;
    tImage.width      = image.width();
    tImage.height     = image.height();
    tImage.component  = 4; // channels
    tImage.bits       = 8; // bits per channel
    tImage.pixel_type = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    tImage.uri        = path;

    if (saveTextureImages)
        tImage.image.assign(image.data(), image.data() + image.sizeInBytes());

    return index;
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
inline uint addGltfTexture(
    tinygltf::Model&                                        model,
    const MeshType&                                         mesh,
    const TextureDescriptor&                                textureDescriptor,
    std::unordered_map<std::string, std::pair<uint, uint>>& addedImages,
    bool                                                    saveTextureImages,
    LogType&                                                log               = nullLogger)
{
    if (textureDescriptor.isNull()) {
        log.log("Cannot save empty texture: " + textureDescriptor.path(), LogType::WARNING_LOG);

        return std::numeric_limits<uint>::max();
    }

    if (saveTextureImages && mesh.textureImage(textureDescriptor.path()).isNull()) {
        log.log("Cannot save empty image: " + textureDescriptor.path(), LogType::WARNING_LOG);

        return std::numeric_limits<uint>::max();
    }

    model.textures.emplace_back();
    tinygltf::Texture& texture = model.textures.back();
    uint               index   = model.textures.size() - 1;
    uint imageId = -1, samplerId = -1;

    if (!addedImages.contains(textureDescriptor.path())) {
        imageId = addGltfImage(model, mesh.textureImage(textureDescriptor.path()), textureDescriptor.path(), saveTextureImages);
        samplerId = addGltfSampler(model, textureDescriptor);
        addedImages[textureDescriptor.path()] = { imageId, samplerId };
    }

    texture.source  = addedImages[textureDescriptor.path()].first;
    texture.sampler = addedImages[textureDescriptor.path()].second;

    return index;
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
inline std::pair<uint, tinygltf::Material&> addGltfMaterial(
    tinygltf::Model&                                        model,
    const MeshType&                                         mesh,
    const Material&                                         material,
    std::unordered_map<std::string, uint>&                  addedTextures,
    std::unordered_map<std::string, std::pair<uint, uint>>& addedImages,
    bool                                                    saveTextureImages,
    LogType&                                                log               = nullLogger)
{   
    model.materials.emplace_back();
    tinygltf::Material& tMaterial = model.materials.back();
    uint                index    = model.materials.size() - 1;

    tMaterial.name = material.name();

    // baseColorFactor
    tMaterial.pbrMetallicRoughness.baseColorFactor = {
        material.baseColor().redF(),
        material.baseColor().greenF(),
        material.baseColor().blueF(),
        material.baseColor().alphaF()};

    // baseColorTexture
    if (!material.baseColorTextureDescriptor().isNull()) {
        if (!addedTextures.contains(material.baseColorTextureDescriptor().path())) {
            uint textureId = addGltfTexture(model, mesh, material.baseColorTextureDescriptor(), addedImages, saveTextureImages, log);
            addedTextures[material.baseColorTextureDescriptor().path()] = textureId;
        }

        tMaterial.pbrMetallicRoughness.baseColorTexture.index = addedTextures[material.baseColorTextureDescriptor().path()];
        //tMaterial.pbrMetallicRoughness.baseColorTexture.texCoord = 0; // default value
    }

    // metallicFactor
    tMaterial.pbrMetallicRoughness.metallicFactor =
        material.metallic();

    // roughnessFactor
    tMaterial.pbrMetallicRoughness.roughnessFactor =
        material.roughness();

    // metallicRoughnessTexture
    auto metallicRoughnessTextureDescriptor = material.textureDescriptor(toUnderlying(Material::TextureType::METALLIC_ROUGHNESS));
    if (!metallicRoughnessTextureDescriptor.isNull()) {
        if (!addedTextures.contains(metallicRoughnessTextureDescriptor.path())) {
            uint textureId = addGltfTexture(model, mesh, metallicRoughnessTextureDescriptor, addedImages, saveTextureImages, log);
            addedTextures[metallicRoughnessTextureDescriptor.path()] = textureId;
        }

        tMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index = addedTextures[metallicRoughnessTextureDescriptor.path()];
        //tMaterial.pbrMetallicRoughness.metallicRoughnessTexture.texCoord = 0; // default value
    }

    // emissiveFactor
    tMaterial.emissiveFactor = {
        material.emissiveColor().redF(),
        material.emissiveColor().greenF(),
        material.emissiveColor().blueF()};

    // emissiveTexture
    auto emissiveTextureDescriptor = material.textureDescriptor(toUnderlying(Material::TextureType::EMISSIVE));
    if (!emissiveTextureDescriptor.isNull()) {
        if (!addedTextures.contains(emissiveTextureDescriptor.path())) {
            uint textureId = addGltfTexture(model, mesh, emissiveTextureDescriptor, addedImages, saveTextureImages, log);
            addedTextures[emissiveTextureDescriptor.path()] = textureId;
        }

        tMaterial.emissiveTexture.index = addedTextures[emissiveTextureDescriptor.path()];
        //tMaterial.emissiveTexture.texCoord = 0; // default value
    }

    // normalTexture
    auto normalTextureDescriptor = material.textureDescriptor(toUnderlying(Material::TextureType::NORMAL));
    if (!normalTextureDescriptor.isNull()) {
        if (!addedTextures.contains(normalTextureDescriptor.path())) {
            uint textureId = addGltfTexture(model, mesh, normalTextureDescriptor, addedImages, saveTextureImages, log);
            addedTextures[normalTextureDescriptor.path()] = textureId;
        }

        tMaterial.normalTexture.index = addedTextures[normalTextureDescriptor.path()];
        //tMaterial.normalTexture.texCoord = 0; // default value
        tMaterial.normalTexture.scale = material.normalScale();
    }

    // occlusionTexture
    auto occlusionTextureDescriptor = material.textureDescriptor(toUnderlying(Material::TextureType::OCCLUSION));
    if (!occlusionTextureDescriptor.isNull()) {
        if (!addedTextures.contains(occlusionTextureDescriptor.path())) {
            uint textureId = addGltfTexture(model, mesh, occlusionTextureDescriptor, addedImages, saveTextureImages, log);
            addedTextures[occlusionTextureDescriptor.path()] = textureId;
        }

        tMaterial.occlusionTexture.index = addedTextures[occlusionTextureDescriptor.path()];
        //tMaterial.occlusionTexture.texCoord = 0; // default value
        tMaterial.occlusionTexture.strength = material.occlusionStrength();
    }

    // doubleSided
    tMaterial.doubleSided =
        material.doubleSided();

    // alphaMode
    switch (material.alphaMode()) {
        using enum Material::AlphaMode;

        case ALPHA_MASK:
            tMaterial.alphaMode = "MASK";
            break;
        case ALPHA_BLEND:
            tMaterial.alphaMode = "BLEND";
            break;
        case ALPHA_OPAQUE:
            tMaterial.alphaMode = "OPAQUE"; // default value
            break;
    }

    // alphaCutoff
    tMaterial.alphaCutoff =
        material.alphaCutoff();

    return {index, tMaterial};
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void addMeshToTinygltfModel(
    const MeshType&  m,
    tinygltf::Model& tModel,
    MeshInfo         meshInfo,
    bool             saveTextureImages,
    LogType&         log               = nullLogger)
{
    // mesh
    tModel.meshes.emplace_back();
    tinygltf::Mesh& mesh  = tModel.meshes.back();
    uint            meshI = tModel.meshes.size() - 1;

    // vertices
    int posAccI = -1;
    int colAccI = -1;
    int normAccI = -1;
    int texCoordAccI = -1;
    int tangentAccI = -1;

    // vertices position buffer, buffer view and accessor
    auto   posBuf = addGltfBuffer(tModel, 3 * m.vertexCount() * sizeof(float));
    float* fd     = reinterpret_cast<float*>(posBuf.second.data.data());
    vertexPositionsToBuffer(m, fd);

    auto posBufView  = addGltfBufferView(tModel, posBuf);
    auto posAccessor = addGltfAccessor(
        tModel, posBufView, TINYGLTF_COMPONENT_TYPE_FLOAT, TINYGLTF_TYPE_VEC3);

    Box3d bBox;
    if constexpr (HasBoundingBox<MeshType>) {
        bBox = m.boundingBox().template cast<double>();
    }
    if (bBox.isNull()) {
        bBox = boundingBox(m).template cast<double>();
    }

    posAccessor.second.maxValues =
        std::vector<double> {bBox.max().x(), bBox.max().y(), bBox.max().z()};
    posAccessor.second.minValues =
        std::vector<double> {bBox.min().x(), bBox.min().y(), bBox.min().z()};

    if constexpr (HasName<MeshType>) {
        if (!m.name().empty())
            mesh.name = m.name();
    }

    posAccI = posAccessor.first;

    if constexpr (HasPerVertexColor<MeshType>) {
        if (meshInfo.hasPerVertexColor()) {
            auto  colBuf = addGltfBuffer(tModel, 4 * m.vertexCount());
            uint* ud     = reinterpret_cast<uint*>(colBuf.second.data.data());
            vertexColorsToBuffer(m, ud, vcl::Color::Format::ABGR);

            auto colBufView  = addGltfBufferView(tModel, colBuf);
            auto colAccessor = addGltfAccessor(
                tModel,
                colBufView,
                TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE,
                TINYGLTF_TYPE_VEC4);
            // glTF requires normalized set to true for integer vertex colors
            colAccessor.second.normalized = true;

            colAccI = colAccessor.first;
        }
    }
    if constexpr (HasPerVertexNormal<MeshType>) {
        if (meshInfo.hasPerVertexNormal()) {
            auto normBuf =
                addGltfBuffer(tModel, 3 * m.vertexCount() * sizeof(float));
            fd = reinterpret_cast<float*>(normBuf.second.data.data());
            vertexNormalsToBuffer(m, fd, true);

            auto normBufView  = addGltfBufferView(tModel, normBuf);
            auto normAccessor = addGltfAccessor(
                tModel,
                normBufView,
                TINYGLTF_COMPONENT_TYPE_FLOAT,
                TINYGLTF_TYPE_VEC3);

            normAccI = normAccessor.first;
        }
    }
    if constexpr (HasPerVertexTexCoord<MeshType>) {
        if (meshInfo.hasPerVertexTexCoord()) {
            auto texCoordBuf =
                addGltfBuffer(tModel, 2 * m.vertexCount() * sizeof(float));
            fd = reinterpret_cast<float*>(texCoordBuf.second.data.data());
            vertexTexCoordsToBuffer(m, fd);

            // flip V coords
            for (unsigned int i = 1; i < m.vertexCount() * 2; i += 2)
                fd[i] = 1 - fd[i];

            auto texCoordBufView  = addGltfBufferView(tModel, texCoordBuf);
            auto texCoordAccessor = addGltfAccessor(
                tModel,
                texCoordBufView,
                TINYGLTF_COMPONENT_TYPE_FLOAT,
                TINYGLTF_TYPE_VEC2);

            texCoordAccI = texCoordAccessor.first;
        }
    }
    if constexpr (HasPerVertexTangent<MeshType>) {
        if (meshInfo.hasPerVertexTangent()) {
            auto tangentBuf =
                addGltfBuffer(tModel, 4 * m.vertexCount() * sizeof(float));
            fd = reinterpret_cast<float*>(tangentBuf.second.data.data());
            vertexTangentsToBuffer(m, fd, true);

            auto tangentBufView  = addGltfBufferView(tModel, tangentBuf);
            auto tangentAccessor = addGltfAccessor(
                tModel,
                tangentBufView,
                TINYGLTF_COMPONENT_TYPE_FLOAT,
                TINYGLTF_TYPE_VEC4);

            tangentAccI = tangentAccessor.first;
        }
    }

    // primitives

    // faces
    if constexpr (HasFaces<MeshType>) {
        if (meshInfo.hasFaces() && m.faceCount() > 0) {
            bool saveWithoutMaterial = false;

            if constexpr (HasPerVertexMaterialIndex<MeshType>) {
                if (meshInfo.hasPerVertexMaterialIndex()) {
                    // faces are sorted per face material index and saved into and index buffer
                    // for each material chunk, a primitive is created with relative accessor
                    // and buffer view into the index buffer
                    // materials are saved into the model

                    using FaceType = MeshType::FaceType;

                    // comparator of faces
                    // ordering first by per-face material index (if available),
                    // then by per-vertex material index (if available),
                    // then by per-face index
                    auto faceComp = [&](const FaceType& f1, const FaceType& f2) {
                        if constexpr (HasPerFaceMaterialIndex<MeshType>) {
                            if (isPerFaceMaterialIndexAvailable(m)) {
                                uint id1 = f1.materialIndex();
                                uint id2 = f2.materialIndex();
                                if (id1 != id2) { // do not return true if equal
                                    return id1 < id2;
                                }
                            }
                        }
                        if constexpr (HasPerVertexMaterialIndex<MeshType>) {
                            if (isPerVertexMaterialIndexAvailable(m)) {
                                uint id1 = f1.vertex(0)->materialIndex();
                                uint id2 = f2.vertex(0)->materialIndex();
                                if (id1 != id2) { // do not return true if equal
                                    return id1 < id2;
                                }
                            }
                        }

                        // if both per-vertex and per-face material indices are equal, sort
                        // by face index to have a stable sorting
                        return f1.index() < f2.index();
                    };

                    // get the list of face indices sorted by material ID
                    // using the face comparator defined above
                    const std::vector<uint> faceIndicesSortedByMaterialID =
                        sortFaceIndicesByFunction(m, faceComp, true);
                    auto indBuf = addGltfBuffer(
                        tModel, 3 * triangulatedFaceCount(m) * sizeof(uint));
                    uint* ud = reinterpret_cast<uint*>(indBuf.second.data.data());
                    TriPolyIndexBiMap indexMap;

                    triangulatedFaceVertexIndicesToBuffer(
                        m, ud, indexMap, MatrixStorageType::ROW_MAJOR);

                    // permute the triangulated face vertex indices according to the face
                    // sorting by material ID (the function also edits the index map from
                    // polygonal faces (which still refers to the mesh ones) to the
                    // triangulated faces (which refers to the sorted triangles))
                    permuteFaceVertexIndicesByFunctionToBuffer(
                        m,
                        ud,
                        faceComp,
                        indexMap);

                    // get the mapping from actual indices to compact indices
                    std::vector<uint> compactIndices = m.faceCompactIndices();

                    // compactIndices tells for each face, which is its new position
                    // we need the inverse mapping: for each new position, which is the old
                    // face index
                    std::vector<uint> oldFaceIndices(compactIndices.size());
                    for (uint i = 0; i < compactIndices.size(); ++i) {
                        oldFaceIndices[compactIndices[i]] = static_cast<uint>(i);
                    }

                    uint lastMaterialIndex = UINT_NULL;
                    std::unordered_map<uint, uint> modelMaterialIndices{};
                    uint chunkByteOffset = 0;
                    uint chunkLength = 0;
                    uint modelMaterialIndex = 0;
                    std::unordered_map<std::string, uint> addedTextures = {};
                    std::unordered_map<std::string, std::pair<uint, uint>> addedImages = {};

                    for (auto faceCompactIndex : faceIndicesSortedByMaterialID) {
                        auto& face = m.face(oldFaceIndices[faceCompactIndex]);
                        // get first vertex's material
                        uint materialIndex = face.vertex(0)->materialIndex();
                        uint faceChunkLength = indexMap.triangleCount(oldFaceIndices[faceCompactIndex]) * 3 * sizeof(uint);

                        if (materialIndex == lastMaterialIndex) {
                            chunkLength += faceChunkLength;

                            continue;
                        }

                        // end previous chunk
                        if (lastMaterialIndex != UINT_NULL) {
                            // buffer view, accessor and primitive
                            auto indBufView = addGltfBufferView(
                                tModel, indBuf, TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER, chunkByteOffset, chunkLength);
                            auto indAccessor = addGltfAccessor(
                                tModel,
                                indBufView,
                                TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT,
                                TINYGLTF_TYPE_SCALAR);
                            auto primitive = addGltfPrimitive(
                                mesh, posAccI, colAccI, normAccI, texCoordAccI, tangentAccI, TINYGLTF_MODE_TRIANGLES);

                            primitive.second.indices = indAccessor.first;
                            primitive.second.material = modelMaterialIndex;
                        }

                        // the material is added to the model if not already present
                        if (!modelMaterialIndices.contains(materialIndex)) {
                            auto material = addGltfMaterial(tModel, m, m.material(materialIndex), addedTextures, addedImages, saveTextureImages, log);
                            modelMaterialIndices[materialIndex] = material.first;
                        }

                        modelMaterialIndex = modelMaterialIndices.at(materialIndex);

                        // start new chunk
                        chunkByteOffset += chunkLength;
                        chunkLength = faceChunkLength;
                        lastMaterialIndex = materialIndex;
                    }

                    // add last chunk
                    if (lastMaterialIndex != UINT_NULL) {
                        // buffer view, accessor and primitive
                        auto indBufView = addGltfBufferView(
                            tModel, indBuf, TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER, chunkByteOffset, chunkLength);
                        auto indAccessor = addGltfAccessor(
                            tModel,
                            indBufView,
                            TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT,
                            TINYGLTF_TYPE_SCALAR);
                        auto primitive = addGltfPrimitive(
                            mesh, posAccI, colAccI, normAccI, texCoordAccI, tangentAccI, TINYGLTF_MODE_TRIANGLES);

                        primitive.second.indices = indAccessor.first;
                        primitive.second.material = modelMaterialIndex;
                    }
                }
                else {
                    saveWithoutMaterial = true;
                }
            }
            else {
                saveWithoutMaterial = true;
            }

            if (saveWithoutMaterial) {
                auto primitive = addGltfPrimitive(
                    mesh, posAccI, colAccI, normAccI, texCoordAccI, tangentAccI, TINYGLTF_MODE_TRIANGLES);

                // indices buffer, buffer view and accessor
                auto indBuf = addGltfBuffer(
                    tModel, 3 * triangulatedFaceCount(m) * sizeof(uint));
                uint* ud = reinterpret_cast<uint*>(indBuf.second.data.data());
                triangulatedFaceVertexIndicesToBuffer(m, ud);

                auto indBufView = addGltfBufferView(
                    tModel, indBuf, TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER);
                auto indAccessor = addGltfAccessor(
                    tModel,
                    indBufView,
                    TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT,
                    TINYGLTF_TYPE_SCALAR);

                primitive.second.indices = indAccessor.first;
            }
        }
    }

    // edges
    if constexpr (EdgeMeshConcept<MeshType>) {
        if (meshInfo.hasEdges() && m.edgeCount() > 0) {
            auto primitive = addGltfPrimitive(
                mesh, posAccI, colAccI, normAccI, texCoordAccI, tangentAccI, TINYGLTF_MODE_LINE);

            // indices buffer, buffer view and accessor
            auto indBuf =
                addGltfBuffer(tModel, 2 * m.edgeCount() * sizeof(uint));
            uint* ud = reinterpret_cast<uint*>(indBuf.second.data.data());
            edgeVertexIndicesToBuffer(m, ud);

            auto indBufView = addGltfBufferView(
                tModel, indBuf, TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER);
            auto indAccessor = addGltfAccessor(
                tModel,
                indBufView,
                TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT,
                TINYGLTF_TYPE_SCALAR);

            primitive.second.indices = indAccessor.first;
        }
    }

    // points
    // since no primitives were added, the mesh has neither faces nor edges
    if (mesh.primitives.size() == 0) {
        auto primitive = addGltfPrimitive(
            mesh, posAccI, colAccI, normAccI, texCoordAccI, tangentAccI, TINYGLTF_MODE_POINTS);
    }

    // node
    tModel.nodes.emplace_back();
    tinygltf::Node& node = tModel.nodes.back();
    node.mesh            = meshI;

    if constexpr (HasTransformMatrix<MeshType>) {
        if (!m.transformMatrix().isIdentity())
            node.matrix = std::vector<double>(
                m.transformMatrix().data(),
                m.transformMatrix().data() + m.transformMatrix().size());
    }

    uint nodeI = tModel.nodes.size() - 1;

    // scene
    tinygltf::Scene& scene = tModel.scenes.back();
    scene.nodes.push_back(nodeI);
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveGltf(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    tinygltf::Model model {};
    MeshInfo        meshInfo(m);

    model.asset.version   = detail::VCL_GLTF_ASSET_VERSION;
    model.asset.generator = detail::VCL_GLTF_GENERATOR_NAME;

    model.scenes.emplace_back();
    model.defaultScene = 0;

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    if (!settings.info.isEmpty())
        meshInfo = settings.info.intersect(meshInfo);

    detail::addMeshToTinygltfModel(
        m,
        model,
        meshInfo,
        settings.embedBuffers || settings.saveTextureImages, // saveTextureImages
        log);

    tinygltf::TinyGLTF gltf;
    bool success = gltf.WriteGltfSceneToFile(
        &model,
        filename,
        settings.embedBuffers, // embedImages
        settings.embedBuffers, // embedBuffers
        true,                  // pretty print
        settings.binary);      // write binary

    if (!success)
        throw std::runtime_error(
            "Failed to export mesh to glTF format: " + filename);
}

template<RangeOfMeshes Meshes, LoggerConcept LogType = NullLogger>
void saveGltf(
    Meshes&&            meshes,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    tinygltf::Model model {};

    model.asset.version   = detail::VCL_GLTF_ASSET_VERSION;
    model.asset.generator = detail::VCL_GLTF_GENERATOR_NAME;

    model.scenes.emplace_back();
    model.defaultScene = 0;

    for (const auto& mesh : meshes) {
        MeshInfo meshInfo(mesh);

        // make sure that the given info contains only components that are
        // actually available in the mesh. meshInfo will contain the
        // intersection between the components that the user wants to save and
        // the components that are available in the mesh.
        if (!settings.info.isEmpty())
            meshInfo = settings.info.intersect(meshInfo);

        detail::addMeshToTinygltfModel(
            mesh,
            model,
            meshInfo,
            settings.embedBuffers || settings.saveTextureImages, // saveTextureImages
            log);
    }

    tinygltf::TinyGLTF gltf;
    bool success = gltf.WriteGltfSceneToFile(
        &model,
        filename,
        settings.embedBuffers, // embedImages
        settings.embedBuffers, // embedBuffers
        true,                  // pretty print
        settings.binary);      // write binary

    if (!success)
        throw std::runtime_error(
            "Failed to export meshes to glTF format: " + filename);
}

} // namespace vcl

#endif // VCL_IO_MESH_GLTF_SAVE_H
