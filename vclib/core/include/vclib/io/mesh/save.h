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

#ifndef VCL_IO_MESH_SAVE_H
#define VCL_IO_MESH_SAVE_H

#include "obj/save.h"
#include "off/save.h"
#include "ply/save.h"
#include "stl/save.h"

#include "capability.h"

/**
 * @defgroup save_mesh Save mesh functions
 * @ingroup io_mesh
 *
 * @brief List of functions that allow to saveMesh to file an input Mesh.
 */

namespace vcl {

/**
 * @brief Returns the set of mesh formats supported for saving.
 *
 * The set contains all the mesh formats that can be saved using all the
 * external libraries compiled with VCLib.
 *
 * @return A set of mesh formats supported for saving.
 *
 * @ingroup save_mesh
 */
inline std::set<FileFormat> saveMeshFormats()
{
    std::set<FileFormat> ff;

    ff.insert(objFileFormat());
    ff.insert(offFileFormat());
    ff.insert(plyFileFormat());
    ff.insert(stlFileFormat());

    return ff;
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
 * @param[in, out] log: The logger object to use for logging messages during
 * saving.
 * @param[in] settings: Settings for saving the file.
 *
 * @throws vcl::UnknownFileFormatException if the file extension is not
 * recognized.
 *
 * @ingroup save_mesh
 */
template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveMesh(
    const MeshType&     m,
    const std::string&  filename,
    LogType&            log      = nullLogger,
    const SaveSettings& settings = SaveSettings())
{
    FileFormat ff = FileInfo::fileFormat(filename);

    if (ff == objFileFormat()) {
        saveObj(m, filename, log, settings);
    }
    else if (ff == offFileFormat()) {
        saveOff(m, filename, log, settings);
    }
    else if (ff == plyFileFormat()) {
        savePly(m, filename, log, settings);
    }
    else if (ff == stlFileFormat()) {
        saveStl(m, filename, log, settings);
    }
    else {
        throw UnknownFileFormatException(ff.extensions().front());
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
void saveMesh(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings,
    LogType&            log = nullLogger)
{
    saveMesh(m, filename, log, settings);
}

} // namespace vcl

#endif // VCL_IO_MESH_SAVE_H
