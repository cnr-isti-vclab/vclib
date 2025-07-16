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

#ifndef VCL_IO_MESH_PLY_LOAD_H
#define VCL_IO_MESH_PLY_LOAD_H

#include "detail/edge.h"
#include "detail/extra.h"
#include "detail/face.h"
#include "detail/tristrip.h"
#include "detail/vertex.h"

#include <vclib/io/mesh/settings.h>
#include <vclib/misc/logger.h>
#include <vclib/space/complex/mesh_info.h>

namespace vcl {

namespace detail {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&           m,
    std::istream&       file,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    const LoadSettings& settings = LoadSettings(),
    LogType&            log      = nullLogger)
{
    PlyHeader header(file, filename);
    if (header.errorWhileLoading())
        throw MalformedFileException("Header not valid: " + filename);

    m.clear();

    loadedInfo = header.getInfo();

    if (settings.enableOptionalComponents)
        enableOptionalComponentsFromInfo(loadedInfo, m);

    if constexpr (HasName<MeshType>) {
        m.name() = FileInfo::fileNameWithoutExtension(filename);
    }
    if constexpr (HasTexturePaths<MeshType>) {
        m.meshBasePath() = FileInfo::pathWithoutFileName(filename);
    }

    // for logging
    std::vector<uint> eln;
    uint              sum = 0;
    for (const PlyElement& el : header) {
        eln.push_back(el.numberElements);
        sum += el.numberElements;
    }

    try {
        uint currElems = 0;
        uint i         = 0;
        for (const PlyElement& el : header) {
            double beginPerc = double(currElems) / sum * 100.0;
            double endPerc   = double(currElems + eln[i]) / sum * 100.0;

            switch (el.type) {
            case ply::VERTEX:
                log.startNewTask(beginPerc, endPerc, "Reading vertices");
                readPlyVertices(file, header, m, log);
                log.endTask("Reading vertices");
                break;
            case ply::FACE:
                log.startNewTask(beginPerc, endPerc, "Reading faces");
                if constexpr (HasFaces<MeshType>)
                    readPlyFaces(file, header, m, loadedInfo, log);
                else
                    readPlyUnknownElement(file, header, el, log);
                log.endTask("Reading faces");
                break;
            case ply::TRISTRIP:
                log.startNewTask(beginPerc, endPerc, "Reading tristrips");
                if constexpr (HasFaces<MeshType>) {
                    loadedInfo.setTriangleMesh();
                    readPlyTriStrips(file, header, m, log);
                }
                else
                    readPlyUnknownElement(file, header, el, log);
                log.endTask("Reading tristrips");
                break;
            case ply::EDGE:
                log.startNewTask(beginPerc, endPerc, "Reading edges");
                if constexpr (HasEdges<MeshType>)
                    readPlyEdges(file, header, m, log);
                else
                    readPlyUnknownElement(file, header, el, log);
                log.endTask("Reading edges");
                break;
            default:
                log.startNewTask(
                    beginPerc, endPerc, "Reading unknown elements");
                readPlyUnknownElement(file, header, el, log);
                log.endTask("Reading unknown elements");
                break;
            }
            currElems += eln[i];
            ++i;
        }
        readPlyTextures(header, m, log, settings);
    }
    catch (const std::runtime_error& err) {
        m.clear();
        throw err;
    }
}

} // namespace detail

/**
 * @brief Loads from the given input ply stream and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * The info about what elements and components have been loaded from the stream
 * will be stored into the loadedInfo argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] inputPlyStream: the stream to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] settings: settings for loading the file/stream.
 * @param[in] log: the logger to use
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&           m,
    std::istream&       inputPlyStream,
    MeshInfo&           loadedInfo,
    const LoadSettings& settings = LoadSettings(),
    LogType&            log      = nullLogger)
{
    detail::loadPly(m, inputPlyStream, "", loadedInfo, settings, log);
}

/**
 * @brief Loads the given ply file and puts the content into the mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * The info about what elements and components have been loaded from the file
 * will be stored into the loadedInfo argument.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] filename: the name of the file to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the file
 * @param[in] settings: settings for loading the file/stream.
 * @param[in] log: the logger to use
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    const LoadSettings& settings = LoadSettings(),
    LogType&            log      = nullLogger)
{
    std::ifstream file = openInputFileStream(filename);

    detail::loadPly(m, file, filename, loadedInfo, settings, log);
}

} // namespace vcl

#endif // VCL_IO_MESH_PLY_LOAD_H
