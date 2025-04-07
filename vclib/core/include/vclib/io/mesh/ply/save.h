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

#ifndef VCL_IO_MESH_PLY_SAVE_H
#define VCL_IO_MESH_PLY_SAVE_H

#include "detail/edge.h"
#include "detail/extra.h"
#include "detail/face.h"
#include "detail/vertex.h"

#include <vclib/exceptions/io.h>
#include <vclib/io/mesh/settings.h>
#include <vclib/misc/logger.h>

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&     m,
    std::ostream&       fp,
    LogType&            log      = nullLogger,
    const SaveSettings& settings = SaveSettings())
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
    header.setNumberVertices(m.vertexNumber());

    if constexpr (HasFaces<MeshType>) {
        if (header.hasFaces()) {
            header.setNumberFaces(m.faceNumber());
        }
    }
    if constexpr (HasEdges<MeshType>) {
        if (header.hasEdges()) {
            header.setNumberEdges(m.edgeNumber());
        }
    }
    writePlyTextures(header, m, log, settings);

    // this should never happen
    if (!header.isValid())
        throw std::runtime_error("Ply Header not valid.");

    fp << header.toString();

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
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&     m,
    std::ostream&       fp,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    savePly(m, fp, log, settings);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&     m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const SaveSettings& settings = SaveSettings())
{
    std::ofstream fp = openOutputFileStream(filename, "ply");

    savePly(m, fp, log, settings);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void savePly(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    savePly(m, filename, log, settings);
}

} // namespace vcl

#endif // VCL_IO_MESH_PLY_SAVE_H
