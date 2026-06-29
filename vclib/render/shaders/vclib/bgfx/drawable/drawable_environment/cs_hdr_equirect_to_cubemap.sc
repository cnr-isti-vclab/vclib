// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.sh>

SAMPLER2D(s_hdr, 0);                     // bound with setTexture()
IMAGE2D_ARRAY_WO(i_cubemap, rgba32f, 1); // bound with setImage() as RW

NUM_THREADS(8, 8, 1) // 8x8 threads per threadgroup
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

    // gid.x,y = pixel
    // gid.z   = cubemap face index [0..5]

    ivec2 pixel = gid.xy;
    int face    = gid.z;

    ivec3 dims  = imageSize(i_cubemap);
    int size    = dims.x;  // cube is size×size×6

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size || face >= 6)
        return; 

    // Pixel center to UV in [-1,1]
    vec2 uv = (vec2(pixel) + 0.5) / float(size);
    uv = uv * 2.0 - 1.0;

    // Get direction corresponding to cubemap face pixel
    vec3 dir = faceDirection(uint(face), uv, true);

    // Convert to lat-long UV
    vec2 equiUV = dirToEquirectUV(dir);

    // Sample HDR
    vec4 hdrColor = texture2DLod(s_hdr, equiUV, 0); // texture2D not supported in compute shaders

    // Write to cubemap layer
    imageStore(i_cubemap, ivec3(pixel.x, pixel.y, face), hdrColor);
}
