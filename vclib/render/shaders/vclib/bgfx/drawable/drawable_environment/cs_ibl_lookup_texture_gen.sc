/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.sh>

IMAGE2D_WO(i_lut, rgba32f, 0);

NUM_THREADS(8, 8, 1)
void main()
{
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);

    ivec2 dims = imageSize(i_lut);
    float size = dims.x; // texture is square

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size)
        return; 

    // Pixel center to UV in [0,1]
    vec2 uv = (vec2(pixel) + 0.5) / float(size);

    float NoV = uv.x;
    float roughness = uv.y;
    float alpha2 = roughness * roughness * roughness * roughness;

    // Compute spherical view vector: (sin(phi), 0, cos(phi))
    vec3 V = vec3(sqrt(1.0 - NoV * NoV), 0.0, NoV);

    // The macro surface normal just points up.
    vec3 N = vec3(0.0, 0.0, 1.0);

    // To make the LUT independent from the material's F0, which is part of the Fresnel term
    // when substituted by Schlick's approximation, we factor it out of the integral,
    // yielding to the form: F0 * I1 + I2
    // I1 and I2 are slightly different in the Fresnel term, but both only depend on
    // NoL and roughness, so they are both numerically integrated and written into two channels.
    float I1 = 0.0;
    float I2 = 0.0;

    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0; i < SAMPLE_COUNT; ++i)
    {
        vec4 sample = getImportanceSample(
            i,
            SAMPLE_COUNT,
            N,
            DISTRIBUTION_GGX,
            roughness
        );

        vec3 H = sample.xyz;
        vec3 L = normalize(reflect(-V, H));

        float NoL = saturate(L.z);
        float NoH = saturate(H.z);
        float VoH = saturate(dot(V, H));

        if(NoL > 0.0)
        {
            // LUT for GGX distribution.

            // Taken from: https://bruop.github.io/ibl
            // Shadertoy: https://www.shadertoy.com/view/3lXXDB
            // Terms besides V are from the GGX PDF we're dividing by.
            float V_pdf = V_GGX(NoV, NoL, alpha2) * VoH * NoL / NoH;
            float Fc = pow(1.0 - VoH, 5.0);
            I1 += (1.0 - Fc) * V_pdf;
            I2 += Fc * V_pdf;
        }
    }

    // The PDF is simply pdf(v, h) -> NDF * <nh>.
    // To parametrize the PDF over l, use the Jacobian transform, yielding to: pdf(v, l) -> NDF * <nh> / 4<vh>
    // Since the BRDF divide through the PDF to be normalized, the 4 can be pulled out of the integral.
    vec3 brdf = vec3(I1, I2, 0.0) * vec3_splat(4.0);
    brdf /= vec3_splat(SAMPLE_COUNT);

    imageStore(i_lut, pixel, vec4(brdf, 1.0));
}
