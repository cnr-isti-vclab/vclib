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

#ifndef VCL_IO_PLY_LOAD_H
#define VCL_IO_PLY_LOAD_H

#include <vclib/mesh/utils/mesh_info.h>
#include <vclib/misc/logger.h>

#include "detail/extra.h"
#include "detail/face.h"
#include "detail/tristrip.h"
#include "detail/vertex.h"

namespace vcl {

namespace detail {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&          m,
    std::istream&      file,
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    PlyHeader header(file, filename);
    if (header.errorWhileLoading())
        throw MalformedFileException("Header not valid: " + filename);

    m.clear();

    loadedInfo = header.getInfo();

    if (enableOptionalComponents)
        enableOptionalComponentsFromInfo(loadedInfo, m);

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
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&     m,
    std::istream& inputPlyStream,
    MeshInfo&     loadedInfo,
    LogType&      log                      = nullLogger,
    bool          enableOptionalComponents = true)
{
    detail::loadPly(
        m, inputPlyStream, "", loadedInfo, log, enableOptionalComponents);
}

/**
 * @brief Loads from the given input ply stream and puts the content into the
 * mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] inputPlyStream: the stream to read from
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&     m,
    std::istream& inputPlyStream,
    LogType&      log                      = nullLogger,
    bool          enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    loadPly(m, inputPlyStream, loadedInfo, log, enableOptionalComponents);
}

/**
 * @brief Loads from the given input ply stream and puts the content into the
 * returned mesh m.
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
 * @param[in] inputPlyStream: the stream to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the stream
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 * @returns the mesh loaded from the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadPly(
    std::istream& inputPlyStream,
    MeshInfo&     loadedInfo,
    LogType&      log                      = nullLogger,
    bool          enableOptionalComponents = true)
{
    MeshType m;
    loadPly(m, inputPlyStream, loadedInfo, log, enableOptionalComponents);
    return m;
}

/**
 * @brief Loads from the given input ply stream and puts the content into the
 * returned mesh m.
 *
 * The function will fill all the components read into the stream that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the stream, will be enabled before loading the stream.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] inputPlyStream: the stream to read from
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * stream, will be enabled before loading the stream.
 * @returns the mesh loaded from the stream.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType loadPly(
    std::istream& inputPlyStream,
    LogType&      log                      = nullLogger,
    bool          enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    return loadPly<MeshType>(
        inputPlyStream, loadedInfo, log, enableOptionalComponents);
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
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * file, will be enabled before loading the file.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadPly(
    MeshType&          m,
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    std::ifstream file = openInputFileStream(filename);

    detail::loadPly(
        m, file, filename, loadedInfo, log, enableOptionalComponents);
}

/**
 * @brief Loads the given ply file and puts the content into the mesh m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: the mesh to fill
 * @param[in] filename: the name of the file to read from
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * file, will be enabled before loading the file.
 *
 * @ingroup load
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

/**
 * @brief Loads the given ply file and puts the content into the returned mesh
 * m.
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
 * @param[in] filename: the name of the file to read from
 * @param[out] loadedInfo: the info about what elements and components have been
 * loaded from the file
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * file, will be enabled before loading the file.
 * @returns the mesh loaded from the file.
 *
 * @ingroup load
 */
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

/**
 * @brief Loads the given ply file and puts the content into the returned mesh
 * m.
 *
 * The function will fill all the components read into the file that can be
 * filled into the mesh. If the enableOprionalComponents argument is enabled,
 * some eventual optional components of the mesh that were not enabled and that
 * can be loaded from the file, will be enabled before loading the file.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: the name of the file to read from
 * @param[in] log: the logger to use
 * @param[in] enableOptionalComponents: if true, some eventual optional
 * components of the mesh that were not enabled and that can be loaded from the
 * file, will be enabled before loading the file.
 * @returns the mesh loaded from the file.
 *
 * @ingroup load
 */
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
