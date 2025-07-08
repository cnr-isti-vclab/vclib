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

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(positions, vec4, VCL_MRB_VERTEX_POSITION_STREAM); // coordinates (3 floats)
BUFFER_RO(normals,   vec4, VCL_MRB_VERTEX_NORMAL_STREAM);   // normals (3 floats)
BUFFER_RO(colors,    uvec4, VCL_MRB_VERTEX_COLOR_STREAM);   // colors (rgba uint)

BUFFER_WO(vOut, vec4, 4); // output vertices
// 2 vec4 per vertex:
// - 3 floats for position + 1 uint for color
// - 3 floats for normal   + 1 float for scale


NUM_THREADS(1, 1, 1) // 1 'thread' per point
void main()
{
    uint pointId = gl_WorkGroupID.x;
    uint idx30 = pointId * 3;
    uint idx31 = idx30+1;
    uint idx32 = idx30+2;

    float col = uintBitsToFloat(colors[pointId/4][pointId%4]);
    vec3 p = vec3(
        positions[idx30/4][idx30%4],
        positions[idx31/4][idx31%4],
        positions[idx32/4][idx32%4]);
    vec3 n = vec3(
        normals[idx30/4][idx30%4],
        normals[idx31/4][idx31%4],
        normals[idx32/4][idx32%4]);

    // Generate quad vertices
    UNROLL
    for (int i = 0; i < 4; ++i) {
        // Offset for quad vertices
        uint vertexId = pointId * 4 + i;
        // pos 3, col 1
        // norm 3, sca 1
        vOut[vertexId * 2]     = vec4(p, col);
        vOut[vertexId * 2 + 1] = vec4(n, 1.0);
    }
}
