// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_3MF_SAVE_H
#define VCL_IO_MESH_3MF_SAVE_H

#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>

#include <vclib/algorithms/mesh.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <lib3mf_implicit.hpp>
#if defined(min)
#undef min
#endif
#if defined(max)
#undef max
#endif

namespace vcl {

namespace detail {

// TODO: implement a function to add a mesh to a 3MF model, like:
// detail::addMeshTo3mfModel(mesh, model, meshInfo, settings, log);

} // namespace detail

/**
 * @brief Saves a mesh to a file with the given filename.
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
void save3mf(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    MeshInfo meshInfo(m);

    // make sure that the given info contains only components that are actually
    // available in the mesh. meshInfo will contain the intersection between the
    // components that the user wants to save and the components that are
    // available in the mesh.
    if (!settings.info.isEmpty())
        meshInfo = settings.info.intersect(meshInfo);

    // TODO: Implement saving the mesh to a 3MF file using the meshInfo and
    // settings.
}

/**
 * @brief Saves a range of meshes to a file with the given filename.
 *
 * @tparam Meshes The range of meshes to save. It must satisfy the RangeOfMeshes
 * concept.
 * @tparam LogType The type of logger to use. It must satisfy the LoggerConcept.
 *
 * @param[in] meshes: The range of meshes to save.
 * @param[in] filename: The filename of the file where to save the mesh data.
 * @param[in] settings: Settings for saving the file.
 * @param[in, out] log: The logger object to use for logging messages during
 * saving.
 */

template<RangeOfMeshes Meshes, LoggerConcept LogType = NullLogger>
void save3mf(
    Meshes&&            meshes,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    // TODO: Create a 3MF model object here.

    for (const auto& mesh : meshes) {
        MeshInfo meshInfo(mesh);

        // make sure that the given info contains only components that are
        // actually available in the mesh. meshInfo will contain the
        // intersection between the components that the user wants to save and
        // the components that are available in the mesh.
        if (!settings.info.isEmpty())
            meshInfo = settings.info.intersect(meshInfo);

        // TODO: Add the mesh to the 3MF model using the meshInfo and settings.
    }

    // TODO: Save the 3MF model to the specified filename.
}

} // namespace vcl

#endif // VCL_IO_MESH_3MF_SAVE_H
