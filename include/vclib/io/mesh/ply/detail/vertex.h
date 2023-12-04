/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_PLY_VERTEX_H
#define VCL_PLY_VERTEX_H

#include <fstream>

#include <vclib/concepts/mesh/per_vertex.h>
#include <vclib/exceptions/io_exceptions.h>
#include <vclib/io/read.h>
#include <vclib/io/write.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/tokenizer.h>

#include "header.h"

namespace vcl::detail {

template<MeshConcept MeshType, VertexConcept VertexType, typename Stream>
void readPlyVertexProperty(
    Stream&     file,
    MeshType&   mesh,
    VertexType& v,
    PlyProperty p)
{
    bool hasBeenRead = false;
    if (p.name >= ply::x && p.name <= ply::z) {
        using Scalar = VertexType::CoordType::ScalarType;
        int a        = p.name - ply::x;
        v.coord()[a] = io::readPrimitiveType<Scalar>(file, p.type);
        hasBeenRead  = true;
    }
    if (p.name >= ply::nx && p.name <= ply::nz) {
        if constexpr (vcl::HasPerVertexNormal<MeshType>) {
            if (vcl::isPerVertexNormalAvailable(mesh)) {
                using Scalar  = VertexType::NormalType::ScalarType;
                int a         = p.name - ply::nx;
                v.normal()[a] = io::readPrimitiveType<Scalar>(file, p.type);
                hasBeenRead   = true;
            }
        }
    }
    if (p.name >= ply::red && p.name <= ply::alpha) {
        if constexpr (vcl::HasPerVertexColor<MeshType>) {
            if (vcl::isPerVertexColorAvailable(mesh)) {
                int a = p.name - ply::red;
                v.color()[a] =
                    io::readPrimitiveType<unsigned char>(file, p.type);
                hasBeenRead = true;
            }
        }
    }
    if (p.name == ply::quality) {
        if constexpr (vcl::HasPerVertexQuality<MeshType>) {
            using QualityType = VertexType::QualityType;
            if (vcl::isPerVertexQualityAvailable(mesh)) {
                v.quality() = io::readPrimitiveType<QualityType>(file, p.type);
                hasBeenRead = true;
            }
        }
    }
    if (p.name >= ply::texture_u && p.name <= ply::texture_v) {
        if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
            using Scalar = VertexType::TexCoordType::ScalarType;
            if (vcl::isPerVertexTexCoordAvailable(mesh)) {
                int a           = p.name - ply::texture_u;
                v.texCoord()[a] = io::readPrimitiveType<Scalar>(file, p.type);
                hasBeenRead     = true;
            }
        }
    }
    if (p.name == ply::unknown) {
        if constexpr (vcl::HasPerVertexCustomComponents<MeshType>) {
            if (mesh.hasPerVertexCustomComponent(p.unknownPropertyName)) {
                io::readCustomComponent(file, v, p.unknownPropertyName, p.type);
                hasBeenRead = true;
            }
        }
    }
    if (!hasBeenRead) {
        if (p.list) {
            uint s = io::readPrimitiveType<int>(file, p.listSizeType);
            for (uint i = 0; i < s; ++i)
                io::readPrimitiveType<int>(file, p.type);
        }
        else {
            io::readPrimitiveType<int>(file, p.type);
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
    vcl::Tokenizer spaceTokenizer  = readAndTokenizeNextNonEmptyLine(file);
    vcl::Tokenizer::iterator token = spaceTokenizer.begin();
    for (const PlyProperty& p : vertexProperties) {
        if (token == spaceTokenizer.end()) {
            throw vcl::MalformedFileException("Unexpected end of line.");
        }
        readPlyVertexProperty(token, mesh, v, p);
    }
}

template<VertexConcept VertexType, MeshConcept MeshType>
void readPlyVertexBin(
    std::istream&                 file,
    VertexType&                   v,
    MeshType&                     mesh,
    const std::list<PlyProperty>& vertexProperties)
{
    for (const PlyProperty& p : vertexProperties) {
        readPlyVertexProperty(file, mesh, v, p);
    }
}

template<MeshConcept MeshType>
void writePlyVertices(
    std::ofstream&   file,
    const PlyHeader& header,
    const MeshType&  mesh)
{
    using VertexType = MeshType::VertexType;

    bool bin = header.format() == ply::BINARY;
    for (const VertexType& v : mesh.vertices()) {
        for (const PlyProperty& p : header.vertexProperties()) {
            bool hasBeenWritten = false;
            if (p.name >= ply::x && p.name <= ply::z) {
                io::writeProperty(
                    file, v.coord()[p.name - ply::x], p.type, bin);
                hasBeenWritten = true;
            }
            if (p.name >= ply::nx && p.name <= ply::nz) {
                if constexpr (vcl::HasPerVertexNormal<MeshType>) {
                    io::writeProperty(
                        file, v.normal()[p.name - ply::nx], p.type, bin);
                    hasBeenWritten = true;
                }
            }
            if (p.name >= ply::red && p.name <= ply::alpha) {
                if constexpr (vcl::HasPerVertexColor<MeshType>) {
                    io::writeProperty(
                        file, v.color()[p.name - ply::red], p.type, bin);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::quality) {
                if constexpr (vcl::HasPerVertexQuality<MeshType>) {
                    io::writeProperty(file, v.quality(), p.type, bin);
                    hasBeenWritten = true;
                }
            }
            if (p.name >= ply::texture_u && p.name <= ply::texture_v) {
                if constexpr (vcl::HasPerVertexTexCoord<MeshType>) {
                    const uint a = p.name - ply::texture_u;
                    io::writeProperty(file, v.texCoord()[a], p.type, bin);
                    hasBeenWritten = true;
                }
            }
            if (p.name == ply::unknown) {
                if constexpr (vcl::HasPerVertexCustomComponents<MeshType>) {
                    if (mesh.hasPerVertexCustomComponent(p.unknownPropertyName))
                    {
                        io::writeCustomComponent(
                            file, v, p.unknownPropertyName, p.type, bin);
                        hasBeenWritten = true;
                    }
                }
            }
            if (!hasBeenWritten) {
                // be sure to write something if the header declares some
                // property that is not in the mesh
                io::writeProperty(file, 0, p.type, bin);
            }
        }
        if (!bin)
            file << std::endl;
    }
}

template<MeshConcept MeshType>
void readPlyVertices(std::istream& file, const PlyHeader& header, MeshType& m)
{
    m.addVertices(header.numberVertices());

    for (uint vid = 0; vid < header.numberVertices(); ++vid) {
        auto& v = m.vertex(vid);
        if (header.format() == ply::ASCII) {
            detail::readPlyVertexTxt(file, v, m, header.vertexProperties());
        }
        else if (header.format() == ply::BINARY) {
            detail::readPlyVertexBin(file, v, m, header.vertexProperties());
        }
    }
}

} // namespace vcl::detail

#endif // VCL_PLY_VERTEX_H
