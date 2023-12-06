/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_IO_LOAD_H
#define VCL_IO_LOAD_H

#include "obj/load.h"
#include "off/load.h"
#include "ply/load.h"
#include "stl/load.h"

namespace vcl {

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * given mesh object. Checks automatically the file format to load from the
 * fiven filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] m: The mesh object in which to store the loaded mesh.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Information about the mesh components that have been
 * loaded from the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] enableOptionalComponents: Whether to enable loading of optional
 * components of the mesh that can be loaded from the input file. Default value
 * is `true`.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void load(
    MeshType&          m,
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    std::string ext = FileInfo::extension(filename);
    ext             = vcl::str::toLower(ext);
    if (ext == ".obj") {
        loadObj(m, filename, loadedInfo, log, enableOptionalComponents);
    }
    else if (ext == ".off") {
        loadOff(m, filename, loadedInfo, log, enableOptionalComponents);
    }
    else if (ext == ".ply") {
        loadPly(m, filename, loadedInfo, log, enableOptionalComponents);
    }
    else if (ext == ".stl") {
        loadStl(m, filename, loadedInfo, log, enableOptionalComponents);
    }
    else {
        throw vcl::UnknownFileFormatException(ext);
    }
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * given mesh object. Checks automatically the file format to load from the
 * fiven filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] m: The mesh object in which to store the loaded mesh.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Information about the mesh components that have been
 * loaded from the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] enableOptionalComponents: Whether to enable loading of optional
 * components of the mesh that can be loaded from the input file. Default value
 * is `true`.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void load(
    MeshType&          m,
    const std::string& filename,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    load(m, filename, loadedInfo, log, enableOptionalComponents);
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * returned mesh object. Checks automatically the file format to load from the
 * fiven filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Information about the mesh components that have been
 * loaded from the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] enableOptionalComponents: Whether to enable loading of optional
 * components of the mesh that can be loaded from the input file. Default value
 * is `true`.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType load(
    const std::string& filename,
    MeshInfo&          loadedInfo,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshType m;
    load(m, filename, loadedInfo, log, enableOptionalComponents);
    return m;
}

/**
 * @brief Loads a mesh from a file with the given filename and stores it in the
 * returned mesh object. Checks automatically the file format to load from the
 * fiven filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] enableOptionalComponents: Whether to enable loading of optional
 * components of the mesh that can be loaded from the input file. Default value
 * is `true`.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType load(
    const std::string& filename,
    LogType&           log                      = nullLogger,
    bool               enableOptionalComponents = true)
{
    MeshInfo loadedInfo;
    return load<MeshType>(filename, loadedInfo, log, enableOptionalComponents);
}

} // namespace vcl

#endif // VCL_IO_LOAD_H
