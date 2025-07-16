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

BUFFER_RW(colors,   uvec4, VCL_MRB_VERTEX_COLOR_STREAM);   // colors (rgba uint)

uniform vec4 u_selectionBox;

NUM_THREADS(1, 1, 1) // 1 'thread' per point
void main()
{
    uint pointId = gl_WorkGroupID.x;
    uint idx30 = pointId * 3;
    uint idx31 = idx30+1;
    uint idx32 = idx30+2;

    float minX = u_selectionBox[0];
    float minY = u_selectionBox[1];
    float maxX = u_selectionBox[2];
    float maxY = u_selectionBox[3];

    vec3 p = vec3(
        positions[idx30/4][idx30%4],
        positions[idx31/4][idx31%4],
        positions[idx32/4][idx32%4]);

    if (p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY) {
        switch(pointId%4) {
            case 0:
                colors[pointId/4] = vec4(uint(0xFF1B78F9), colors[pointId/4].y, colors[pointId/4].z, colors[pointId/4].w);
                break;
            case 1:
                colors[pointId/4] = vec4(colors[pointId/4].x, uint(0xFF1B78F9), colors[pointId/4].z, colors[pointId/4].w);
                break;
            case 2:
                colors[pointId/4] = vec4(colors[pointId/4].x, colors[pointId/4].y, uint(0xFF1B78F9), colors[pointId/4].w);
                break;
            case 3:
                colors[pointId/4] = vec4(colors[pointId/4].x, colors[pointId/4].y, colors[pointId/4].z, uint(0xFF1B78F9));
                break;
        }
    }
}
