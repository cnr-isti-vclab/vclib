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
IMAGE2D_ARRAY_WO(u_specular, rgba32f, 1);

uniform vec4 u_dataPack;
#define roughness u_dataPack.x
#define sourceResolution u_dataPack.y

NUM_THREADS(8, 8, 1)
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

    // gid.x,y = pixel
    // gid.z   = cubemap face index [0..5]

    ivec2 pixel = gid.xy;
    int face    = gid.z;

    ivec3 dims  = imageSize(u_specular);
    int size    = dims.x;  // cube is size×size×6

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size || face >= 6)
        return; 

    // Pixel center to UV in [-1,1]
    vec2 uv = (vec2(pixel) + 0.5) / float(size);
    uv = uv * 2.0 - 1.0;

    // Get direction corresponding to cubemap face pixel
    vec3 dir = faceDirection(uint(face), uv, true);

    vec3 N = normalize(dir);
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;
    vec3 prefilteredColor = vec3_splat(0.0);
    float alpha2 = roughness * roughness * roughness * roughness;

    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        // Hammersley point on hemisphere
        vec2 Xi = hammersley(i, SAMPLE_COUNT);
        vec3 H  = importanceSampleGGX(Xi, N, roughness);

        // determine mip level to sample based on pdf
        float NdotH = max(dot(N, H), 0.0);
        float pdf = D_GGX(NdotH, alpha2) / 4.0; // actually D * NdotH / (4.0 * HdotV) but remember N = V = R
        float saTexel  = 4.0 * PI / (6.0 * sourceResolution * sourceResolution);
        float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf);
        float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if (NdotL > 0.0)
        {
            prefilteredColor += textureCubeLod(s_env0, L, mipLevel).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }
    prefilteredColor /= totalWeight;

    imageStore(u_specular, ivec3(pixel.x, pixel.y, face), vec4(prefilteredColor, 1.0));
}
