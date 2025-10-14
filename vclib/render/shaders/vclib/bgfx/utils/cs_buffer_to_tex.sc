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

IMAGE2D_WO(s_tex, rgba8, 4); // texture to write to
BUFFER_RO(buf, uint, 5); // interpret as uint buffer

// We expect the texture (used as an array) to be big 
// enough to contain at least wGroupSize.x * wGroupSize.y * wGroupSize.z elements

// X and Y are workGroup X and Y sizes, Z is texture X size, W is buffer size (all to be interpreted as uints)
uniform vec4 u_workGroupSizeXYTexSizeXAndBufSize;

// AGBR function "inverts" the uint's byte order inside the vec4 (little endian),
// using this would cause us to write the inverse of what we actually need in the texture (we would then need to invert them again when reading).
// RGBA function "preserves" the uint's byte order inside the vec4 (big endian),
// which is what we need in this case
vec4 uintRGBAToVec4Color(uint color) {
    vec4 temp = uintABGRToVec4Color(color);
    return vec4(temp.w, temp.z, temp.y, temp.x);
}

NUM_THREADS(1, 1, 1) // 1 'thread' per buffer index
void main()
{
    uvec2 wGroupSize = uvec2(floatBitsToUint(u_workGroupSizeXYTexSizeXAndBufSize.x), floatBitsToUint(u_workGroupSizeXYTexSizeXAndBufSize.y));
    uint texXSize = floatBitsToUint(u_workGroupSizeXYTexSizeXAndBufSize.z);
    uint bufferIndex = gl_WorkGroupID.x + (gl_WorkGroupID.y * wGroupSize.x) + (gl_WorkGroupID.z * wGroupSize.y * wGroupSize.x);

    if (bufferIndex >= floatBitsToUint(u_workGroupSizeXYTexSizeXAndBufSize.w)) {
        return;
    }

    ivec2 txCoord = ivec2((int)bufferIndex%texXSize, (int)(bufferIndex/texXSize));
    imageStore(s_tex, txCoord, uintRGBAToVec4Color(buf[bufferIndex]));
}