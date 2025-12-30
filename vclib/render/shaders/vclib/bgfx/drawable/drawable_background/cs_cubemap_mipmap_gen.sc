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

#include <vclib/bgfx/drawable/drawable_background/uniforms.sh>

SAMPLERCUBE(s_env0, 0);
IMAGE2D_ARRAY_RW(u_nextMip, rgba32f, 1);

uniform vec4 u_dataPack;
#define prevMip u_dataPack.x

NUM_THREADS(8, 8, 1)
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

    // gid.x,y = pixel
    // gid.z   = cubemap face index [0..5]

    ivec2 pixel = gid.xy;
    int face    = gid.z;

    ivec3 dims  = imageSize(u_nextMip);
    int size    = dims.x;  // cube is size×size×6

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size || face >= 6)
        return; 

    ivec2 prevCoord = pixel * 2;

    float invPrevSize = 1.0 / (size * 2.0);

    ivec3 prev0 = ivec3(prevCoord.x,     prevCoord.y,     face);
    ivec3 prev1 = ivec3(prevCoord.x + 1, prevCoord.y,     face);
    ivec3 prev2 = ivec3(prevCoord.x,     prevCoord.y + 1, face);
    ivec3 prev3 = ivec3(prevCoord.x + 1, prevCoord.y + 1, face);

    vec2 uv0 = prev0.xy * invPrevSize * 2.0 - 1.0;
    vec2 uv1 = prev1.xy * invPrevSize * 2.0 - 1.0;
    vec2 uv2 = prev2.xy * invPrevSize * 2.0 - 1.0;
    vec2 uv3 = prev3.xy * invPrevSize * 2.0 - 1.0;

    vec2 uvCenter0 = (prev0.xy + 0.5) * invPrevSize * 2.0 - 1.0;
    vec2 uvCenter1 = (prev1.xy + 0.5) * invPrevSize * 2.0 - 1.0;
    vec2 uvCenter2 = (prev2.xy + 0.5) * invPrevSize * 2.0 - 1.0;
    vec2 uvCenter3 = (prev3.xy + 0.5) * invPrevSize * 2.0 - 1.0;    
    
    float weight0 = solidAngle(uv0, invPrevSize * 2.0);
    float weight1 = solidAngle(uv1, invPrevSize * 2.0);
    float weight2 = solidAngle(uv2, invPrevSize * 2.0);
    float weight3 = solidAngle(uv3, invPrevSize * 2.0);

    vec3 dir0 = faceDirection(face, uvCenter0, false);    
    vec3 dir1 = faceDirection(face, uvCenter1, false);    
    vec3 dir2 = faceDirection(face, uvCenter2, false);
    vec3 dir3 = faceDirection(face, uvCenter3, false);

    vec4 newColor = (
        textureCubeLod(s_env0, leftHand(dir0), prevMip) * weight0 +
        textureCubeLod(s_env0, leftHand(dir1), prevMip) * weight1 +
        textureCubeLod(s_env0, leftHand(dir2), prevMip) * weight2 +
        textureCubeLod(s_env0, leftHand(dir3), prevMip) * weight3
    ) / (
        weight0 +
        weight1 +
        weight2 +
        weight3
    );

    imageStore(u_nextMip, ivec3(pixel.x, pixel.y, face), newColor);
}