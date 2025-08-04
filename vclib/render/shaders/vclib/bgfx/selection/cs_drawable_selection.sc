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

BUFFER_RW(vertex_selected, uint, 4);   // is vertex selected? 1 bit per vertex...

IMAGE2D_WO(tex_selection, r8, 7);

uniform vec4 u_selectionBox;
uniform vec4 u_workgroupSizeAndVertexCount;

// THE SELECTION IS CHECKED IN NDC SPACE. I decided for this because this way i only need the viewRect and the modelViewProj uniforms.
// Possibility: uniform containing selection box passed already in NDC space? It's probably doable

NUM_THREADS(1, 1, 1) // 1 'thread' per point
void main()
{
    uint vertexCount = floatBitsToUint(u_workgroupSizeAndVertexCount.w);
    uvec3 workGroupSize = uvec3(floatBitsToUint(u_workgroupSizeAndVertexCount.x), floatBitsToUint(u_workgroupSizeAndVertexCount.y), floatBitsToUint(u_workgroupSizeAndVertexCount.z));
    uint pointId = gl_WorkGroupID.x + workGroupSize.x * gl_WorkGroupID.y + workGroupSize.x * workGroupSize.y * gl_WorkGroupID.z;
    if(pointId >= vertexCount) {
        return;
    }
    uint idx30 = pointId * 3;
    uint idx31 = idx30+1;
    uint idx32 = idx30+2;

    float minX = u_selectionBox[0];
    float minY = u_selectionBox[1];
    float maxX = u_selectionBox[2];
    float maxY = u_selectionBox[3];

    vec3 maxNDC = vec3(
        (maxX - u_viewRect.x) / u_viewRect.z * 2 - 1,
        1 - ((minY - u_viewRect.y) / u_viewRect.w * 2),
        0
    );
    vec3 minNDC = vec3(
        (minX - u_viewRect.x) / u_viewRect.z * 2 - 1,
        1 - ((maxY - u_viewRect.y) / u_viewRect.w * 2),
        1
    );

    vec4 p = vec4(
        positions[idx30/4][idx30%4],
        positions[idx31/4][idx31%4],
        positions[idx32/4][idx32%4],
        1
    );

    vec4 pNDC = mul(u_modelViewProj, p);
    pNDC = pNDC / pNDC.w;

    uint bufferIndex = pointId/32;
    uint bitOffset = 31-(pointId%32);
    uint bitMask = 0x1 << bitOffset;
    uint _useless;
    if (pNDC.x >= minNDC.x && pNDC.x <= maxNDC.x && pNDC.y >= minNDC.y && pNDC.y <= maxNDC.y) {
        atomicFetchAndOr(vertex_selected[bufferIndex], bitMask, _useless);
    } else {
        atomicFetchAndAnd(vertex_selected[bufferIndex], ~bitMask, _useless);
    }
    uint texSizeX = uint(imageSize(tex_selection).x);
    imageStore(tex_selection, ivec2(pointId%texSizeX, pointId/texSizeX), uintABGRToVec4Color((vertex_selected[bufferIndex] & bitMask) >> (bitOffset)).x);
}
