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

#ifndef VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_H
#define VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_H

#include "mesh_render_info_macros.h"

namespace vcl {

struct MeshRenderInfo {
    enum class Points {
        DRAW = 0,
        SHAPE_PIXEL,
        SHAPE_CIRCLE,
        SHADING_NONE,
        SHADING_VERT,
        COLOR_VERTEX,
        COLOR_MESH,
        COLOR_USER,

        COUNT
    };

    enum class Surface {
        DRAW = 0,
        SHADING_NONE,
        SHADING_FLAT,
        SHADING_SMOOTH,
        COLOR_FACE,
        COLOR_VERTEX,
        COLOR_MESH,
        COLOR_USER,
        COLOR_VERTEX_TEX,
        COLOR_WEDGE_TEX,

        COUNT
    };

    enum class Wireframe {
        DRAW = 0,
        SHADING_NONE,
        SHADING_VERT,
        COLOR_VERTEX,
        COLOR_MESH,
        COLOR_USER,

        COUNT
    };

    enum class Edges {
        DRAW = 0,
        SHADING_NONE,
        SHADING_FLAT,
        SHADING_SMOOTH,
        COLOR_VERTEX,
        COLOR_EDGE,
        COLOR_USER,
        COLOR_MESH,

        COUNT
    };
};

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_H
