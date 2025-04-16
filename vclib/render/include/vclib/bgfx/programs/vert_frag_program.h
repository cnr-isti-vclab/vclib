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

#ifndef VCL_BGFX_PROGRAMS_VERT_FRAG_PROGRAM_H
#define VCL_BGFX_PROGRAMS_VERT_FRAG_PROGRAM_H

namespace vcl {

enum class VertFragProgram {
    DRAWABLE_AXIS,
    DRAWABLE_DIRECTIONAL_LIGHT,

    DRAWABLE_MESH_EDGES,
    DRAWABLE_MESH_POINTS,

    DRAWABLE_MESH_SURFACE_UBER,
    //SURFACE_NONE
    DRAWABLE_MESH_SURFACE_NONE_COLOR_VERTEX,
    DRAWABLE_MESH_SURFACE_NONE_COLOR_MESH,
    DRAWABLE_MESH_SURFACE_NONE_COLOR_FACE,
    DRAWABLE_MESH_SURFACE_NONE_COLOR_USER,
    DRAWABLE_MESH_SURFACE_NONE_TEX_VERTEX,
    DRAWABLE_MESH_SURFACE_NONE_TEX_WEDGE,
    //SURFACE_FLAT
    DRAWABLE_MESH_SURFACE_FLAT_COLOR_VERTEX,
    DRAWABLE_MESH_SURFACE_FLAT_COLOR_MESH,
    DRAWABLE_MESH_SURFACE_FLAT_COLOR_FACE,
    DRAWABLE_MESH_SURFACE_FLAT_COLOR_USER,
    DRAWABLE_MESH_SURFACE_FLAT_TEX_VERTEX,
    DRAWABLE_MESH_SURFACE_FLAT_TEX_WEDGE,
    //SURFACE_SMOOTH
    DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_VERTEX,
    DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_MESH,
    DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_FACE,
    DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_USER,
    DRAWABLE_MESH_SURFACE_SMOOTH_TEX_VERTEX,
    DRAWABLE_MESH_SURFACE_SMOOTH_TEX_WEDGE,

    //SURFACE_NONE
    DRAWABLE_MESH_SURFACE_NONE_COLOR_VERTEX_SI,
    DRAWABLE_MESH_SURFACE_NONE_COLOR_MESH_SI,
    DRAWABLE_MESH_SURFACE_NONE_COLOR_FACE_SI,
    DRAWABLE_MESH_SURFACE_NONE_COLOR_USER_SI,
    DRAWABLE_MESH_SURFACE_NONE_TEX_VERTEX_SI,
    DRAWABLE_MESH_SURFACE_NONE_TEX_WEDGE_SI,
    //SURFACE_FLAT
    DRAWABLE_MESH_SURFACE_FLAT_COLOR_VERTEX_SI,
    DRAWABLE_MESH_SURFACE_FLAT_COLOR_MESH_SI,
    DRAWABLE_MESH_SURFACE_FLAT_COLOR_FACE_SI,
    DRAWABLE_MESH_SURFACE_FLAT_COLOR_USER_SI,
    DRAWABLE_MESH_SURFACE_FLAT_TEX_VERTEX_SI,
    DRAWABLE_MESH_SURFACE_FLAT_TEX_WEDGE_SI,
    //SURFACE_SMOOTH
    DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_VERTEX_SI,
    DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_MESH_SI,
    DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_FACE_SI,
    DRAWABLE_MESH_SURFACE_SMOOTH_COLOR_USER_SI,
    DRAWABLE_MESH_SURFACE_SMOOTH_TEX_VERTEX_SI,
    DRAWABLE_MESH_SURFACE_SMOOTH_TEX_WEDGE_SI,

    DRAWABLE_MESH_WIREFRAME,
    DRAWABLE_MESH_EDGES_ID,
    DRAWABLE_MESH_POINTS_ID,
    DRAWABLE_MESH_SURFACE_ID,
    DRAWABLE_MESH_WIREFRAME_ID,

    DRAWABLE_TRACKBALL,

    FONT_BASIC,
    FONT_DISTANCE_FIELD,
    FONT_DISTANCE_FIELD_DROP_SHADOW,
    FONT_DISTANCE_FIELD_DROP_SHADOW_IMAGE,
    FONT_DISTANCE_FIELD_OUTLINE,
    FONT_DISTANCE_FIELD_OUTLINE_DROP_SHADOW_IMAGE,
    FONT_DISTANCE_FIELD_OUTLINE_IMAGE,
    FONT_DISTANCE_FIELD_SUBPIXEL,

    COUNT
};

} // namespace vcl

#endif // VCL_BGFX_PROGRAMS_VERT_FRAG_PROGRAM_H
