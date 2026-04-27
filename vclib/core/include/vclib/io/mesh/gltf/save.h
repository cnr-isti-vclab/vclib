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
    int bufferViewTarget = TINYGLTF_TARGET_ARRAY_BUFFER)
{
    model.bufferViews.emplace_back();
    tinygltf::BufferView& bufView = model.bufferViews.back();
    uint                  index   = model.bufferViews.size() - 1;
    bufView.buffer                = buffer.first;
    bufView.byteLength            = buffer.second.data.size();
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
    // TODO check if component type and type are valid values

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
    tinygltf::Mesh&  mesh,
    int              posAccessorIndex,
    int              colAccessorIndex,
    int              normAccessorIndex,
    int              mode) {
    // TODO check if mode is a valid value

    mesh.primitives.emplace_back();
    tinygltf::Primitive& primitive   = mesh.primitives.back();
    uint                index        = mesh.primitives.size() - 1;
    primitive.mode                   = mode;
    primitive.attributes["POSITION"] = posAccessorIndex;

    if (colAccessorIndex >= 0)
        primitive.attributes["COLOR_0"] = colAccessorIndex;

    if (normAccessorIndex >= 0)
        primitive.attributes["NORMAL"] = normAccessorIndex;

    return {index, primitive};
}

template<MeshConcept MeshType>
void addMeshToTinygltfModel(
    const MeshType&  m,
    tinygltf::Model& tModel,
    MeshInfo         meshInfo)
{
    // mesh
    tModel.meshes.emplace_back();
    tinygltf::Mesh& mesh  = tModel.meshes.back();
    uint            meshI = tModel.meshes.size() - 1;

    // vertices
    int posAccI = -1, colAccI = -1, normAccI = -1;

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

    posAccessor.second.maxValues = std::vector<double> {
        bBox.max().x(), bBox.max().y(), bBox.max().z()};
    posAccessor.second.minValues = std::vector<double> {
        bBox.min().x(), bBox.min().y(), bBox.min().z()};

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
            // TODO per vertex tex coord
        }
    }

    // primitives

    // faces
    if constexpr (HasFaces<MeshType>) {
        if (meshInfo.hasFaces()) {
            auto primitive =
                addGltfPrimitive(mesh, posAccI, colAccI, normAccI, TINYGLTF_MODE_TRIANGLES);

            // indices buffer, buffer view and accessor
            auto indBuf =
                addGltfBuffer(tModel, 3 * triangulatedFaceCount(m) * sizeof(uint));
            uint* ud = reinterpret_cast<uint*>(indBuf.second.data.data());
            triangulatedFaceVertexIndicesToBuffer(
                m, ud);

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

    // edges
    if constexpr (EdgeMeshConcept<MeshType>) {
        if (meshInfo.hasEdges()) {
            auto primitive =
                addGltfPrimitive(mesh, posAccI, colAccI, normAccI, TINYGLTF_MODE_LINE);

            // indices buffer, buffer view and accessor
            auto indBuf =
                addGltfBuffer(tModel, 2 * m.edgeCount() * sizeof(uint));
            uint* ud = reinterpret_cast<uint*>(indBuf.second.data.data());
            edgeVertexIndicesToBuffer(
                m, ud);

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
        auto primitive =
            addGltfPrimitive(mesh, posAccI, colAccI, normAccI, TINYGLTF_MODE_POINTS);
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

    // --- TODO LIST ---

    // TODO save all materials
    // TODO save all textures (textures, images, samplers)

    // TODO a primitive per chunk (chunk split by material. Multiple chunks
    // could be using the same material)
    // TODO each primitive will then reference an accessor for:
    // TODO - texture normals
    // TODO each primitive will reference its material
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

    detail::addMeshToTinygltfModel(m, model, meshInfo);

    // TODO settings.saveTextureImages

    tinygltf::TinyGLTF gltf;
    bool               success = gltf.WriteGltfSceneToFile(
        &model,
        filename,
        true,             // embedImages
        true,             // embedBuffers
        true,             // pretty print
        settings.binary); // write binary

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

        detail::addMeshToTinygltfModel(mesh, model, meshInfo);
    }

    // TODO settings.saveTextureImages

    tinygltf::TinyGLTF gltf;
    bool               success = gltf.WriteGltfSceneToFile(
        &model,
        filename,
        true,             // embedImages
        true,             // embedBuffers
        true,             // pretty print
        settings.binary); // write binary

    if (!success)
        throw std::runtime_error(
            "Failed to export meshes to glTF format: " + filename);
}

} // namespace vcl

#endif // VCL_IO_MESH_GLTF_SAVE_H
