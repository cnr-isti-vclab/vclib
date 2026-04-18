/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_IO_MESH_SAVE_MESHES_H
#define VCL_IO_MESH_SAVE_MESHES_H

#ifdef VCLIB_WITH_TINYGLTF
#include "gltf/save.h"
#endif

#include "capability.h"

namespace vcl {

/**
 * @brief Returns the set of mesh formats supported for saving multiple Meshes
 * to file.
 *
 * The set contains all the mesh formats that can be saved using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of mesh formats supported for saving multiple Meshes.
 *
 * @ingroup save_mesh
 */
inline std::set<FileFormat> saveMeshesFormats()
{
    std::set<FileFormat> ff;

#ifdef VCLIB_WITH_TINYGLTF
    ff.insert(gltfFileFormat());
#endif

    return ff;
}

/**
 * @brief Saves a range of meshes to a file with the given filename. Checks
 * automatically the file format to save from the given filename.
 *
 * @tparam Meshes The range of meshes to save. It must satisfy the RangeOfMeshes
 * concept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] meshes: The range of meshes to save.
 * @param[in] filename: The filename of the file where to save the meshes.
 * @param[in] settings: Settings for saving the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * saving.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup save_mesh
 */
template<RangeOfMeshes Meshes, LoggerConcept LogType = NullLogger>
void saveMeshes(
    Meshes&&               meshes,
    const std::string&     filename,
    const SaveSettings&    settings,
    LogType&               log = nullLogger)
{
    FileFormat ff = FileInfo::fileFormat(filename);

#ifdef VCLIB_WITH_TINYGLTF
    if (ff == gltfFileFormat()) {
        saveGltf(meshes, filename, settings, log);
    }
    else {
        throw UnknownFileFormatException(ff.extensions().front());
    }
#endif
}

/**
 * @brief Saves a range of meshes to a file with the given filename. Checks
 * automatically the file format to save from the given filename.
 *
 * @tparam Meshes The range of meshes to save. It must satisfy the RangeOfMeshes
 * concept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] meshes: The range of meshes to save.
 * @param[in] filename: The filename of the file where to save the meshes.
 * @param[in, out] log: The logger object to use for logging messages during
 * saving.
 * @param[in] settings: Settings for saving the file.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup save_mesh
 */
template<RangeOfMeshes Meshes, LoggerConcept LogType = NullLogger>
void saveMeshes(
    Meshes&&            meshes,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const SaveSettings& settings = SaveSettings())
{
    saveMeshes(meshes, filename, settings, log);
}

} // namespace vcl

#endif // VCL_IO_MESH_SAVE_MESHES_H
