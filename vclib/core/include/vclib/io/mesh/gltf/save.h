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

#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>
#include <tiny_gltf.h>

#define VCL_GLTF_ASSET_VERSION "2.0"
#define VCL_GLTF_GENERATOR_NAME "vclib-tinygltf-exporter"

namespace vcl {

namespace detail {

inline std::pair<uint, tinygltf::Buffer&> addGltfBuffer(
    tinygltf::Model& model,
    size_t           size)
{
    model.buffers.emplace_back();
    tinygltf::Buffer& buf = model.buffers.back();
    uint index = model.buffers.size() - 1;
    buf.data.resize(size);

    return { index, buf };
}

inline std::pair<uint, tinygltf::BufferView&> addGltfBufferView(
    tinygltf::Model&                   model,
    std::pair<uint, tinygltf::Buffer&> buffer)
{
    model.bufferViews.emplace_back();
    tinygltf::BufferView& bufView = model.bufferViews.back();
    uint index = model.bufferViews.size() - 1;
    bufView.buffer = buffer.first;
    bufView.byteLength = buffer.second.data.size();

    return { index, bufView };
}

inline std::pair<uint, tinygltf::Accessor&> addGltfAccessor(
    tinygltf::Model&                   model,
    std::pair<uint, tinygltf::BufferView&> bufferView,
    int componentType,
    int type)
{
    //TODO check if component type and type are valid

    model.accessors.emplace_back();
    tinygltf::Accessor& accessor = model.accessors.back();
    uint index = model.accessors.size() - 1;
    accessor.bufferView = bufferView.first;
    accessor.componentType = componentType;
    accessor.type = type;
    // count = bytes / (comp_bytes * num_comp_in_type)
    accessor.count = bufferView.second.byteLength / (
        tinygltf::GetComponentSizeInBytes(componentType) * tinygltf::GetNumComponentsInType(type));

    return { index, accessor };
}

template<MeshConcept MeshType>
void addMeshToTinygltfModel(
    const MeshType&  m,
    tinygltf::Model& tModel,
    MeshInfo         meshInfo)
{
    //TODO updateBoundingBox(bunnyMesh); (?)
    uint posAccessorI, colAccessorI, normAccessorI, indAccessorI;

    // mesh
    tModel.meshes.emplace_back();
    tinygltf::Mesh& mesh = tModel.meshes.back();
    uint meshI = tModel.meshes.size() - 1;

    // vertices
    if constexpr (HasVertices<MeshType>) {
        // primitive
        mesh.primitives.emplace_back();
        tinygltf::Primitive& primitive = mesh.primitives.back();
        primitive.mode = TINYGLTF_MODE_TRIANGLES;

        // vertices position buffer, buffer view and accessor
        auto posBuf = addGltfBuffer(tModel, 3 * m.vertexCount() * sizeof(float));
        float* fd = reinterpret_cast<float*>(posBuf.second.data.data());
        vertexPositionsToBuffer(m, fd);

        auto posBufView = addGltfBufferView(tModel, posBuf);
        auto posAccessor = addGltfAccessor(tModel, posBufView, TINYGLTF_COMPONENT_TYPE_FLOAT, TINYGLTF_TYPE_VEC3);
        posAccessorI = posAccessor.first;

        //TODO remove
        // tModel.accessors.emplace_back();
        // tinygltf::Accessor& posAccessor = tModel.accessors.back();
        // posAccessorI = tModel.accessors.size() - 1;
        // posAccessor.bufferView = posBufViewI;
        // posAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT; // gltf FLOAT - 32bit
        // posAccessor.type = TINYGLTF_TYPE_VEC3;
        // posAccessor.count = posBufView.byteLength / (4 * 3); // count = bytes / (float_bytes * vec3_elem_count)

        if constexpr (HasBoundingBox<MeshType>) {
            if (!m.boundingBox().isNull()) {
                auto bBox = m.boundingBox();
                posAccessor.second.maxValues = std::vector<double>{bBox.max().x(), bBox.max().y(), bBox.max().z()};
                posAccessor.second.minValues = std::vector<double>{bBox.min().x(), bBox.min().y(), bBox.min().z()};
            }
        }

        primitive.attributes["POSITION"] = posAccessorI;

        if constexpr (HasPerVertexColor<MeshType>) {
            if (meshInfo.hasPerVertexColor()) {
                auto colBuf = addGltfBuffer(tModel, 4 * m.vertexCount());
                uint32_t* u32d = reinterpret_cast<uint32_t*>(colBuf.second.data.data());
                vertexColorsToBuffer(m, u32d, vcl::Color::Format::RGBA);

                auto colBufView = addGltfBufferView(tModel, colBuf);
                auto colAccessor = addGltfAccessor(tModel, colBufView, TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE, TINYGLTF_TYPE_VEC4);
                colAccessorI = colAccessor.first;

                primitive.attributes["COLOR_0"] = colAccessorI;
            }
        }
        if constexpr (HasPerVertexNormal<MeshType>) {
            if (meshInfo.hasPerVertexNormal()) {
                auto normBuf = addGltfBuffer(tModel, 3 * m.vertexCount() * sizeof(float));
                fd = reinterpret_cast<float*>(normBuf.second.data.data());
                vertexNormalsToBuffer(m, fd);

                auto normBufView = addGltfBufferView(tModel, normBuf);
                auto normAccessor = addGltfAccessor(tModel, normBufView, TINYGLTF_COMPONENT_TYPE_FLOAT, TINYGLTF_TYPE_VEC3);
                normAccessorI = normAccessor.first;

                primitive.attributes["NORMAL"] = normAccessorI;
            }
        }
        if constexpr (HasPerVertexTexCoord<MeshType>) {
            if (meshInfo.hasPerVertexTexCoord()) {
                //TODO per vertex tex coord
            }
        }
    }

    // faces
    if constexpr (HasFaces<MeshType>) {
        tinygltf::Primitive& primitive = mesh.primitives.back();

        // indices buffer, buffer view and accessor
        auto indBuf = addGltfBuffer(tModel, 3 * triangulatedFaceCount(m) * sizeof(uint));
        uint* ud = reinterpret_cast<uint*>(indBuf.second.data.data());
        triangulatedFaceVertexIndicesToBuffer(m, ud); //TODO should indexMap be used?

        auto indBufView = addGltfBufferView(tModel, indBuf);
        auto indAccessor = addGltfAccessor(tModel, indBufView, TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT, TINYGLTF_TYPE_SCALAR);
        indAccessorI = indAccessor.first;

        primitive.indices = indAccessorI;

        if (meshInfo.hasPerFaceColor()) {
            //TODO per face color
        }
    }

    // node
    tModel.nodes.emplace_back();
    tinygltf::Node& node = tModel.nodes.back();
    node.mesh = meshI;

    if (HasTransformMatrix<MeshType>) {
        node.matrix = std::vector<double>(m.transformMatrix().data(), m.transformMatrix().data() + m.transformMatrix().size());
    }

    uint nodeI = tModel.nodes.size() - 1;

    // scene
    tinygltf::Scene& scene = tModel.scenes.back();
    scene.nodes.push_back(nodeI);

    // --- TODO LIST ---

    //TODO save all materials
    //TODO save all textures

    //TODO a primitive per chunk (chunk split by material. Multiple chunks could be using the same material)
    //TODO each primitive will then reference an accessor for:
        //TODO - indices
        //TODO - text normals
    //TODO each primitive will reference its material
}

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveGltf(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    tinygltf::Model model{};
    MeshInfo meshInfo(m);

    model.asset.version = VCL_GLTF_ASSET_VERSION;
    model.asset.generator = VCL_GLTF_GENERATOR_NAME;

    model.scenes.emplace_back();
    model.defaultScene = 0;

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    if (!settings.info.isEmpty())
        meshInfo = settings.info.intersect(meshInfo);

    detail::addMeshToTinygltfModel(m, model, meshInfo);

    //TODO settings.saveTextureImages

    tinygltf::TinyGLTF gltf;
    bool success = gltf.WriteGltfSceneToFile(&model, filename,
        true,             // embedImages
        true,             // embedBuffers
        true,             // pretty print
        settings.binary); // write binary

    //TODO print success or failure?
    if (success)
        std::cout << "Export successful" << std::endl;
    else
        std::cout << "Export failed" << std::endl;
}

template<RangeOfMeshes Meshes, LoggerConcept LogType = NullLogger>
void saveGltf(
    Meshes&&            meshes,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    tinygltf::Model model{};

    model.asset.version = VCL_GLTF_ASSET_VERSION;
    model.asset.generator = VCL_GLTF_GENERATOR_NAME;

    model.scenes.emplace_back();
    model.defaultScene = 0;

    for (const auto& mesh : meshes) {
        MeshInfo meshInfo(mesh);

       // make sure that the given info contains only components that are actually
       // available in the mesh. meshInfo will contain the intersection between the
       // components that the user wants to save and the components that are
       // available in the mesh.
        if (!settings.info.isEmpty())
            meshInfo = settings.info.intersect(meshInfo);

        detail::addMeshToTinygltfModel(mesh, model, meshInfo);
    }

    //TODO settings.saveTextureImages

    tinygltf::TinyGLTF gltf;
    bool success = gltf.WriteGltfSceneToFile(&model, filename,
        true,             // embedImages
        true,             // embedBuffers
        true,             // pretty print
        settings.binary); // write binary

    //TODO print success or failure?
    if (success)
        std::cout << "Export successful" << std::endl;
    else
        std::cout << "Export failed" << std::endl;
}

} // namespace vcl

#endif // VCL_IO_MESH_GLTF_SAVE_H
