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

// INFO: the two textures MUST be of the same size
IMAGE2D_RO(s_primIds, rgba8, 0);
IMAGE2D_RO(s_meshIds, rgba8, 1);
BUFFER_RW(face_selected, uint, 6);

uniform vec4 u_meshIdAndDispatchSizeXY;

uint texVec4ToUint(vec4 pixel) {
    return
        ((uint(pixel.r * 255.0) & 0xff) << uint(24))
        | ((uint(pixel.g * 255.0) & 0xff) << uint(16))  
        | ((uint(pixel.b * 255.0) & 0xff) << uint(8)) 
        | (uint(pixel.a * 255.0) & 0xff);
}

NUM_THREADS(1, 1, 1)
void main() {
    uint meshId = floatBitsToUint(u_meshIdAndDispatchSizeXY.x);
    // NOTE: meshID 0 is reserved to indicate that no data is available (i.e. the fragment did NOT pass)
    if(meshId == 0) {
        return;
    }
    uvec2 dispatchSize = uvec2(floatBitsToUint(u_meshIdAndDispatchSizeXY.y), floatBitsToUint(u_meshIdAndDispatchSizeXY.z));
    uint tex1DCoord = 
          uint(gl_WorkGroupID.x) 
        + uint(gl_WorkGroupID.y) * dispatchSize.x 
        + uint(gl_WorkGroupID.z) * dispatchSize.x * dispatchSize.y;
    uvec2 imSz = imageSize(s_primIds).xy;
    ivec2 tex2DCoord = ivec2(int(tex1DCoord % imSz.x), int(tex1DCoord / imSz.x));

    vec4 vecMeshId = imageLoad(s_meshIds, tex2DCoord);
    uint texMeshId = texVec4ToUint(vecMeshId);
    if (texMeshId != meshId) {
        return;
    }

    vec4 vecPrimId = imageLoad(s_primIds, tex2DCoord);
    uint texPrimId = texVec4ToUint(vecPrimId);
    uint bufferIndex = texPrimId/32;
    uint bitOffset = 31-(texPrimId%32);
    uint bitMask = 0x1 << bitOffset;
    uint _useless;
    atomicFetchAndAnd(face_selected[bufferIndex], ~bitMask, _useless);
}