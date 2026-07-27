// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_GLTF_SAVE_H
#define VCL_IO_MESH_GLTF_SAVE_H

#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>

#include <vclib/algorithms/mesh.h>

#include <tiny_gltf.h>

namespace vcl {

namespace detail {

inline static const std::string VCL_GLTF_ASSET_VERSION = "2.0";
inline static const std::string VCL_GLTF_GENERATOR_NAME =
    "vclib-tinygltf-exporter";

struct GltfAccessors
{
    int pos      = -1;
    int col      = -1;
    int norm     = -1;
    int texCoord = -1;
    int tangent  = -1;
};

inline std::pair<uint, uint> allocateInGltfBuffer(
    tinygltf::Model& model,
    size_t           size)
{
    if (model.buffers.empty()) {
        model.buffers.emplace_back();
    }
    tinygltf::Buffer& buf = model.buffers.front();

    uint byteOffset = buf.data.size();
    uint padding    = (4 - (byteOffset % 4)) % 4;
    byteOffset += padding;

    buf.data.resize(byteOffset + size);

    return {0, byteOffset};
}

inline std::pair<uint, tinygltf::BufferView&> addGltfBufferView(
    tinygltf::Model& model,
    uint             bufferIndex,
    uint             byteOffset,
    uint             byteLength,
    int              bufferViewTarget = TINYGLTF_TARGET_ARRAY_BUFFER)
{
    model.bufferViews.emplace_back();
    tinygltf::BufferView& bufView = model.bufferViews.back();
    uint                  index   = model.bufferViews.size() - 1;
    bufView.buffer                = bufferIndex;
    bufView.byteOffset            = byteOffset;
    bufView.byteLength            = byteLength;
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
    accessor.count      = bufferView.second.byteLength /
                          (tinygltf::GetComponentSizeInBytes(componentType) *
                           tinygltf::GetNumComponentsInType(type));
    accessor.normalized = normalized;

    return {index, accessor};
}

inline std::pair<uint, tinygltf::Primitive&> addGltfPrimitive(
    tinygltf::Mesh&      mesh,
    const GltfAccessors& accessors,
    int                  mode)
{
    mesh.primitives.emplace_back();
    tinygltf::Primitive& primitive   = mesh.primitives.back();
    uint                 index       = mesh.primitives.size() - 1;
    primitive.mode                   = mode;
    primitive.attributes["POSITION"] = accessors.pos;

    if (accessors.col >= 0)
        primitive.attributes["COLOR_0"] = accessors.col;

    if (accessors.norm >= 0)
        primitive.attributes["NORMAL"] = accessors.norm;

    if (accessors.tangent >= 0)
        primitive.attributes["TANGENT"] = accessors.tangent;

    // if multiple textures per render pass become supported
    // multiple TEXCOORD must be set (TEXCOORD_1, etc...)
    if (accessors.texCoord >= 0)
        primitive.attributes["TEXCOORD_0"] = accessors.texCoord;

    return {index, primitive};
}

inline void addGltfIndexedPrimitive(
    tinygltf::Model&     model,
    tinygltf::Mesh&      mesh,
    uint                 bufferIndex,
    uint                 byteOffset,
    uint                 byteLength,
    const GltfAccessors& accessors,
    int                  mode,
    uint                 materialIndex = UINT_NULL)
{
    auto indBufView = addGltfBufferView(
        model,
        bufferIndex,
        byteOffset,
        byteLength,
        TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER);
    auto indAccessor = addGltfAccessor(
        model,
        indBufView,
        TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT,
        TINYGLTF_TYPE_SCALAR);
    auto primitive = addGltfPrimitive(mesh, accessors, mode);

    primitive.second.indices = indAccessor.first;
    if (materialIndex != UINT_NULL) {
        primitive.second.material = materialIndex;
    }
}

inline uint addGltfSampler(
    tinygltf::Model&         model,
    const TextureDescriptor& textureDescriptor)
{
    model.samplers.emplace_back();
    tinygltf::Sampler& sampler = model.samplers.back();
    uint               index   = model.samplers.size() - 1;
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
    tImage.width            = image.width();
    tImage.height           = image.height();
    tImage.component        = 4; // channels
    tImage.bits             = 8; // bits per channel
    tImage.pixel_type       = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE;
    tImage.uri              = path;

    if (saveTextureImages)
        tImage.image.assign(image.data(), image.data() + image.sizeInBytes());

    return index;
}

inline std::size_t hashSampler(const TextureDescriptor& textureDescriptor)
{
    std::size_t minFilter =
        std::hash<int> {}(static_cast<int>(textureDescriptor.minFilter()));
    std::size_t magFilter =
        std::hash<int> {}(static_cast<int>(textureDescriptor.magFilter()));
    std::size_t wrapS =
        std::hash<int> {}(static_cast<int>(textureDescriptor.wrapU()));
    std::size_t wrapT =
        std::hash<int> {}(static_cast<int>(textureDescriptor.wrapV()));

    return minFilter ^ (magFilter << 1) ^ (wrapS << 2) ^ (wrapT << 3);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
inline uint addGltfTexture(
    tinygltf::Model&                       model,
    const MeshType&                        mesh,
    const TextureDescriptor&               textureDescriptor,
    std::unordered_map<std::string, uint>& addedImages,
    std::unordered_map<std::size_t, uint>& addedSamplers,
    bool                                   saveTextureImages,
    LogType&                               log = nullLogger)
{
    if (textureDescriptor.isNull()) {
        log.log(
            "Cannot save empty texture: " + textureDescriptor.path(),
            LogType::WARNING_LOG);

        return UINT_NULL;
    }

    if (saveTextureImages &&
        mesh.textureImage(textureDescriptor.path()).isNull()) {
        log.log(
            "Cannot save empty image: " + textureDescriptor.path(),
            LogType::WARNING_LOG);

        return UINT_NULL;
    }

    model.textures.emplace_back();
    tinygltf::Texture& texture = model.textures.back();
    uint               index   = model.textures.size() - 1;
    uint               imageId = -1, samplerId = -1;
    std::size_t        samplerHash = hashSampler(textureDescriptor);

    if (!addedImages.contains(textureDescriptor.path())) {
        imageId = addGltfImage(
            model,
            mesh.textureImage(textureDescriptor.path()),
            textureDescriptor.path(),
            saveTextureImages);
        addedImages[textureDescriptor.path()] = imageId;
    }

    if (!addedSamplers.contains(samplerHash)) {
        samplerId                  = addGltfSampler(model, textureDescriptor);
        addedSamplers[samplerHash] = samplerId;
    }

    texture.source  = addedImages[textureDescriptor.path()];
    texture.sampler = addedSamplers[samplerHash];

    return index;
}

inline std::size_t hashTexture(const TextureDescriptor& textureDescriptor)
{
    return std::hash<std::string> {}(textureDescriptor.path()) ^
           (hashSampler(textureDescriptor) << 1);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
inline std::pair<uint, tinygltf::Material&> addGltfMaterial(
    tinygltf::Model&                       model,
    const MeshType&                        mesh,
    const Material&                        material,
    std::unordered_map<std::size_t, uint>& addedTextures,
    std::unordered_map<std::string, uint>& addedImages,
    std::unordered_map<std::size_t, uint>& addedSamplers,
    bool                                   saveTextureImages,
    LogType&                               log = nullLogger)
{
    model.materials.emplace_back();
    tinygltf::Material& tMaterial = model.materials.back();
    uint                index     = model.materials.size() - 1;

    tMaterial.name = material.name();

    // texture list
    std::vector<Material::TextureType> textureTypes = {
        Material::TextureType::BASE_COLOR,
        Material::TextureType::METALLIC_ROUGHNESS,
        Material::TextureType::NORMAL,
        Material::TextureType::OCCLUSION,
        Material::TextureType::EMISSIVE};

    // textures
    for (const Material::TextureType& type : textureTypes) {
        auto textureDescriptor = material.textureDescriptor(toUnderlying(type));
        if (!textureDescriptor.isNull()) {
            std::size_t textureHash = hashTexture(textureDescriptor);

            if (!addedTextures.contains(textureHash)) {
                uint textureId = addGltfTexture(
                    model,
                    mesh,
                    textureDescriptor,
                    addedImages,
                    addedSamplers,
                    saveTextureImages,
                    log);
                addedTextures[textureHash] = textureId;
            }

            if (addedTextures[textureHash] != UINT_NULL) {
                int textureIndex = addedTextures[textureHash];

                switch (type) {
                    using enum Material::TextureType;

                case BASE_COLOR:
                    tMaterial.pbrMetallicRoughness.baseColorTexture.index =
                        textureIndex;
                    break;

                case METALLIC_ROUGHNESS:
                    tMaterial.pbrMetallicRoughness.metallicRoughnessTexture
                        .index = textureIndex;
                    break;

                case NORMAL:
                    tMaterial.normalTexture.index = textureIndex;
                    tMaterial.normalTexture.scale = material.normalScale();
                    break;

                case OCCLUSION:
                    tMaterial.occlusionTexture.index = textureIndex;
                    tMaterial.occlusionTexture.strength =
                        material.occlusionStrength();
                    break;

                case EMISSIVE:
                    tMaterial.emissiveTexture.index = textureIndex;
                    break;

                default:
                    log.log(
                        std::format(
                            "Unhandled TextureType: {}",
                            static_cast<int>(type)),
                        LogType::WARNING_LOG);
                }
            }
        }
    }

    // baseColorFactor
    tMaterial.pbrMetallicRoughness.baseColorFactor = {
        material.baseColor().redF(),
        material.baseColor().greenF(),
        material.baseColor().blueF(),
        material.baseColor().alphaF()};

    // metallicFactor
    tMaterial.pbrMetallicRoughness.metallicFactor = material.metallic();

    // roughnessFactor
    tMaterial.pbrMetallicRoughness.roughnessFactor = material.roughness();

    // emissiveFactor
    tMaterial.emissiveFactor = {
        material.emissiveColor().redF(),
        material.emissiveColor().greenF(),
        material.emissiveColor().blueF()};

    // doubleSided
    tMaterial.doubleSided = material.doubleSided();

    // alphaMode
    switch (material.alphaMode()) {
        using enum Material::AlphaMode;

    case ALPHA_MASK: tMaterial.alphaMode = "MASK"; break;
    case ALPHA_BLEND: tMaterial.alphaMode = "BLEND"; break;
    case ALPHA_OPAQUE:
        tMaterial.alphaMode = "OPAQUE"; // default value
        break;
    }

    // alphaCutoff
    tMaterial.alphaCutoff = material.alphaCutoff();

    return {index, tMaterial};
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void addMeshToTinygltfModel(
    const MeshType&  m,
    tinygltf::Model& tModel,
    MeshInfo         meshInfo,
    bool             saveTextureImages,
    LogType&         log = nullLogger)
{
    // mesh
    tModel.meshes.emplace_back();
    tinygltf::Mesh& mesh  = tModel.meshes.back();
    uint            meshI = tModel.meshes.size() - 1;

    // vertices accessors
    GltfAccessors accessors;

    bool exportWedgeTexCoord = false;
    if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
        if (meshInfo.hasPerFaceWedgeTexCoords()) {
            exportWedgeTexCoord = true;
        }
    }

    bool exportVertexTexCoord = false;
    if constexpr (HasPerVertexTexCoord<MeshType>) {
        if (meshInfo.hasPerVertexTexCoord() && !exportWedgeTexCoord) {
            exportVertexTexCoord = true;
        }
    }

    std::vector<std::pair<uint, uint>>          vertWedgeMap;
    std::list<uint>                             vertsToDuplicate;
    std::list<std::list<std::pair<uint, uint>>> facesToReassign;
    uint                                        nV = 0;

    if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
        if (exportWedgeTexCoord) {
            nV = verticesToDuplicateByWedgeTexCoordsCount(
                m, vertWedgeMap, vertsToDuplicate, facesToReassign);
        }
    }

    uint totalVertices = m.vertexCount() + nV;

    // vertices position buffer, buffer view and accessor
    auto posAlloc =
        allocateInGltfBuffer(tModel, 3 * totalVertices * sizeof(float));
    float* fd = reinterpret_cast<float*>(
        tModel.buffers[posAlloc.first].data.data() + posAlloc.second);
    vertexPositionsToBuffer(m, fd);
    if (exportWedgeTexCoord) {
        appendDuplicateVertexPositionsToBuffer(m, vertsToDuplicate, fd);
    }

    auto posBufView = addGltfBufferView(
        tModel,
        posAlloc.first,
        posAlloc.second,
        3 * totalVertices * sizeof(float));
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

    accessors.pos = posAccessor.first;

    if constexpr (HasPerVertexColor<MeshType>) {
        if (meshInfo.hasPerVertexColor()) {
            auto  colAlloc = allocateInGltfBuffer(tModel, 4 * totalVertices);
            uint* ud       = reinterpret_cast<uint*>(
                tModel.buffers[colAlloc.first].data.data() + colAlloc.second);
            vertexColorsToBuffer(m, ud, vcl::Color::Format::ABGR);
            if (exportWedgeTexCoord) {
                appendDuplicateVertexColorsToBuffer(
                    m, vertsToDuplicate, ud, vcl::Color::Format::ABGR);
            }

            auto colBufView = addGltfBufferView(
                tModel, colAlloc.first, colAlloc.second, 4 * totalVertices);
            auto colAccessor = addGltfAccessor(
                tModel,
                colBufView,
                TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE,
                TINYGLTF_TYPE_VEC4);
            // glTF requires normalized set to true for integer vertex colors
            colAccessor.second.normalized = true;

            accessors.col = colAccessor.first;
        }
    }
    if constexpr (HasPerVertexNormal<MeshType>) {
        if (meshInfo.hasPerVertexNormal()) {
            auto normAlloc =
                allocateInGltfBuffer(tModel, 3 * totalVertices * sizeof(float));
            fd = reinterpret_cast<float*>(
                tModel.buffers[normAlloc.first].data.data() + normAlloc.second);
            vertexNormalsToBuffer(m, fd, true);
            if (exportWedgeTexCoord) {
                appendDuplicateVertexNormalsToBuffer(
                    m, vertsToDuplicate, fd, true);
            }

            auto normBufView = addGltfBufferView(
                tModel,
                normAlloc.first,
                normAlloc.second,
                3 * totalVertices * sizeof(float));
            auto normAccessor = addGltfAccessor(
                tModel,
                normBufView,
                TINYGLTF_COMPONENT_TYPE_FLOAT,
                TINYGLTF_TYPE_VEC3);

            accessors.norm = normAccessor.first;
        }
    }
    if (exportWedgeTexCoord || exportVertexTexCoord) {
        auto texCoordAlloc =
            allocateInGltfBuffer(tModel, 2 * totalVertices * sizeof(float));
        fd = reinterpret_cast<float*>(
            tModel.buffers[texCoordAlloc.first].data.data() +
            texCoordAlloc.second);

        if (exportWedgeTexCoord) {
            if constexpr (HasPerFaceWedgeTexCoords<MeshType>) {
                wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
                    m, vertWedgeMap, facesToReassign, fd);
            }
        }
        else if (exportVertexTexCoord) {
            vertexTexCoordsToBuffer(m, fd);
        }

        // flip V coords
        for (unsigned int i = 1; i < totalVertices * 2; i += 2)
            fd[i] = 1 - fd[i];

        auto texCoordBufView = addGltfBufferView(
            tModel,
            texCoordAlloc.first,
            texCoordAlloc.second,
            2 * totalVertices * sizeof(float));
        auto texCoordAccessor = addGltfAccessor(
            tModel,
            texCoordBufView,
            TINYGLTF_COMPONENT_TYPE_FLOAT,
            TINYGLTF_TYPE_VEC2);

        accessors.texCoord = texCoordAccessor.first;
    }
    if constexpr (HasPerVertexTangent<MeshType>) {
        if (meshInfo.hasPerVertexTangent()) {
            auto tangentAlloc =
                allocateInGltfBuffer(tModel, 4 * totalVertices * sizeof(float));
            fd = reinterpret_cast<float*>(
                tModel.buffers[tangentAlloc.first].data.data() +
                tangentAlloc.second);
            vertexTangentsToBuffer(m, fd, true);
            if (exportWedgeTexCoord) {
                appendDuplicateVertexTangentsToBuffer(
                    m, vertsToDuplicate, fd, true);
            }

            auto tangentBufView = addGltfBufferView(
                tModel,
                tangentAlloc.first,
                tangentAlloc.second,
                4 * totalVertices * sizeof(float));
            auto tangentAccessor = addGltfAccessor(
                tModel,
                tangentBufView,
                TINYGLTF_COMPONENT_TYPE_FLOAT,
                TINYGLTF_TYPE_VEC4);

            accessors.tangent = tangentAccessor.first;
        }
    }

    // primitives

    // faces
    if constexpr (HasFaces<MeshType>) {
        if (meshInfo.hasFaces() && m.faceCount() > 0) {
            bool saveWithoutMaterial = false;

            bool exportMaterials      = false;
            bool usePerFaceMaterial   = false;
            bool usePerVertexMaterial = false;

            if constexpr (HasPerFaceMaterialIndex<MeshType>) {
                if (meshInfo.hasPerFaceMaterialIndex()) {
                    exportMaterials    = true;
                    usePerFaceMaterial = true;
                }
            }

            if constexpr (HasPerVertexMaterialIndex<MeshType>) {
                if (meshInfo.hasPerVertexMaterialIndex() &&
                    !usePerFaceMaterial) {
                    exportMaterials      = true;
                    usePerVertexMaterial = true;
                }
            }

            if (exportMaterials) {
                // faces are sorted per material index and saved into an
                // index buffer for each material chunk, a primitive is
                // created with relative accessor and buffer view into the
                // index buffer materials are saved into the model

                using FaceType = MeshType::FaceType;

                // comparator of faces
                // ordering by material index (if available)
                auto faceComp = [&](const FaceType& f1, const FaceType& f2) {
                    if (usePerFaceMaterial) {
                        if constexpr (HasPerFaceMaterialIndex<MeshType>) {
                            if (isPerFaceMaterialIndexAvailable(m)) {
                                uint id1 = f1.materialIndex();
                                uint id2 = f2.materialIndex();
                                if (id1 != id2) { // do not return true if equal
                                    return id1 < id2;
                                }
                            }
                        }
                    }
                    else if (usePerVertexMaterial) {
                        if constexpr (HasPerVertexMaterialIndex<MeshType>) {
                            if (isPerVertexMaterialIndexAvailable(m)) {
                                uint id1 = f1.vertex(0)->materialIndex();
                                uint id2 = f2.vertex(0)->materialIndex();
                                if (id1 != id2) { // do not return true if equal
                                    return id1 < id2;
                                }
                            }
                        }
                    }

                    // if per-vertex material indices are equal,
                    // sort by face index to have a stable sorting
                    return f1.index() < f2.index();
                };

                // get the list of face indices sorted by material ID
                // using the face comparator defined above
                const std::vector<uint> faceIndicesSortedByMaterialID =
                    sortFaceIndicesByFunction(m, faceComp, true);
                auto indAlloc = allocateInGltfBuffer(
                    tModel, 3 * triangulatedFaceCount(m) * sizeof(uint));
                uint* ud = reinterpret_cast<uint*>(
                    tModel.buffers[indAlloc.first].data.data() +
                    indAlloc.second);
                TriPolyIndexBiMap indexMap;

                triangulatedFaceVertexIndicesToBuffer(
                    m, ud, indexMap, MatrixStorageType::ROW_MAJOR);

                if (exportWedgeTexCoord) {
                    replaceTriangulatedFaceVertexIndicesByVertexDuplicationToBuffer(
                        m, vertsToDuplicate, facesToReassign, indexMap, ud);
                }

                // permute the triangulated face vertex indices according to
                // the face sorting by material ID (the function also edits
                // the index map from polygonal faces (which still refers to
                // the mesh ones) to the triangulated faces (which refers to
                // the sorted triangles))
                permuteFaceVertexIndicesByFunctionToBuffer(
                    m, ud, faceComp, indexMap);

                // get the mapping from actual indices to compact indices
                std::vector<uint> compactIndices = m.faceCompactIndices();

                // compactIndices tells for each face, which is its new
                // position we need the inverse mapping: for each new
                // position, which is the old face index
                std::vector<uint> oldFaceIndices(compactIndices.size());
                for (uint i = 0; i < compactIndices.size(); ++i) {
                    oldFaceIndices[compactIndices[i]] = static_cast<uint>(i);
                }

                uint                           lastMaterialIndex = UINT_NULL;
                std::unordered_map<uint, uint> modelMaterialIndices {};
                uint chunkByteOffset    = indAlloc.second;
                uint chunkLength        = 0;
                uint modelMaterialIndex = 0;
                std::unordered_map<std::size_t, uint> addedTextures = {};
                std::unordered_map<std::string, uint> addedImages   = {};
                std::unordered_map<std::size_t, uint> addedSamplers = {};

                auto flushChunk = [&]() {
                    // buffer view, accessor and primitive
                    if (chunkLength > 0) {
                        addGltfIndexedPrimitive(
                            tModel,
                            mesh,
                            indAlloc.first,
                            chunkByteOffset,
                            chunkLength,
                            accessors,
                            TINYGLTF_MODE_TRIANGLES,
                            lastMaterialIndex == UINT_NULL ?
                                UINT_NULL :
                                modelMaterialIndex);
                    }
                };

                for (auto faceCompactIndex : faceIndicesSortedByMaterialID) {
                    auto& face = m.face(oldFaceIndices[faceCompactIndex]);

                    uint materialIndex = 0;
                    if (usePerFaceMaterial) {
                        if constexpr (HasPerFaceMaterialIndex<MeshType>) {
                            materialIndex = face.materialIndex();
                        }
                    }
                    else if (usePerVertexMaterial) {
                        if constexpr (HasPerVertexMaterialIndex<MeshType>) {
                            materialIndex = face.vertex(0)->materialIndex();
                        }
                    }
                    uint faceChunkLength =
                        indexMap.triangleCount(
                            oldFaceIndices[faceCompactIndex]) *
                        3 * sizeof(uint);

                    if (materialIndex == lastMaterialIndex) {
                        chunkLength += faceChunkLength;

                        continue;
                    }

                    // end previous chunk
                    flushChunk();

                    // the material is added to the model if not already
                    // present
                    if (!modelMaterialIndices.contains(materialIndex)) {
                        auto material = addGltfMaterial(
                            tModel,
                            m,
                            m.material(materialIndex),
                            addedTextures,
                            addedImages,
                            addedSamplers,
                            saveTextureImages,
                            log);
                        modelMaterialIndices[materialIndex] = material.first;
                    }

                    modelMaterialIndex = modelMaterialIndices.at(materialIndex);

                    // start new chunk
                    chunkByteOffset += chunkLength;
                    chunkLength       = faceChunkLength;
                    lastMaterialIndex = materialIndex;
                }

                // add last chunk
                flushChunk();
            }
            else {
                saveWithoutMaterial = true;
            }

            if (saveWithoutMaterial) {
                // indices buffer, buffer view and accessor
                auto indAlloc = allocateInGltfBuffer(
                    tModel, 3 * triangulatedFaceCount(m) * sizeof(uint));
                uint* ud = reinterpret_cast<uint*>(
                    tModel.buffers[indAlloc.first].data.data() +
                    indAlloc.second);

                TriPolyIndexBiMap indexMap;
                triangulatedFaceVertexIndicesToBuffer(
                    m, ud, indexMap, MatrixStorageType::ROW_MAJOR);

                if (exportWedgeTexCoord) {
                    replaceTriangulatedFaceVertexIndicesByVertexDuplicationToBuffer(
                        m, vertsToDuplicate, facesToReassign, indexMap, ud);
                }

                addGltfIndexedPrimitive(
                    tModel,
                    mesh,
                    indAlloc.first,
                    indAlloc.second,
                    3 * triangulatedFaceCount(m) * sizeof(uint),
                    accessors,
                    TINYGLTF_MODE_TRIANGLES);
            }
        }
    }

    // edges
    if constexpr (EdgeMeshConcept<MeshType>) {
        if (meshInfo.hasEdges() && m.edgeCount() > 0) {
            // indices buffer, buffer view and accessor
            auto indAlloc =
                allocateInGltfBuffer(tModel, 2 * m.edgeCount() * sizeof(uint));
            uint* ud = reinterpret_cast<uint*>(
                tModel.buffers[indAlloc.first].data.data() + indAlloc.second);
            edgeVertexIndicesToBuffer(m, ud);

            addGltfIndexedPrimitive(
                tModel,
                mesh,
                indAlloc.first,
                indAlloc.second,
                2 * m.edgeCount() * sizeof(uint),
                accessors,
                TINYGLTF_MODE_LINE);
        }
    }

    // points
    if (mesh.primitives.size() == 0) {
        // since no primitives were added, the mesh has neither faces nor edges
        auto primitive =
            addGltfPrimitive(mesh, accessors, TINYGLTF_MODE_POINTS);
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

/**
 * @brief Saves a mesh to a file with the given filename.
 *
 * @note Currently, this function has several limitations:
 *  1) only a single set of texture coordinates is exported (TEXCOORD_0).
 *     Per-wedge texture coordinates have priority over per-vertex ones.
 *  2) output primitives are organized by per-face or per-vertex material
 *     indices (per-face has priority). If per-vertex material indices are used,
 *     a face will arbitrarily inherit the material of its first vertex.
 *  4) even if different meshes were to share the same data, it would be
 *     duplicated. Each mesh is exported without consideration to the other
 *     meshes' data
 *
 * @tparam MeshType The type of mesh to save. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: The mesh object to save.
 * @param[in] filename: The filename of the file where to save the mesh data.
 * @param[in] settings: Settings for saving the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * saving.
 */

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
        settings.embedBuffers ||
            settings.saveTextureImages, // saveTextureImages
        log);

    tinygltf::TinyGLTF gltf;
    bool               success = gltf.WriteGltfSceneToFile(
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

/**
 * @brief Saves a range of meshes to a file with the given filename.
 *
 * @note Currently, this function has several limitations:
 *  1) only a single set of texture coordinates is exported (TEXCOORD_0).
 *     Per-wedge texture coordinates have priority over per-vertex ones.
 *  2) output primitives are organized by per-face or per-vertex material
 *     indices (per-face has priority). If per-vertex material indices are used,
 *     a face will arbitrarily inherit the material of its first vertex.
 *  4) even if different meshes were to share the same data, it would be
 *     duplicated. Each mesh is exported without consideration to the other
 *     meshes' data
 *
 * @tparam MeshType The type of mesh to save. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] meshes: The range of meshes to save.
 * @param[in] filename: The filename of the file where to save the mesh data.
 * @param[in] settings: Settings for saving the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * saving.
 */

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
            settings.embedBuffers ||
                settings.saveTextureImages, // saveTextureImages
            log);
    }

    tinygltf::TinyGLTF gltf;
    bool               success = gltf.WriteGltfSceneToFile(
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
