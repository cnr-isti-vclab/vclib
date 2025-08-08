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

BUFFER_RW(vertex_selected, uint, 4);   // is vertex selected? 1 bit per vertex...

uniform vec4 u_selectionBox; // screen space
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
    uint bufferIndex = pointId/32;
    uint bitOffset = 31-(pointId%32);
    uint bitMask = 0x1 << bitOffset;
    uint _useless;
    atomicFetchAndAnd(vertex_selected[bufferIndex], ~bitMask, _useless);
}
