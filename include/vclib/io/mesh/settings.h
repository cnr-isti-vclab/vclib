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

#ifndef VCL_IO_MESH_SETTINGS_H
#define VCL_IO_MESH_SETTINGS_H

namespace vcl {

/**
 * @brief The LoadSettings structure contains the settings that can be used to
 * load a mesh from a stream/file.
 */
struct LoadSettings
{
    /**
     * @brief if true, some eventual optional components of the mesh that were
     * not enabled and that can be loaded from the stream/file, will be enabled
     * before loading the stream/file.
     */
    bool enableOptionalComponents = true;
    bool loadTextureImages        = false;
};

} // namespace vcl

#endif // VCL_IO_MESH_SETTINGS_H
