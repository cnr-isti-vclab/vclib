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

#ifndef VCL_LOAD_SAVE_PLY_DETAIL_EDGE_H
#define VCL_LOAD_SAVE_PLY_DETAIL_EDGE_H

#include "header.h"

#include <vclib/io/write.h>
#include <vclib/misc/tokenizer.h>

namespace vcl::detail {

template<EdgeMeshConcept MeshType, EdgeConcept EdgeType, typename Stream>
void readPlyEdgeProperty(
    Stream&     file,
    MeshType&   mesh,
    EdgeType&   e,
    PlyProperty p,
    std::endian end = std::endian::little)
{
    bool hasBeenRead = false;
    // loading vertex indices in case of list
    if (p.name == ply::vertex_indices) {
        std::vector<uint> vids; // contains the vertex ids of the actual edge
        uint eSize = io::readPrimitiveType<uint>(file, p.listSizeType, end);
        vids.resize(eSize);
        for (uint i = 0; i < eSize; ++i) {
            vids[i] = io::readPrimitiveType<size_t>(file, p.type, end);
            if (vids[i] >= mesh.vertexNumber()) {
                throw MalformedFileException(
                    "Bad vertex index for edge " + std::to_string(e.index()));
            }
        }
        hasBeenRead = true;
        e.setVertex(0u, vids[0]);
        e.setVertex(1u, vids[1]);
    }
    if (p.name == ply::vertex1) { // loading vertex1 index
        uint v0     = io::readPrimitiveType<uint>(file, p.type, end);
        hasBeenRead = true;
        e.setVertex(0u, v0);
    }
    if (p.name == ply::vertex2) { // loading vertex2 index
        uint v1     = io::readPrimitiveType<uint>(file, p.type, end);
        hasBeenRead = true;
        e.setVertex(1u, v1);
    }
    // loading one of the normal components
    if (p.name >= ply::nx && p.name <= ply::nz) {
        if constexpr (HasPerEdgeNormal<MeshType>) {
            if (isPerEdgeNormalAvailable(mesh)) {
                using Scalar = EdgeType::NormalType::ScalarType;
                int    a     = p.name - ply::nx;
                Scalar n     = io::readPrimitiveType<Scalar>(file, p.type, end);
                hasBeenRead  = true;
            }
        }
    }
    // loading one of the color components
    if (p.name >= ply::red && p.name <= ply::alpha) {
        if constexpr (HasPerFaceColor<MeshType>) {
            if (isPerFaceColorAvailable(mesh)) {
                int           a = p.name - ply::red;
                unsigned char c =
                    io::readPrimitiveType<unsigned char>(file, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    if (p.name == ply::quality) { // loading the quality component
        if constexpr (HasPerFaceQuality<MeshType>) {
            using QualityType = EdgeType::QualityType;
            if (isPerFaceQualityAvailable(mesh)) {
                QualityType s =
                    io::readPrimitiveType<QualityType>(file, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    if (p.name == ply::unknown) {
        if constexpr (HasPerFaceCustomComponents<MeshType>) {
            if (mesh.hasPerFaceCustomComponent(p.unknownPropertyName)) {
                io::readCustomComponent(
                    file, e, p.unknownPropertyName, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    // if nothing has been read, it means that there is some data we don't know
    // we still need to read and discard what we read
    if (!hasBeenRead) {
        if (p.list) {
            uint s = io::readPrimitiveType<int>(file, p.listSizeType, end);
            for (uint i = 0; i < s; ++i)
                io::readPrimitiveType<int>(file, p.type, end);
        }
        else {
            io::readPrimitiveType<int>(file, p.type, end);
        }
    }
}

template<EdgeConcept EdgeType, MeshConcept MeshType>
void readPlyEdgeTxt(
    std::istream&                 file,
    EdgeType&                     e,
    MeshType&                     mesh,
    const std::list<PlyProperty>& edgeProperties)
{
    Tokenizer           spaceTokenizer = readAndTokenizeNextNonEmptyLine(file);
    Tokenizer::iterator token          = spaceTokenizer.begin();
    for (const PlyProperty& p : edgeProperties) {
        if (token == spaceTokenizer.end()) {
            throw MalformedFileException("Unexpected end of line.");
        }
        readPlyEdgeProperty(token, mesh, e, p);
    }
}

template<EdgeConcept EdgeType, MeshConcept MeshType>
void readPlyEdgeBin(
    std::istream&                 file,
    EdgeType&                     e,
    MeshType&                     mesh,
    const std::list<PlyProperty>& edgeProperties,
    std::endian                   end)
{
    for (const PlyProperty& p : edgeProperties) {
        readPlyEdgeProperty(file, mesh, e, p, end);
    }
}

template<EdgeMeshConcept MeshType>
void writePlyEdges(
    std::ostream&    file,
    const PlyHeader& header,
    const MeshType&  mesh)
{
    using EdgeType = MeshType::EdgeType;

    FileType format;
    if (header.format() == ply::ASCII) {
        format.isBinary = false;
    }
    else if (header.format() == ply::BINARY_BIG_ENDIAN) {
        format.endian = std::endian::big;
    }

    // indices of vertices that do not consider deleted vertices
    std::vector<uint> vIndices = mesh.vertexCompactIndices();

    for (const EdgeType& e : mesh.edges()) {
        for (const PlyProperty& p : header.edgeProperties()) {
            bool hasBeenWritten = false;
            if (p.name == ply::vertex1) {
                io::writeProperty(
                    file, vIndices[mesh.index(e.vertex(0))], p.type, format);
                hasBeenWritten = true;
            }
            if (p.name == ply::vertex2) {
                io::writeProperty(
                    file, vIndices[mesh.index(e.vertex(1))], p.type, format);
                hasBeenWritten = true;
            }
            if (p.name >= ply::nx && p.name <= ply::nz) {
                if constexpr (HasPerEdgeNormal<MeshType>) {
                    io::writeProperty(
                        file, e.normal()[p.name - ply::nx], p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name >= ply::red && p.name <= ply::alpha) {
                if constexpr (HasPerEdgeColor<MeshType>) {
                    io::writeProperty(
                        file, e.color()[p.name - ply::red], p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::quality) {
                if constexpr (HasPerEdgeQuality<MeshType>) {
                    io::writeProperty(file, e.quality(), p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (!hasBeenWritten) {
                // be sure to write something if the header declares some
                // property that is not in the mesh
                io::writeProperty(file, 0, p.type, format);
            }
        }
        if (!format.isBinary)
            file << std::endl;
    }
}

template<EdgeMeshConcept MeshType, LoggerConcept LogType>
void readPlyEdges(
    std::istream&    file,
    const PlyHeader& header,
    MeshType&        m,
    LogType&         log)
{
    using EdgeType = MeshType::EdgeType;
    m.reserveEdges(header.numberEdges());

    log.startProgress("Reading edges", header.numberEdges());

    for (uint eid = 0; eid < header.numberEdges(); ++eid) {
        uint      eeid = m.addEdge();
        EdgeType& e    = m.edge(eeid);
        if (header.format() == ply::ASCII) {
            detail::readPlyEdgeTxt(file, e, m, header.edgeProperties());
        }
        else {
            std::endian end = header.format() == ply::BINARY_BIG_ENDIAN ?
                                  std::endian::big :
                                  std::endian::little;
            detail::readPlyEdgeBin(file, e, m, header.edgeProperties(), end);
        }

        log.progress(eid);
    }

    log.endProgress();
}

} // namespace vcl::detail

#endif // VCL_LOAD_SAVE_PLY_DETAIL_EDGE_H
