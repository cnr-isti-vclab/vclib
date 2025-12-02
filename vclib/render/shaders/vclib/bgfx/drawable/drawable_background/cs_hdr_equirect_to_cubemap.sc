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

IMAGE2D_RO(s_hdr, rgba32f, 0);     // bound with setTexture()
IMAGE2D_ARRAY_WO(u_cubemap, rgba32f, 1); // bound with setImage() as RW

// Cubemap face directions
vec3 faceDirection(uint face, vec2 uv)
{
    // uv in range [-1,1]
    switch (face)
    {
        case 0: return normalize(vec3( 1.0,    uv.y, -uv.x )); // +X
        case 1: return normalize(vec3(-1.0,    uv.y,  uv.x )); // -X
        case 2: return normalize(vec3( uv.x,  1.0,   -uv.y));  // +Y
        case 3: return normalize(vec3( uv.x, -1.0,    uv.y));  // -Y
        case 4: return normalize(vec3( uv.x,  uv.y,   1.0 ));  // +Z
        case 5: return normalize(vec3( uv.x,  uv.y,  -1.0 ));  // -Z
        default: return vec3_splat(0.0);
    }
}

// Converts direction -> equirectangular UV
vec2 dirToEquirectUV(vec3 dir)
{
    float phi   = atan2(dir.z, dir.x);        // [-pi..pi]
    float theta = asin(dir.y);               // [-pi/2..pi/2]

    float u = (phi   / (2.0*PI)) + 0.5;
    float v = (theta / PI) + 0.5;

    return vec2(u, v);
}

NUM_THREADS(8, 8, 1) // 8x8 threads per threadgroup
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

    // gid.x,y = pixel
    // gid.z   = cubemap face index [0..5]

    ivec2 pixel = gid.xy;
    int face    = gid.z;

    ivec3 dims  = imageSize(u_cubemap);
    int size    = dims.x;  // cube is size×size×6

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size || face >= 6)
        return;

    // Pixel center to UV in [-1,1]
    vec2 uv = (vec2(pixel) + 0.5) / float(size);
    uv = uv * 2.0 - 1.0;

    // Get direction corresponding to cubemap face pixel
    vec3 dir = faceDirection(uint(face), uv);

    // Convert to lat-long UV
    vec2 equiUV = dirToEquirectUV(dir);

    // Sample HDR
    vec4 hdrColor = imageLoad(s_hdr, equiUV);

    float asd = float(pixel.x)/size;

    // debug: solid color per face
    switch(face) {
        case 0: hdrColor = vec4(asd, 0.0, 0.0, 1.0); break; // +X red
        case 1: hdrColor = vec4(0.0, asd, 0.0, 1.0); break; // -X green
        case 2: hdrColor = vec4(0.0, 0.0, asd, 1.0); break; // +Y blue
        case 3: hdrColor = vec4(asd, asd, 0.0, 1.0); break; // -Y yellow
        case 4: hdrColor = vec4(asd, 0.0, asd, 1.0); break; // +Z magenta
        case 5: hdrColor = vec4(0.0, asd, asd, 1.0); break; // -Z cyan
        default: hdrColor = vec4(0.0, 0.0, 0.0, 1.0); break;
    }

    if(pixel.x == pixel.y)
        hdrColor = vec4_splat(1.0);

    // Write to cubemap layer
    imageStore(u_cubemap, ivec3(pixel, face), hdrColor);
}
