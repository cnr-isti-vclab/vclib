// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.sh>

SAMPLERCUBE(s_env0, 0);
IMAGE2D_ARRAY_WO(i_specular, rgba32f, 1);

NUM_THREADS(8, 8, 1)
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

    // gid.x,y = pixel
    // gid.z   = cubemap face index [0..5]

    ivec2 pixel = gid.xy;
    int face    = gid.z;

    ivec3 dims  = imageSize(i_specular);
    int size    = dims.x;  // cube is size×size×6

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size || face >= 6)
        return; 

    // Pixel center to UV in [-1,1]
    vec2 uv = (vec2(pixel) + 0.5) / float(size);
    uv = uv * 2.0 - 1.0;

    // Get direction corresponding to cubemap face pixel
    vec3 dir = faceDirection(uint(face), uv, false);

    vec3 N = normalize(dir);
    vec3 V = N;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3_splat(0.0);
    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec4 sample = getImportanceSample(
            i,                              // current sample index
            SAMPLE_COUNT, 
            N, 
            DISTRIBUTION_GGX,
            u_roughness
        );

        vec3 H = sample.xyz;
        float pdf = sample.w;

        float mipLevel = computeLod(pdf, u_cubeSideResolution, float(SAMPLE_COUNT));

        vec3 L = normalize(reflect(-V, H));
        float NoL = dot(N, L);

        if(NoL > 0.0)
        {
            if(u_roughness == 0.0)
                mipLevel = 0.0;

            vec3 sampleColor = textureCubeLod(s_env0, leftHand(L), mipLevel).rgb;
            prefilteredColor += sampleColor * NoL;
            totalWeight += NoL;
        }
    }
    prefilteredColor /= totalWeight;

    imageStore(i_specular, ivec3(pixel.x, pixel.y, face), vec4(prefilteredColor, 1.0));
}
