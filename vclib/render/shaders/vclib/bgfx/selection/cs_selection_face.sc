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
BUFFER_RO(indices, uint, 5);
BUFFER_RW(face_selected, uint, 6); // is face selected? 1 bit per face...

uniform vec4 u_selectionBox; // screen space
uniform vec4 u_workgroupSizeAndVertexCount;

// THE SELECTION IS CHECKED IN NDC SPACE. I decided for this because this way i only need the viewRect and the modelViewProj uniforms.
// Possibility: uniform containing selection box passed already in NDC space? It's probably doable

NUM_THREADS(1, 1, 1) // 1 'thread' per face, or 1 'thread' per 3 indices
void main()
{
    uint indexCount = floatBitsToUint(u_workgroupSizeAndVertexCount.w);
    uvec3 workGroupSize = uvec3(floatBitsToUint(u_workgroupSizeAndVertexCount.x), floatBitsToUint(u_workgroupSizeAndVertexCount.y), floatBitsToUint(u_workgroupSizeAndVertexCount.z));
    uint indicesBaseIndex = 3 * (gl_WorkGroupID.x + workGroupSize.x * gl_WorkGroupID.y + workGroupSize.x * workGroupSize.y * gl_WorkGroupID.z);
    uvec3 idcs = uvec3(indices[indicesBaseIndex], indices[indicesBaseIndex + 1], indices[indicesBaseIndex + 2]);

    if(indicesBaseIndex >= indexCount) {
        return;
    }

    uint slctd = 1;
    for (uint i = 0; i < 3; i ++) {
        uint bufferIndex = idcs[i]/32;
        uint bitOffset = 31-(idcs[i]%32);
        uint bitMask = 0x1 << bitOffset;
        if((vertex_selected[bufferIndex] & bitMask) == uint(0)) {
            slctd = 0;
            break;
        }
    }

    uint fBufferIndex = (indicesBaseIndex/3)/32;
    uint fBitOffset = 31-((indicesBaseIndex/3)%32);
    uint fBitMask = 0x1 << fBitOffset;
    uint _useless;
    if (slctd == 1) {
        atomicFetchAndOr(vertex_selected[fBufferIndex], fBitMask, _useless);
    } else {
        atomicFetchAndAnd(vertex_selected[fBufferIndex], ~fBitMask, _useless);
    }
}