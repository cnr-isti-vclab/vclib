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

BUFFER_RW(face_selected, uint, 6); // is face selected? 1 bit per face...
IMAGE2D_RO(s_tex, rgba16f, 4); // each texel: x and y upper and lower 16 bits of meshID (meshID of 0 -> nothing); z and w upper and lower 16 bits of PrimitiveID

uniform vec4 u_dispatchXYSizeAndMeshID;

NUM_THREADS(1, 1, 1) // 1 thread per texel
void main()
{
    uvec2 wGroupSize = uvec2(floatBitsToUint(u_dispatchXYSizeAndMeshID.x), floatBitsToUint(u_dispatchXYSizeAndMeshID.y));
    uint tex1DCoord = gl_WorkGroupID.x + (gl_WorkGroupID.y * wGroupSize.x) + (gl_WorkGroupID.z * wGroupSize.y * wGroupSize.x);
    ivec2 texSize = imageSize(s_tex);
    ivec2 texCoord = ivec2(tex1DCoord%texSize.x, tex1DCoord/texSize.x);
    if (texCoord.y >= texSize.y) {
        return;
    }
    vec4 temp = imageLoad(s_tex, texCoord);
    uint meshID = ((floatBitsToUint(temp.x) & 0xFFFF) << 16) | (floatBitsToUint(temp.y) & 0xFFFF);
    if (meshID != floatBitsToUint(u_dispatchXYSizeAndMeshID.z)) {
        return;
    }
    uint faceIndex = ((floatBitsToUint(temp.z) & 0xFFFF) << 16) | (floatBitsToUint(temp.w) & 0xFFFF);
    uint fBufferIndex = faceIndex/32;
    uint fBitOffset = 31-(faceIndex%32);
    uint fBitMask = 0x1 << fBitOffset;
    uint _useless;
    atomicFetchAndOr(face_selected[fBufferIndex], fBitMask, _useless);
}