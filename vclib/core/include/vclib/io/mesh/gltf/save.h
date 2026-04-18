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

#ifndef VCL_IO_MESH_GLTF_SAVE_H
#define VCL_IO_MESH_GLTF_SAVE_H

#include <vclib/io/file_info.h>
#include <vclib/io/mesh/settings.h>

namespace vcl {

template<MeshConcept MeshType, LoggerConcept LogType = NullLogger>
void saveGltf(
    const MeshType&     m,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    // todo implement
}

template<RangeOfMeshes Meshes, LoggerConcept LogType = NullLogger>
void saveGltf(
    Meshes&&            meshes,
    const std::string&  filename,
    const SaveSettings& settings = SaveSettings(),
    LogType&            log      = nullLogger)
{
    for (const auto& mesh : meshes) {
        // todo...
    }
}

} // namespace vcl

#endif // VCL_IO_MESH_GLTF_SAVE_H
