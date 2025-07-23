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

#ifndef VCL_IO_MESH_LOAD_MESHES_H
#define VCL_IO_MESH_LOAD_MESHES_H

#ifdef VCLIB_WITH_TINYGLTF
#include "gltf/load.h"
#endif

#include "capability.h"

namespace vcl {

/**
 * @brief Returns the set of mesh formats supported for loading multiple Meshes
 * from file.
 *
 * The set contains all the mesh formats that can be loaded using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of mesh formats supported for loading multiple Meshes.
 *
 * @ingroup load_mesh
 */
inline std::set<FileFormat> loadMeshesFileFormats()
{
    std::set<FileFormat> ff;

#ifdef VCLIB_WITH_TINYGLTF
    ff.insert(gltfFileFormat());
#endif

    return ff;
}

/**
 * @brief Loads a list of meshes from a file with the given filename and
 * stores it in the given vector. Checks automatically the file format to
 * load from the given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] meshes: The vector of mesh objects in which to store the loaded
 * meshes.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Vector of information about the meshes components
 * that have been loaded from the file.
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadMeshes(
    std::vector<MeshType>& meshes,
    const std::string&     filename,
    std::vector<MeshInfo>& loadedInfo,
    const LoadSettings&    settings,
    LogType&               log = nullLogger)
{
    FileFormat ff = FileInfo::fileFormat(filename);

    loadedInfo.clear();

#ifdef VCLIB_WITH_TINYGLTF
    if (ff == gltfFileFormat()) {
        loadGltf(meshes, filename, loadedInfo, settings, log);
    }
    else {
        throw UnknownFileFormatException(ff.extensions().front());
    }
#endif
}

/**
 * @brief Loads a list of meshes from a file with the given filename and
 * stores it in the given vector. Checks automatically the file format to
 * load from the given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] meshes: The vector of mesh objects in which to store the loaded
 * meshes.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Vector of information about the meshes components
 * that have been loaded from the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] settings: settings for loading the file.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadMeshes(
    std::vector<MeshType>& meshes,
    const std::string&     filename,
    std::vector<MeshInfo>& loadedInfo,
    LogType&               log      = nullLogger,
    const LoadSettings&    settings = LoadSettings())
{
    loadMeshes(meshes, filename, loadedInfo, settings, log);
}

/**
 * @brief Loads a list of meshes from a file with the given filename and
 * stores it in the given vector. Checks automatically the file format to
 * load from the given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] meshes: The vector of mesh objects in which to store the loaded
 * meshes.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadMeshes(
    std::vector<MeshType>& meshes,
    const std::string&     filename,
    const LoadSettings&    settings,
    LogType&               log = nullLogger)
{
    std::vector<MeshInfo> loadedInfo;
    loadMeshes(meshes, filename, loadedInfo, settings, log);
}

/**
 * @brief Loads a list of meshes from a file with the given filename and
 * stores it in the given vector. Checks automatically the file format to
 * load from the given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[out] meshes: The vector of mesh objects in which to store the loaded
 * meshes.
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] settings: settings for loading the file.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void loadMeshes(
    std::vector<MeshType>& meshes,
    const std::string&     filename,
    LogType&               log      = nullLogger,
    const LoadSettings&    settings = LoadSettings())
{
    std::vector<MeshInfo> loadedInfo;
    loadMeshes(meshes, filename, loadedInfo, settings, log);
}

/**
 * @brief Loads a list of meshes from a file with the given filename and
 * stores it in the returned vector. Checks automatically the file format to
 * load from the given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Vector of information about the meshes components
 * that have been loaded from the file.
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @return The vector of mesh objects in which to store the loaded meshes.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
std::vector<MeshType> loadMeshes(
    const std::string&     filename,
    std::vector<MeshInfo>& loadedInfo,
    const LoadSettings&    settings,
    LogType&               log = nullLogger)
{
    std::vector<MeshType> v;
    loadMeshes(v, filename, loadedInfo, settings, log);
    return v;
}

/**
 * @brief Loads a list of meshes from a file with the given filename and
 * stores it in the returned vector. Checks automatically the file format to
 * load from the given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[out] loadedInfo: Vector of information about the meshes components
 * that have been loaded from the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] settings: settings for loading the file.
 * @return The vector of mesh objects in which to store the loaded meshes.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
std::vector<MeshType> loadMeshes(
    const std::string&     filename,
    std::vector<MeshInfo>& loadedInfo,
    LogType&               log      = nullLogger,
    const LoadSettings&    settings = LoadSettings())
{
    std::vector<MeshType> v;
    loadMeshes(v, filename, loadedInfo, settings, log);
    return v;
}

/**
 * @brief Loads a list of meshes from a file with the given filename and
 * stores it in the returned vector. Checks automatically the file format to
 * load from the given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in] settings: settings for loading the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @return The vector of mesh objects in which to store the loaded meshes.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
std::vector<MeshType> loadMeshes(
    const std::string&  filename,
    const LoadSettings& settings,
    LogType&            log = nullLogger)
{
    std::vector<MeshType> v;
    std::vector<MeshInfo> loadedInfo;
    loadMeshes(v, filename, loadedInfo, settings, log);
    return v;
}

/**
 * @brief Loads a list of meshes from a file with the given filename and
 * stores it in the returned vector. Checks automatically the file format to
 * load from the given filename.
 *
 * @tparam MeshType The type of mesh to load. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] filename: The filename of the file containing the mesh data.
 * @param[in, out] log: The logger object to use for logging messages during
 * loading. Default is the nullLogger object.
 * @param[in] settings: settings for loading the file.
 * @return The vector of mesh objects in which to store the loaded meshes.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup load_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
std::vector<MeshType> loadMeshes(
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const LoadSettings& settings = LoadSettings())
{
    std::vector<MeshType> v;
    std::vector<MeshInfo> loadedInfo;
    loadMeshes(v, filename, loadedInfo, settings, log);
    return v;
}

} // namespace vcl

#endif // VCL_IO_MESH_LOAD_MESHES_H
