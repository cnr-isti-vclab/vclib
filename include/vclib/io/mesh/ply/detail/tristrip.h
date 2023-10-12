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

#ifndef VCL_IO_PLY_TRISTRIP_H
#define VCL_IO_PLY_TRISTRIP_H

#include <fstream>

#include <vclib/io/read.h>
#include <vclib/misc/tokenizer.h>

#include "header.h"

namespace vcl::detail {

template<FaceMeshConcept MeshType>
void facesFromPlyTriStrip(MeshType& m, const std::vector<int>& tristrip)
{
    using FaceType   = MeshType::FaceType;

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
            uint i = 0;
            for (auto& v : f.vertices()) {
                v = &m.vertex(tristrip[k + i]);
                ++i;
            }
            if (k % 2 == 0 != firstOddPos)
                std::swap(f.vertex(0), f.vertex(1));
        }
    }
}

template<FaceMeshConcept MeshType>
void readPlyTriStripsTxt(std::ifstream& file, const PlyHeader& header, MeshType& m)
{
    for (uint tid = 0; tid < header.numberTriStrips(); ++tid) {
        vcl::Tokenizer spaceTokenizer  = readAndTokenizeNextNonEmptyLine(file);
        vcl::Tokenizer::iterator token = spaceTokenizer.begin();
        for (const PlyProperty& p : header.triStripsProperties()) {
            if (token == spaceTokenizer.end()) {
                throw vcl::MalformedFileException("Unexpected end of line.");
            }
            bool hasBeenRead = false;
            if (p.name == ply::vertex_indices) {
                uint tSize =
                    io::readProperty<uint>(token, p.listSizeType);
                std::vector<int> tristrip(tSize);
                for (uint i = 0; i < tSize; ++i) {
                    tristrip[i] =
                        io::readProperty<size_t>(token, p.type);
                }
                hasBeenRead = true;
                facesFromPlyTriStrip(m, tristrip);
            }
            if (!hasBeenRead) {
                if (p.list) {
                    uint s =
                        io::readProperty<int>(token, p.listSizeType);
                    for (uint i = 0; i < s; ++i) {
                        ++token;
                    }
                }
                else {
                    ++token;
                }
            }
        }
    }
}

template<FaceMeshConcept MeshType>
void readPlyTriStripsBin(std::ifstream& file, const PlyHeader& header, MeshType& m)
{
    for (uint tid = 0; tid < header.numberTriStrips(); ++tid) {
        for (const PlyProperty& p : header.triStripsProperties()) {
            bool hasBeenRead = false;
            if (p.name == ply::vertex_indices) {
                uint tSize =
                    io::readProperty<uint>(file, p.listSizeType);
                std::vector<int> tristrip(tSize);
                for (uint i = 0; i < tSize; ++i)
                    tristrip[i] = io::readProperty<int>(file, p.type);
                hasBeenRead = true;
                facesFromPlyTriStrip(m, tristrip);
            }
            if (!hasBeenRead) {
                if (p.list) {
                    uint s =
                        io::readProperty<int>(file, p.listSizeType);
                    for (uint i = 0; i < s; ++i)
                        io::readProperty<int>(file, p.type);
                }
                else {
                    io::readProperty<int>(file, p.type);
                }
            }
        }
    }
}

template<FaceMeshConcept MeshType>
void readPlyTriStrips(std::ifstream& file, const PlyHeader& header, MeshType& mesh)
{
    if (header.format() == ply::ASCII) {
        detail::readPlyTriStripsTxt(file, header, mesh);
    }
    else if (header.format() == ply::BINARY) {
        detail::readPlyTriStripsBin(file, header, mesh);
    }
}

} // namespace vcl::ply

#endif // VCL_IO_PLY_TRISTRIP_H
