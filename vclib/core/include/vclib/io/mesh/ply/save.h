// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_PLY_SAVE_H
#define VCL_IO_MESH_PLY_SAVE_H

#include "detail/edge.h"
#include "detail/extra.h"
#include "detail/face.h"
#include "detail/material.h"
#include "detail/vertex.h"

#include <vclib/io/mesh/settings.h>

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&     m,
    std::ostream&       fp,
    const std::string&  fileBasePath,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    using namespace detail;
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    if (!settings.info.isEmpty())
        meshInfo = settings.info.intersect(meshInfo);

    PlyHeader header(
        settings.binary ? ply::BINARY_LITTLE_ENDIAN : ply::ASCII, meshInfo);
    header.setVertexCount(m.vertexCount());

    if constexpr (HasFaces<MeshType>) {
        if (header.hasFaces()) {
            header.setFaceCount(m.faceCount());
        }
    }
    if constexpr (HasEdges<MeshType>) {
        if (header.hasEdges()) {
            header.setEdgeCount(m.edgeCount());
        }
    }
    if constexpr (HasMaterials<MeshType>) {
        if (header.hasMaterials()) {
            header.setMaterialCount(m.materialCount());
        }
    }

    // When meshlabCompatibility is enabled, we intentionally add legacy texture
    // information to the header (via addTexturesToHeader) in addition to the
    // newer material element written later. This redundancy maximizes
    // compatibility with tools that only understand one of the two formats.
    if (settings.meshlabCompatibility) {
        addTexturesToHeader(header, m);
    }

    // this should never happen
    if (!header.isValid())
        throw std::runtime_error("Ply Header not valid.");

    fp << header.toString(settings);

    writePlyVertices(fp, header, m);

    if constexpr (HasFaces<MeshType>) {
        if (header.hasFaces()) {
            writePlyFaces(fp, header, m);
        }
    }

    if constexpr (HasEdges<MeshType>) {
        if (header.hasEdges()) {
            writePlyEdges(fp, header, m);
        }
    }

    if constexpr (HasMaterials<MeshType>) {
        if (header.hasMaterials()) {
            writePlyMaterials(fp, header, m);
        }
        if (settings.saveTextureImages) {
            saveTextureImages(m, fileBasePath, BitSet8::ALL(), log);
        }
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    std::ofstream fp = openOutputFileStream(filename, "ply");

    std::string basePath = FileInfo::pathWithoutFileName(filename);

    savePly(m, fp, basePath, settings, log);
}

} // namespace vcl

#endif // VCL_IO_MESH_PLY_SAVE_H
