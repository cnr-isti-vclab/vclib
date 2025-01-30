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

#ifndef VCL_LOAD_SAVE_PLY_DETAIL_VERTEX_H
#define VCL_LOAD_SAVE_PLY_DETAIL_VERTEX_H

#include "header.h"

#include <vclib/concepts/mesh/per_vertex.h>
#include <vclib/exceptions/io.h>
#include <vclib/io/read.h>
#include <vclib/io/write.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/tokenizer.h>

namespace vcl::detail {

template<MeshConcept MeshType, VertexConcept VertexType, typename Stream>
void readPlyVertexProperty(
    Stream&     file,
    MeshType&   mesh,
    VertexType& v,
    PlyProperty p,
    std::endian end = std::endian::little)
{
    bool hasBeenRead = false;
    if (p.name >= ply::x && p.name <= ply::z) {
        using Scalar = VertexType::CoordType::ScalarType;
        int a        = p.name - ply::x;
        v.coord()[a] = io::readPrimitiveType<Scalar>(file, p.type, end);
        hasBeenRead  = true;
    }
    if (p.name >= ply::nx && p.name <= ply::nz) {
        if constexpr (HasPerVertexNormal<MeshType>) {
            if (isPerVertexNormalAvailable(mesh)) {
                using Scalar = VertexType::NormalType::ScalarType;
                int a        = p.name - ply::nx;
                v.normal()[a] =
                    io::readPrimitiveType<Scalar>(file, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    if (p.name >= ply::red && p.name <= ply::alpha) {
        if constexpr (HasPerVertexColor<MeshType>) {
            if (isPerVertexColorAvailable(mesh)) {
                int a = p.name - ply::red;
                v.color()[a] =
                    io::readPrimitiveType<unsigned char>(file, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    if (p.name == ply::quality) {
        if constexpr (HasPerVertexQuality<MeshType>) {
            using QualityType = VertexType::QualityType;
            if (isPerVertexQualityAvailable(mesh)) {
                v.quality() =
                    io::readPrimitiveType<QualityType>(file, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    if (p.name >= ply::texture_u && p.name <= ply::texture_v) {
        if constexpr (HasPerVertexTexCoord<MeshType>) {
            using Scalar = VertexType::TexCoordType::ScalarType;
            if (isPerVertexTexCoordAvailable(mesh)) {
                int a = p.name - ply::texture_u;
                v.texCoord()[a] =
                    io::readPrimitiveType<Scalar>(file, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    if (p.name == ply::texnumber) {
        if constexpr (HasPerVertexTexCoord<MeshType>) {
            if (isPerVertexTexCoordAvailable(mesh)) {
                v.texCoord().index() =
                    io::readPrimitiveType<ushort>(file, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    if (p.name == ply::unknown) {
        if constexpr (HasPerVertexCustomComponents<MeshType>) {
            if (mesh.hasPerVertexCustomComponent(p.unknownPropertyName)) {
                io::readCustomComponent(
                    file, v, p.unknownPropertyName, p.type, end);
                hasBeenRead = true;
            }
        }
    }
    if (!hasBeenRead) {
        if (p.list) {
            uint s = io::readPrimitiveType<int>(file, p.listSizeType);
            for (uint i = 0; i < s; ++i)
                io::readPrimitiveType<int>(file, p.type, end);
        }
        else {
            io::readPrimitiveType<int>(file, p.type, end);
        }
    }
}

template<VertexConcept VertexType, MeshConcept MeshType>
void readPlyVertexTxt(
    std::istream&                 file,
    VertexType&                   v,
    MeshType&                     mesh,
    const std::list<PlyProperty>& vertexProperties)
{
    Tokenizer           spaceTokenizer = readAndTokenizeNextNonEmptyLine(file);
    Tokenizer::iterator token          = spaceTokenizer.begin();
    for (const PlyProperty& p : vertexProperties) {
        if (token == spaceTokenizer.end()) {
            throw MalformedFileException("Unexpected end of line.");
        }
        readPlyVertexProperty(token, mesh, v, p);
    }
}

template<VertexConcept VertexType, MeshConcept MeshType>
void readPlyVertexBin(
    std::istream&                 file,
    VertexType&                   v,
    MeshType&                     mesh,
    const std::list<PlyProperty>& vertexProperties,
    std::endian                   end)
{
    for (const PlyProperty& p : vertexProperties) {
        readPlyVertexProperty(file, mesh, v, p, end);
    }
}

template<MeshConcept MeshType>
void writePlyVertices(
    std::ostream&    file,
    const PlyHeader& header,
    const MeshType&  mesh)
{
    using VertexType = MeshType::VertexType;

    FileType format;
    if (header.format() == ply::ASCII) {
        format.isBinary = false;
    }
    else if (header.format() == ply::BINARY_BIG_ENDIAN) {
        format.endian = std::endian::big;
    }

    for (const VertexType& v : mesh.vertices()) {
        for (const PlyProperty& p : header.vertexProperties()) {
            bool hasBeenWritten = false;
            if (p.name >= ply::x && p.name <= ply::z) {
                io::writeProperty(
                    file, v.coord()[p.name - ply::x], p.type, format);
                hasBeenWritten = true;
            }
            if (p.name >= ply::nx && p.name <= ply::nz) {
                if constexpr (HasPerVertexNormal<MeshType>) {
                    io::writeProperty(
                        file, v.normal()[p.name - ply::nx], p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name >= ply::red && p.name <= ply::alpha) {
                if constexpr (HasPerVertexColor<MeshType>) {
                    io::writeProperty(
                        file, v.color()[p.name - ply::red], p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::quality) {
                if constexpr (HasPerVertexQuality<MeshType>) {
                    io::writeProperty(file, v.quality(), p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name >= ply::texture_u && p.name <= ply::texture_v) {
                if constexpr (HasPerVertexTexCoord<MeshType>) {
                    const uint a = p.name - ply::texture_u;
                    io::writeProperty(file, v.texCoord()[a], p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::texnumber) {
                if constexpr (HasPerVertexTexCoord<MeshType>) {
                    io::writeProperty(
                        file, v.texCoord().index(), p.type, format);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::unknown) {
                if constexpr (HasPerVertexCustomComponents<MeshType>) {
                    if (mesh.hasPerVertexCustomComponent(
                            p.unknownPropertyName)) {
                        io::writeCustomComponent(
                            file, v, p.unknownPropertyName, p.type, format);
                        hasBeenWritten = true;
                    }
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

template<MeshConcept MeshType, LoggerConcept LogType>
void readPlyVertices(
    std::istream&    file,
    const PlyHeader& header,
    MeshType&        m,
    LogType&         log)
{
    m.addVertices(header.numberVertices());

    log.startProgress("Reading vertices", header.numberVertices());

    for (uint vid = 0; vid < header.numberVertices(); ++vid) {
        auto& v = m.vertex(vid);
        if (header.format() == ply::ASCII) {
            detail::readPlyVertexTxt(file, v, m, header.vertexProperties());
        }
        else {
            std::endian end = header.format() == ply::BINARY_BIG_ENDIAN ?
                                  std::endian::big :
                                  std::endian::little;
            detail::readPlyVertexBin(
                file, v, m, header.vertexProperties(), end);
        }
        log.progress(vid);
    }
    log.endProgress();
}

} // namespace vcl::detail

#endif // VCL_LOAD_SAVE_PLY_DETAIL_VERTEX_H
