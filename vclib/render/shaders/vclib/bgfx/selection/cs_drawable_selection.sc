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

BUFFER_RW(vertex_selected, vec4, 4);   // is vertex selected? 1 bit per vertex...

IMAGE2D_WO(tex_selection, rgba8, 7);

uniform vec4 u_selectionBox;

/* TODO: Clearly you'll have to check the coordinates in view space... 
* (i imagine the selection box "lives" on the view plane)
*/

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

    uint bufferIndex = pointId/128;
    uint vec4Index = (pointId%128)/32;
    uint bitOffset = 31-(pointId%32);
    uint bitMask = 0x1 << bitOffset;
    uint newValue;
    if (p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY) {
        newValue = (uint(vertex_selected[bufferIndex][vec4Index]) | bitMask);
    } else {
        newValue = (uint(vertex_selected[bufferIndex][vec4Index]) & (~bitMask));
    }
    switch(vec4Index) {
        case 0:
            vertex_selected[bufferIndex] = vec4(
                newValue,
                vertex_selected[bufferIndex].y,
                vertex_selected[bufferIndex].z,
                vertex_selected[bufferIndex].w
            );
            break;
        case 1:
            vertex_selected[bufferIndex] = vec4(
                vertex_selected[bufferIndex].x,
                newValue,
                vertex_selected[bufferIndex].z,
                vertex_selected[bufferIndex].w
            );
            break;
        case 2:
            vertex_selected[bufferIndex] = vec4(
                vertex_selected[bufferIndex].x,
                vertex_selected[bufferIndex].y,
                newValue,
                vertex_selected[bufferIndex].w
            );
            break;
        case 3:
            vertex_selected[bufferIndex] = vec4(
                vertex_selected[bufferIndex].x,
                vertex_selected[bufferIndex].y,
                vertex_selected[bufferIndex].z,
                newValue
            );
            break;
    }
    imageStore(tex_selection, ivec2(pointId, 0), uintABGRToVec4Color((uint(vertex_selected[bufferIndex][vec4Index]) & bitMask) >> (bitOffset)));
}
