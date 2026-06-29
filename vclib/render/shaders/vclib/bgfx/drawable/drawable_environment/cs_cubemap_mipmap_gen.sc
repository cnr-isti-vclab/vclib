// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.sh>

IMAGE2D_ARRAY_RW(i_prevMip, rgba32f, 0);
IMAGE2D_ARRAY_RW(i_nextMip, rgba32f, 1);

NUM_THREADS(8, 8, 1)
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

    // gid.x,y = pixel
    // gid.z   = cubemap face index [0..5]

    ivec2 pixel = gid.xy;
    int face    = gid.z;

    ivec3 dims  = imageSize(i_nextMip);
    int size    = dims.x;  // cube is size×size×6

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size || face >= 6)
        return; 

    ivec2 prevCoord = pixel * 2;

    float invPrevSize = 1.0 / imageSize(i_prevMip).x;

    vec4 newColor = vec4_splat(0.0);

    float totalWeight = 0.0;

    for(uint i = 0; i < 4; i++)
    {
        ivec3 prev = ivec3(prevCoord.x + (i % 2), prevCoord.y + int(i > 1), face);

        vec2 uv = vec2(prev.xy) * vec2_splat(invPrevSize) * vec2(2.0, 2.0) - vec2(1.0, 1.0);

        float weight = solidAngle(uv, invPrevSize * 2.0);

        newColor += imageLoad(i_prevMip, prev) * vec4_splat(weight);

        totalWeight += weight;
    }

    newColor /= vec4_splat(totalWeight);

    imageStore(i_nextMip, ivec3(pixel.x, pixel.y, face), newColor);
}
