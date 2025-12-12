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
IMAGE2D_ARRAY_WO(u_irradiance, rgba32f, 1);

NUM_THREADS(8, 8, 1)
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

    // gid.x,y = pixel
    // gid.z   = cubemap face index [0..5]

    ivec2 pixel = gid.xy;
    int face    = gid.z;

    ivec3 dims  = imageSize(u_irradiance);
    int size    = dims.x;  // cube is size×size×6

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size || face >= 6)
        return; 

    // Pixel center to UV in [-1,1]
    vec2 uv = (vec2(pixel) + 0.5) / float(size);
    uv = uv * 2.0 - 1.0;

    // Get direction corresponding to cubemap face pixel
    vec3 dir = faceDirection(uint(face), uv, true);

    vec3 normal = normalize(dir);
    vec3 up     = vec3(0.0, 1.0, 0.0);
    vec3 right  = normalize(cross(up, normal));
    up          = normalize(cross(normal, right));

    // Irradiance integration
    const float sampleDelta = 0.075;
    vec3 irradiance = vec3_splat(0.0);
    uint nrSamples = 0;
    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);
        for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            // Spherical to cartesian (in tangent space)
            vec3 tangentSample;
            tangentSample.x = sinTheta * cosPhi;
            tangentSample.y = sinTheta * sinPhi;
            tangentSample.z = cosTheta;

            // Tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

            // Sample the environment's radiance
            vec3 envColor = textureCubeLod(s_env0, sampleVec, 0).rgb;

            // Weight by cosine of theta
            // sin(theta) compensates for smaller area covered by samples closer to the pole
            irradiance += envColor * cosTheta * sinTheta;

            nrSamples++;
        }
    }
    // (1.0 / float(nrSamples)) take the average of the samples
    // PI = PI^2 / PI where:
    //    PI^2 is the intergration interval to normalize the average (get the actual integral)
    //    1 / PI lambertian BRDF factor
    irradiance = PI * irradiance * (1.0 / float(nrSamples));

    imageStore(u_irradiance, ivec3(pixel.x, pixel.y, face), vec4(irradiance, 1.0));
}