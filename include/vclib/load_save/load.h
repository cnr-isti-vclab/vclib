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

#ifndef VCL_LOAD_SAVE_LOAD_H
#define VCL_LOAD_SAVE_LOAD_H

#include "obj/load.h"
#include "off/load.h"
#include "ply/load.h"
#include "stl/load.h"

/**
 * @defgroup load Load functions
 * @ingroup load_save
 *
 * @brief List of functions that allow to load from file an input Mesh.
 */

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
 * @param[in] settings: settings for loading the file.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void load(
    MeshType&           m,
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    std::string ext = FileInfo::extension(filename);
    ext             = vcl::toLower(ext);

    loadedInfo.clear();

    if (ext == ".obj") {
        loadObj(m, filename, loadedInfo, log, settings);
    }
    else if (ext == ".off") {
        loadOff(m, filename, loadedInfo, log, settings);
    }
    else if (ext == ".ply") {
        loadPly(m, filename, loadedInfo, log, settings);
    }
    else if (ext == ".stl") {
        loadStl(m, filename, loadedInfo, log, settings);
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
 * @param[in] settings: settings for loading the file.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void load(
    MeshType&           m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    load(m, filename, loadedInfo, log, settings);
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
 * @param[in] settings: settings for loading the file.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType load(
    const std::string&  filename,
    MeshInfo&           loadedInfo,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshType m;
    load(m, filename, loadedInfo, log, settings);
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
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType load(
    const std::string&  filename,
    const LoadSettings& settings,
    LogType&            log = nullLogger)
{
    MeshInfo loadedInfo;
    return load<MeshType>(filename, loadedInfo, log, settings);
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
 * @param[in] settings: settings for loading the file.
 * @return The mesh object containing the loaded mesh.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType load(
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    MeshInfo loadedInfo;
    return load<MeshType>(filename, loadedInfo, log, settings);
}

} // namespace vcl

#endif // VCL_LOAD_SAVE_LOAD_H
