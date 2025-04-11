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

#ifndef VCL_IO_MESH_PLY_DETAIL_TRISTRIP_H
#define VCL_IO_MESH_PLY_DETAIL_TRISTRIP_H

#include "header.h"

#include <vclib/io/read.h>
#include <vclib/misc/tokenizer.h>

#include <istream>

namespace vcl::detail {

template<FaceMeshConcept MeshType>
void facesFromPlyTriStrip(MeshType& m, const std::vector<int>& tristrip)
{
    using FaceType = MeshType::FaceType;

    bool firstOddPos = false;
    for (uint k = 0; k < tristrip.size() - 2; ++k) {
        if (tristrip[k + 2] < 0) {
            k += 2;
            if (k % 2 == 0)
                firstOddPos = false;
            else
                firstOddPos = true;
        }
        else {
            uint      fid = m.addFace();
            FaceType& f   = m.face(fid);
            if constexpr (FaceType::VERTEX_NUMBER < 0) {
                f.resizeVertices(3);
            }
            for (uint i = 0; i < f.vertexNumber(); ++i) {
                f.setVertex(i, tristrip[k + i]);
            }

            if (k % 2 == 0 != firstOddPos) {
                auto* tmp = f.vertex(0);
                f.setVertex(0U, f.vertex(1));
                f.setVertex(1U, tmp);
            }
        }
    }
}

template<FaceMeshConcept MeshType, LoggerConcept LogType>
void readPlyTriStripsTxt(
    std::istream&    file,
    const PlyHeader& header,
    MeshType&        m,
    LogType&         log)
{
    log.startProgress("Reading Triangle Strips", header.numberTriStrips());

    for (uint tid = 0; tid < header.numberTriStrips(); ++tid) {
        Tokenizer spaceTokenizer  = readAndTokenizeNextNonEmptyLine(file);
        Tokenizer::iterator token = spaceTokenizer.begin();
        for (const PlyProperty& p : header.triStripsProperties()) {
            if (token == spaceTokenizer.end()) {
                throw MalformedFileException("Unexpected end of line.");
            }
            bool hasBeenRead = false;
            if (p.name == ply::vertex_indices) {
                uint tSize = io::readPrimitiveType<uint>(token, p.listSizeType);
                std::vector<int> tristrip(tSize);
                for (uint i = 0; i < tSize; ++i) {
                    tristrip[i] = io::readPrimitiveType<size_t>(token, p.type);
                }
                hasBeenRead = true;
                facesFromPlyTriStrip(m, tristrip);
            }
            if (!hasBeenRead) {
                if (p.list) {
                    uint s = io::readPrimitiveType<int>(token, p.listSizeType);
                    for (uint i = 0; i < s; ++i) {
                        ++token;
                    }
                }
                else {
                    ++token;
                }
            }
        }
        log.progress(tid);
    }
    log.endProgress();
}

template<FaceMeshConcept MeshType, LoggerConcept LogType>
void readPlyTriStripsBin(
    std::istream&    file,
    const PlyHeader& header,
    MeshType&        m,
    std::endian      end,
    LogType&         log)
{
    log.startProgress("Reading Triangle Strips", header.numberTriStrips());
    for (uint tid = 0; tid < header.numberTriStrips(); ++tid) {
        for (const PlyProperty& p : header.triStripsProperties()) {
            bool hasBeenRead = false;
            if (p.name == ply::vertex_indices) {
                uint tSize =
                    io::readPrimitiveType<uint>(file, p.listSizeType, end);
                std::vector<int> tristrip(tSize);
                for (uint i = 0; i < tSize; ++i)
                    tristrip[i] = io::readPrimitiveType<int>(file, p.type, end);
                hasBeenRead = true;
                facesFromPlyTriStrip(m, tristrip);
            }
            if (!hasBeenRead) {
                if (p.list) {
                    uint s =
                        io::readPrimitiveType<int>(file, p.listSizeType, end);
                    for (uint i = 0; i < s; ++i)
                        io::readPrimitiveType<int>(file, p.type, end);
                }
                else {
                    io::readPrimitiveType<int>(file, p.type, end);
                }
            }
        }
        log.progress(tid);
    }
    log.endProgress();
}

template<FaceMeshConcept MeshType, LoggerConcept LogType>
void readPlyTriStrips(
    std::istream&    file,
    const PlyHeader& header,
    MeshType&        mesh,
    LogType&         log)
{
    if (header.format() == ply::ASCII) {
        detail::readPlyTriStripsTxt(file, header, mesh, log);
    }
    else {
        std::endian end = header.format() == ply::BINARY_BIG_ENDIAN ?
                              std::endian::big :
                              std::endian::little;
        detail::readPlyTriStripsBin(file, header, mesh, end, log);
    }
}

} // namespace vcl::detail

#endif // VCL_IO_MESH_PLY_DETAIL_TRISTRIP_H
