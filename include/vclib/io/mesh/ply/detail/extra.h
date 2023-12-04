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

#ifndef VCL_IO_PLY_EXTRA_H
#define VCL_IO_PLY_EXTRA_H

#include <fstream>

#include <vclib/io/read.h>
#include <vclib/mesh/requirements.h>

#include "header.h"

namespace vcl::detail {

template<MeshConcept MeshType>
void readPlyTextures(const PlyHeader& header, MeshType& mesh)
{
    if constexpr (vcl::HasTexturePaths<MeshType>) {
        for (const std::string& str : header.textureFileNames()) {
            mesh.pushTexturePath(str);
        }
    }
}

template<MeshConcept MeshType>
void writePlyTextures(PlyHeader& header, const MeshType& mesh)
{
    if constexpr (vcl::HasTexturePaths<MeshType>) {
        for (const std::string& str : mesh.texturePaths()) {
            header.pushTextureFileName(str);
        }
    }
}

inline void readPlyUnknownElement(
    std::istream&    file,
    const PlyHeader& header,
    PlyElement       el)
{
    if (header.format() == ply::ASCII) {
        for (uint i = 0; i < el.numberElements; ++i) {
            readAndTokenizeNextNonEmptyLine(file);
        }
    }
    else {
        for (uint i = 0; i < el.numberElements; ++i) {
            for (const PlyProperty& p : el.properties) {
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
    }
}

} // namespace vcl::detail

#endif // VCL_IO_PLY_EXTRA_H
