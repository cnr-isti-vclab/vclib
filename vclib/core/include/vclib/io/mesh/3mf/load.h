// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_3MF_LOAD_H
#define VCL_IO_MESH_3MF_LOAD_H

#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>

#ifndef NOMINMAX
#    define NOMINMAX
#endif
#include <lib3mf_implicit.hpp>
#if defined(min)
#    undef min
#endif
#if defined(max)
#    undef max
#endif

namespace vcl {

namespace detail {

// implementation details here...

} // namespace detail

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void load3mf(
    std::vector<MeshType>& m,
    const std::string&     filename,
    std::vector<MeshInfo>& loadedInfo,
    const LoadSettings&    settings = LoadSettings(),
    LogType&               log      = nullLogger)
{
    try {
        // entrypoint to the 3MF library
        Lib3MF::PWrapper wrapper = Lib3MF::CWrapper::loadLibrary();

        // Create a new model and a reader for 3MF files
        Lib3MF::PModel  model  = wrapper->CreateModel();
        Lib3MF::PReader reader = model->QueryReader("3mf");

        // Read the 3MF file into the model
        reader->ReadFromFile(filename);

        Lib3MF::PMeshObjectIterator mesh_it = model->GetMeshObjects();
        mesh_it->MoveNext();
        Lib3MF::PMeshObject mesh_3mf_format = mesh_it->GetCurrentMeshObject();

        std::vector<Lib3MF::sPosition> vertices;
        std::vector<Lib3MF::sTriangle> triangles;
        mesh_3mf_format->GetVertices(vertices);
        mesh_3mf_format->GetTriangleIndices(triangles);

        auto mit = m.begin();
        
        using PositionType = typename MeshType::VertexType::PositionType;

        (*mit).addVertices(mesh_3mf_format->GetVertexCount());
        for(int i = 0; Lib3MF::sPosition vertex : vertices){
            (*mit).vertex(i).position() = PositionType(vertex.m_Coordinates[0], vertex.m_Coordinates[1], vertex.m_Coordinates[2]);
        }

        if constexpr (HasFaces<MeshType>){
            uint fi = (*mit).addFaces(mesh_3mf_format->GetTriangleCount());
            for(Lib3MF::sTriangle triangle : triangles){
                auto& f = (*mit).face(fi);
                if constexpr (HasPolygons<MeshType>) {
                    f.resizeVertices(3);
                }
                for(int j=0; j<3; ++j){
                    f.setVertex(j, triangle.m_Indices[j]);
                }
            }
        }

    }
    catch (Lib3MF::ELib3MFException& e) {
        throw CannotOpenFileException("Failed to load 3MF file: " + filename);
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void load3mf(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    const LoadSettings& settings = LoadSettings(),
    LogType&            log      = nullLogger)
{
    std::vector<MeshType> meshes;
    std::vector<MeshInfo> infos;
    load3mf(meshes, filename, infos, settings, log);

    if (meshes.empty()) {
        return;
    }

    m          = std::move(meshes.front());
    loadedInfo = std::move(infos.front());
    for (std::size_t i = 1; i < meshes.size(); ++i) {
        m.append(std::move(meshes[i]));
        // todo manage infos
    }
}

} // namespace vcl

#endif // VCL_IO_MESH_3MF_LOAD_H
