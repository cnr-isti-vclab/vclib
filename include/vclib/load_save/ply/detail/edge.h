/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/io/write.h>
#include <vclib/misc/tokenizer.h>

#include "header.h"

namespace vcl::detail {

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
            if (!hasBeenWritten) {
                // be sure to write something if the header declares some
                // property that is not in the mesh
                io::writeProperty(file, 0, p.type, format);
            }
        }
    }
}

} // namespace vcl::detail

#endif // VCL_LOAD_SAVE_PLY_DETAIL_EDGE_H
