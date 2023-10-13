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

#ifndef VCL_IO_PLY_LOAD_H
#define VCL_IO_PLY_LOAD_H

#include <vclib/misc/logger.h>
#include <vclib/misc/mesh_info.h>

#include "detail/extra.h"
#include "detail/face.h"
#include "detail/tristrip.h"
#include "detail/vertex.h"

namespace vcl {

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
 * @param m
 * @param filename
 * @param loadedInfo
 * @param enableOptionalComponents
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&          m,
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    using namespace detail;
    std::ifstream file = openInputFileStream(filename);

    PlyHeader header(filename, file);
    if (header.errorWhileLoading())
        throw MalformedFileException("Header not valid: " + filename);

    m.clear();

    loadedInfo = header.getInfo();

    if (enableOptionalComponents)
        io::detail::enableOptionalComponents(loadedInfo, m);

    if constexpr (HasName<MeshType>) {
        m.name() = FileInfo::fileNameWithoutExtension(filename);
    }
    if constexpr (HasTexturePaths<MeshType>) {
        m.meshBasePath() = FileInfo::pathWithoutFileName(filename);
    }
    try {
        for (const PlyElement& el : header) {
            switch (el.type) {
            case ply::VERTEX: readPlyVertices(file, header, m); break;
            case ply::FACE: readPlyFaces(file, header, m); break;
            case ply::TRISTRIP: readPlyTriStrips(file, header, m); break;
            default: readPlyUnknownElement(file, header, el); break;
            }
        }
        readPlyTextures(header, m);
    }
    catch (const std::runtime_error& err) {
        m.clear();
        file.close();
        throw err;
    }

    file.close();
}

/**
 * @brief Loads the given ply file and puts the content into the mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * If you need to know what elements and components have been loaded after the
 * loading of the file, please see the overload of the function with the
 * additional 'loadedInfo' argument.
 *
 * @param m
 * @param filename
 * @param enableOptionalComponents
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&          m,
    const std::string& filename,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    loadPly(m, filename, loadedInfo, log, enableOptionalComponents);
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadPly(
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshType m;
    loadPly(m, filename, loadedInfo, log, enableOptionalComponents);
    return m;
}

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadPly(
    const std::string& filename,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    return loadPly<MeshType>(
        filename, loadedInfo, log, enableOptionalComponents);
}

} // namespace vcl

#endif // VCL_IO_PLY_LOAD_H