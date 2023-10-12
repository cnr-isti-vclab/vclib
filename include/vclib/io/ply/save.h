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

#ifndef VCL_IO_PLY_SAVE_H
#define VCL_IO_PLY_SAVE_H

#include <vclib/exceptions/io_exceptions.h>
#include <vclib/misc/logger.h>

#include "ply.h"
#include "ply_edge.h"
#include "ply_extra.h"
#include "ply_face.h"
#include "ply_vertex.h"
#include "../detail/io_utils.h"

namespace vcl::io {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&    m,
    const std::string& filename,
    const MeshInfo&    info,
    LogType&           log    = nullLogger,
    bool               binary = true)
{
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    meshInfo = info.intersect(meshInfo);

    ply::PlyHeader header(binary ? ply::BINARY : ply::ASCII, meshInfo);
    header.setNumberVertices(m.vertexNumber());

    if constexpr (vcl::HasFaces<MeshType>) {
        if (header.hasFaces()) {
            header.setNumberFaces(m.faceNumber());
        }
    }
    if constexpr (vcl::HasEdges<MeshType>) {
        if (header.hasEdges()) {
            header.setNumberEdges(m.edgeNumber());
        }
    }
    ply::saveTextures(header, m);

    if (!header.isValid())
        throw std::runtime_error("Ply Header not valid.");

    std::ofstream fp = detail::saveFileStream(filename, "ply");

    fp << header.toString();

    ply::saveVertices(fp, header, m);

    if constexpr (vcl::HasFaces<MeshType>) {
        if (header.hasFaces()) {
            ply::saveFaces(fp, header, m);
        }
    }

    if constexpr (vcl::HasEdges<MeshType>) {
        if (header.hasEdges()) {
            ply::saveEdges(fp, header, m);
        }
    }

    fp.close();
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&    m,
    const std::string& filename,
    LogType&           log    = nullLogger,
    bool               binary = true)
{
    MeshInfo info(m);
    savePly(m, filename, info, log, binary);
}

} // namespace vcl::io

#endif // VCL_IO_PLY_SAVE_H
