// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/shaders_common.sh>

IMAGE2D_WO(s_tex, rgba8, 4); // texture to write to
BUFFER_RO(buf, uint, 5); // interpret as uint buffer

// We expect the texture (used as an array) to be big 
// enough to contain at least wGroupSize.x * wGroupSize.y * wGroupSize.z elements

// X and Y are workGroup X and Y sizes, Z is texture X size, W is buffer size (all to be interpreted as uints)
uniform vec4 u_workGroupSizeXYTexSizeXAndBufSize;

NUM_THREADS(1, 1, 1) // 1 'thread' per buffer index
void main()
{
    uvec2 wGroupSize = uvec2(floatBitsToUint(u_workGroupSizeXYTexSizeXAndBufSize.x), floatBitsToUint(u_workGroupSizeXYTexSizeXAndBufSize.y));
    uint texXSize = floatBitsToUint(u_workGroupSizeXYTexSizeXAndBufSize.z);
    uint bufferIndex = gl_WorkGroupID.x + (gl_WorkGroupID.y * wGroupSize.x) + (gl_WorkGroupID.z * wGroupSize.y * wGroupSize.x);

    if (bufferIndex >= floatBitsToUint(u_workGroupSizeXYTexSizeXAndBufSize.w)) {
        return;
    }

    ivec2 txCoord = ivec2(int(bufferIndex%texXSize), int(bufferIndex/texXSize));

    // RGBA "preserves" the uint's byte order inside the vec4 (big endian)
    imageStore(s_tex, txCoord, uintRGBAToVec4Color(buf[bufferIndex]));
}
