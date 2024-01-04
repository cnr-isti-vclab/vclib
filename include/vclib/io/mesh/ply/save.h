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

#include "detail/edge.h"
#include "detail/extra.h"
#include "detail/face.h"
#include "detail/vertex.h"

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType& m,
    std::ostream&   fp,
    const MeshInfo& info,
    LogType&        log    = nullLogger,
    bool            binary = true)
{
    using namespace detail;
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    meshInfo = info.intersect(meshInfo);

    PlyHeader header(binary ? ply::BINARY : ply::ASCII, meshInfo);
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
    writePlyTextures(header, m);

    // this should never happen
    if (!header.isValid())
        throw std::runtime_error("Ply Header not valid.");

    fp << header.toString();

    writePlyVertices(fp, header, m);

    if constexpr (vcl::HasFaces<MeshType>) {
        if (header.hasFaces()) {
            writePlyFaces(fp, header, m);
        }
    }

    if constexpr (vcl::HasEdges<MeshType>) {
        if (header.hasEdges()) {
            writePlyEdges(fp, header, m);
        }
    }
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType& m,
    std::ostream&   fp,
    const MeshInfo& info,
    bool            binary,
    LogType&        log = nullLogger)
{
    savePly(m, fp, info, log, binary);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType& m,
    std::ostream&   fp,
    bool            binary,
    LogType&        log = nullLogger)
{
    MeshInfo info(m);
    savePly(m, fp, info, log, binary);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType& m,
    std::ostream&   fp,
    LogType&        log    = nullLogger,
    bool            binary = true)
{
    MeshInfo info(m);
    savePly(m, fp, info, log, binary);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&    m,
    const std::string& filename,
    const MeshInfo&    info,
    LogType&           log    = nullLogger,
    bool               binary = true)
{
    std::ofstream fp = openOutputFileStream(filename, "ply");

    savePly(m, fp, info, log, binary);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&    m,
    const std::string& filename,
    const MeshInfo&    info,
    bool               binary,
    LogType&           log = nullLogger)
{
    savePly(m, filename, info, log, binary);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&    m,
    const std::string& filename,
    bool               binary,
    LogType&           log = nullLogger)
{
    MeshInfo info(m);
    savePly(m, filename, info, log, binary);
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

} // namespace vcl

#endif // VCL_IO_PLY_SAVE_H
