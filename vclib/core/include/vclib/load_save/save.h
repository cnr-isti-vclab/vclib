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

#ifndef VCL_LOAD_SAVE_SAVE_H
#define VCL_LOAD_SAVE_SAVE_H

#include "obj/save.h"
#include "off/save.h"
#include "ply/save.h"
#include "stl/save.h"

/**
 * @defgroup save Save functions
 * @ingroup load_save
 *
 * @brief List of functions that allow to save to file an input Mesh.
 */

namespace vcl {

/**
 * @brief Saves a mesh to a file with the given filename. Checks automatically
 * the file format to save from the given filename.
 *
 * @tparam MeshType The type of mesh to save. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: The mesh object to save.
 * @param[in] filename: The filename of the file where to save the mesh data.
 * @param[in, out] log: The logger object to use for logging messages during
 * saving.
 * @param[in] settings: Settings for saving the file.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup save
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void save(
    const MeshType&     m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const SaveSettings& settings = SaveSettings())
{
    std::string ext = FileInfo::extension(filename);
    ext             = toLower(ext);
    if (ext == ".obj") {
        saveObj(m, filename, log, settings);
    }
    else if (ext == ".off") {
        saveOff(m, filename, log, settings);
    }
    else if (ext == ".ply") {
        savePly(m, filename, log, settings);
    }
    else if (ext == ".stl") {
        saveStl(m, filename, log, settings);
    }
    else {
        throw UnknownFileFormatException(ext);
    }
}

/**
 * @brief Saves a mesh to a file with the given filename. Checks automatically
 * the file format to save from the given filename.
 *
 * @tparam MeshType The type of mesh to save. It must satisfy the MeshConcept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] m: The mesh object to save.
 * @param[in] filename: The filename of the file where to save the mesh data.
 * @param[in] settings: Settings for saving the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * saving.
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void save(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    save(m, filename, log, settings);
}

} // namespace vcl

#endif // VCL_LOAD_SAVE_SAVE_H
