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

#include <vclib/io.h>
#include <vclib/mesh.h>
#include <vclib/meshes.h>
#include <vclib/space.h>
#include <tiny_gltf.h>
#include <vector>

#include <iostream>

#include <cstdint>

namespace vcl
{
template<MeshConcept MeshType>
void addMeshToTinygltfModel(
    const MeshType&  m,
    tinygltf::Model& tM)
{
    MeshInfo meshInfo(m);
    //TODO vcl::updateBoundingBox(bunnyMesh); (?)
    uint posBufI, colBufI, normBufI;

    // vertices
    if constexpr (HasVertices<MeshType>) {
        // vertices position buffer
        m.buffers.push_back();
        // buffer index, buffer ref
        tinygltf::Buffer& posBuf = m.buffers.back();
        posBufI = m.buffers.size() - 1;

        posBuf.data.resize(3 * m.vertexCount() * sizeof(float));
        float* fd = reinterpret_cast<float*>(posBuf.data.data());
        vcl::vertexPositionsToBuffer(m, fd);

        // vertices color buffer
        if constexpr (HasPerVertexColor<MeshType>) {
            if (meshInfo.hasPerVertexColor()) {
                m.buffers.push_back();
                // buffer index, buffer ref
                tinygltf::Buffer& colBuf = m.buffers.back();
                colBufI = m.buffers.size() - 1;

                colBuf.data.resize(4 * m.vertexCount());
                uint32_t* u32d = reinterpret_cast<uint32_t*>(colBuf.data.data());
                vcl::vertexColorsToBuffer(m, u32d, vcl::Color::Format::RGBA);
            }
        }
        if constexpr (HasPerVertexNormal<MeshType>) {
            if (meshInfo.hasPerVertexNormal()) {
                m.buffers.push_back();
                // buffer index, buffer ref
                tinygltf::Buffer& normBuf = m.buffers.back();
                normBufI = m.buffers.size() - 1;

                normBuf.data.resize(3 * m.vertexCount() * sizeof(float));
                fd = reinterpret_cast<float*>(normBuf.data.data());
                vcl::vertexNormalsToBuffer(m, fd);
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
        //TODO triangle indices
        //TODO use function triangulatedFaceVertexIndicesToBuffer from algorithms/mesh/import_export/export_buffer.h

        if (meshInfo.hasPerFaceColor()) {
            //TODO per face color
        }
    }

    //TODO mesh



    // --- TODO LIST ---

    //TODO save all vertices into a single buffer (face triangulation needed?)
    //TODO save all vertex colors into a single buffer
    //TODO save all normals into a single buffer
    //TODO save all materials
    //TODO save all textures

    //TODO a primitive per chunk (chunk split by material. Multiple chunks could be using the same material)
    //TODO each primitive will then reference an accessor for:
        //TODO - indices
        //TODO - colors
        //TODO - normals
        //TODO - text normals
    //TODO each primitive will reference its material
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveGltf(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    MeshInfo meshInfo(m);
    tinygltf::Model tM{};

    //TODO settings.binary
    //TODO settings.saveTextureImages

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    if (!settings.info.isEmpty())
        meshInfo = settings.info.intersect(meshInfo);

    addMeshToTinygltfModel(m, tM);

    //TODO write scene to file
}
} // namespace vcl

int main()
{
    std::cout << "Loading mesh..." << std::endl;

    //TODO tri mesh (material index solo dei vertici)
    vcl::LoadSettings loadSettings;
    vcl::MeshInfo info;
    loadSettings.loadTextureImages = true;
    auto bunnyMesh = vcl::loadMesh<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj", info, loadSettings);
    vcl::updateBoundingBox(bunnyMesh);

    std::cout << "Converting mesh..." << std::endl;

    tinygltf::Model model;

    //TODO controlla il codice di save e load buffer
    //TODO controlla il codice di rendering
    //TODO 1) colore per vertice (to test)
    //TODO 2) templating su mesh concept
    //TODO 3) templating per mesh poligonali
    //TODO faces to triangles: triangulatedFaceVertexIndicesToBuffer

    //TODO pull dalla main branch

    //TODO materials, tex coord, nome della mesh
    //TODO separazione delle primitive in base al materiale dei vertici

    model.asset.version = "2.0";
    model.asset.generator = "vclib-tinygltf-exporter";

    // mesh
    tinygltf::Mesh mesh;

    // triangles
    tinygltf::Primitive primitive;
    primitive.attributes["POSITION"] = 0;
    primitive.attributes["NORMAL"] = 1; //TODO only if info.hasPerVertexNormal()
    if (info.hasPerVertexColor())
        primitive.attributes["COLOR_0"] = 3;
    primitive.indices = 2;
    primitive.mode = TINYGLTF_MODE_TRIANGLES;
    //TODO Tex coords
    //TODO material

    mesh.primitives.push_back(primitive);
    model.meshes.push_back(mesh);

    //TODO materials
    //guarda loadGltfPrimitiveMaterial

    // buffer
    tinygltf::Buffer positionsBuffer{}, normalsBuffer{}, indicesBuffer{}, colorsBuffer{};

    // vertices
    positionsBuffer.data.resize(3 * bunnyMesh.vertexCount() * sizeof(float));
    float* fd = reinterpret_cast<float*>(positionsBuffer.data.data());
    vcl::vertexPositionsToBuffer(bunnyMesh, fd);

    // normals
    normalsBuffer.data.resize(3 * bunnyMesh.vertexCount() * sizeof(float));
    fd = reinterpret_cast<float*>(normalsBuffer.data.data());
    vcl::vertexNormalsToBuffer(bunnyMesh, fd);

    // indices
    indicesBuffer.data.resize(3 * bunnyMesh.faceCount() * sizeof(uint32_t));
    uint32_t* u32d = reinterpret_cast<uint32_t*>(indicesBuffer.data.data());
    // indices of vertices that do not consider deleted vertices
    std::vector<uint32_t> vIndices = bunnyMesh.vertexCompactIndices();
    size_t indexI = 0;

    for (const vcl::TriMesh::Face& f : bunnyMesh.faces()) {
        for (const vcl::TriMesh::Vertex* v : f.vertices()) {
            u32d[indexI] = vIndices[bunnyMesh.index(v)];
            indexI++;
        }
    }

    // colors
    if (info.hasPerVertexColor()) {
        colorsBuffer.data.resize(4 * bunnyMesh.vertexCount());
        u32d = reinterpret_cast<uint32_t*>(colorsBuffer.data.data());
        vcl::vertexColorsToBuffer(bunnyMesh, u32d, vcl::Color::Format::RGBA);
    }

    model.buffers.push_back(positionsBuffer);
    model.buffers.push_back(normalsBuffer);
    model.buffers.push_back(indicesBuffer);
    if (info.hasPerVertexColor()) model.buffers.push_back(colorsBuffer);

    // buffer views
    tinygltf::BufferView positionsBufferView{}, normalsBufferView{}, indicesBufferView{}, colorsBufferView{};

    positionsBufferView.buffer = 0;
    positionsBufferView.byteLength = positionsBuffer.data.size();

    normalsBufferView.buffer = 1;
    normalsBufferView.byteLength = normalsBuffer.data.size();

    indicesBufferView.buffer = 2;
    indicesBufferView.byteLength = indicesBuffer.data.size();
    indicesBufferView.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;

    colorsBufferView.buffer = 3;
    colorsBufferView.byteLength = colorsBuffer.data.size();

    model.bufferViews.push_back(positionsBufferView);
    model.bufferViews.push_back(normalsBufferView);
    model.bufferViews.push_back(indicesBufferView);
    if (info.hasPerVertexColor()) model.bufferViews.push_back(colorsBufferView);

    // accessors
    tinygltf::Accessor positionsAccessor{}, normalsAccessor{}, indicesAccessor{}, colorsAccessor{};

    positionsAccessor.bufferView = 0;
    positionsAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT; // gltf FLOAT - 32bit
    positionsAccessor.type = TINYGLTF_TYPE_VEC3;
    positionsAccessor.count = positionsBufferView.byteLength / (4 * 3); // count = bytes / (float_bytes * vec3_elem_count)
    auto bBox = bunnyMesh.boundingBox();
    positionsAccessor.maxValues = std::vector<double>{bBox.max().x(), bBox.max().y(), bBox.max().z()};
    positionsAccessor.minValues = std::vector<double>{bBox.min().x(), bBox.min().y(), bBox.min().z()};

    normalsAccessor.bufferView = 1;
    normalsAccessor.componentType = TINYGLTF_COMPONENT_TYPE_FLOAT; // gltf FLOAT - 32bit
    normalsAccessor.type = TINYGLTF_TYPE_VEC3;
    normalsAccessor.count = normalsBufferView.byteLength / (4 * 3); // count = bytes / (float_bytes * vec3_elem_count)

    indicesAccessor.bufferView = 2;
    indicesAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT; // gltf UNSIGNED_INT - 32bit
    indicesAccessor.type = TINYGLTF_TYPE_SCALAR;
    indicesAccessor.count = indicesBufferView.byteLength / 4; // count = bytes / uint_bytes

    if (info.hasPerVertexColor()) {
        colorsAccessor.bufferView = 3;
        colorsAccessor.componentType = TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE; // gltf UNSIGNED_BYTE - 8bit
        colorsAccessor.type = TINYGLTF_TYPE_VEC4;
        colorsAccessor.count = colorsBufferView.byteLength / 4; // count = bytes / vec4_elem_count
    }

    model.accessors.push_back(positionsAccessor);
    model.accessors.push_back(normalsAccessor);
    model.accessors.push_back(indicesAccessor);
    if (info.hasPerVertexColor()) model.accessors.push_back(colorsAccessor);

    // node
    tinygltf::Node node;
    node.mesh = 0;
    node.matrix = std::vector<double>(bunnyMesh.transformMatrix().data(), bunnyMesh.transformMatrix().data() + bunnyMesh.transformMatrix().size());
    model.nodes.push_back(node);

    // default scene
    tinygltf::Scene scene;
    scene.nodes.push_back(0);
    model.scenes.push_back(scene);
    model.defaultScene = 0;

    std::cout << "Exporting to gltf..." << std::endl;

    tinygltf::TinyGLTF gltf;
    bool success = gltf.WriteGltfSceneToFile(&model, VCLIB_EXAMPLE_MESHES_PATH "/gltf/bunny_export_gltf.gltf",
          true,   // embedImages
          true,   // embedBuffers
          true,   // pretty print
          false); // write binary

    if (success)
        std::cout << "Export successful" << std::endl;
    else
        std::cout << "Export failed" << std::endl;

    return 0;
}
