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

#ifndef VCL_RENDER_DRAWABLE_MESH_MESH_BUFFER_ID_H
#define VCL_RENDER_DRAWABLE_MESH_MESH_BUFFER_ID_H

#include <vclib/space/core/bit_set.h>
#include <vclib/types.h>

namespace vcl {

enum class MeshBufferId : uint {
    VERTICES,
    VERT_NORMALS,
    VERT_COLORS,
    VERT_TEXCOORDS,

    TRIANGLES,
    TRI_NORMALS,
    TRI_COLORS,
    WEDGE_TEXCOORDS,

    WIREFRAME,

    EDGES,
    EDGE_COLORS,
    EDGE_NORMALS,

    TEXTURES,

    COUNT,
};

namespace detail {
using BuffersToFillUnderlyingType = ushort;
} // namespace detail

static_assert(
    sizeof(detail::BuffersToFillUnderlyingType) < (uint) MeshBufferId::COUNT,
    "BuffersToFill is not able to store all MeshBufferID values");

using BuffersToFill = vcl::BitSet<detail::BuffersToFillUnderlyingType>;

namespace detail {

inline BuffersToFill buffersToFillAll()
{
    BuffersToFill all;
    all.set();
    return all;
}

} // namespace detail

const inline BuffersToFill BUFFERS_TO_FILL_NONE = BuffersToFill();

const inline BuffersToFill BUFFERS_TO_FILL_ALL = detail::buffersToFillAll();

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_BUFFER_ID_H
