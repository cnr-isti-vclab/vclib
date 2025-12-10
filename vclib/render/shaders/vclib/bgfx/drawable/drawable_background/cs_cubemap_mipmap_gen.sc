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

IMAGE2D_ARRAY_RW(u_prevMip, rgba32f, 0);
IMAGE2D_ARRAY_RW(u_nextMip, rgba32f, 1);

NUM_THREADS(1, 1, 1)
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

    vec4 newColor = (
        imageLoad(u_prevMip, ivec3(prevCoord.x,     prevCoord.y,     face)) +
        imageLoad(u_prevMip, ivec3(prevCoord.x + 1, prevCoord.y,     face)) +
        imageLoad(u_prevMip, ivec3(prevCoord.x,     prevCoord.y + 1, face)) +
        imageLoad(u_prevMip, ivec3(prevCoord.x + 1, prevCoord.y + 1, face))
    ) * vec4_splat(0.25);

    imageStore(u_nextMip, ivec3(pixel.x, pixel.y, face), newColor);
}