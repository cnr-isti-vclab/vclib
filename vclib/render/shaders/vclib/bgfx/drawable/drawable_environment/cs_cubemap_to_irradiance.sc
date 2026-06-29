// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.sh>

SAMPLERCUBE(s_env0, 0);
IMAGE2D_ARRAY_WO(i_irradiance, rgba32f, 1);

NUM_THREADS(8, 8, 1)
void main()
{
    ivec3 gid = ivec3(gl_GlobalInvocationID.xyz);

    // gid.x,y = pixel
    // gid.z   = cubemap face index [0..5]

    ivec2 pixel = gid.xy;
    int face    = gid.z;

    ivec3 dims  = imageSize(i_irradiance);
    int size    = dims.x;  // cube is size×size×6

    // in case of an out of bounds thread
    if (pixel.x >= size || pixel.y >= size || face >= 6)
        return; 

    // Pixel center to UV in [-1,1]
    vec2 uv = (vec2(pixel) + 0.5) / float(size);
    uv = uv * 2.0 - 1.0;

    // Get direction corresponding to cubemap face pixel
    vec3 dir = faceDirection(uint(face), uv, false);

    vec3 normal = normalize(dir);

    vec3 irradiance = vec3_splat(0.0);
    const uint SAMPLE_COUNT = 1024u;
    for (uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec4 sample = getImportanceSample(
            i,                              // current sample index
            SAMPLE_COUNT, 
            normal, 
            DISTRIBUTION_LAMBERTIAN, 
            -1.0                            // roughness (not needed for irradiance)
        );

        vec3 H = sample.xyz;
        float pdf = sample.w;

        float mipLevel = computeLod(pdf, u_cubeSideResolution, float(SAMPLE_COUNT)); //TODO: see if a bias is needed

        vec3 lambertian = textureCubeLod(s_env0, leftHand(H), mipLevel).rgb;

        // the below operations cancel each other out
        // lambertian *= NdotH; // lamberts law
        // lambertian /= pdf; // invert bias from importance sampling
        // lambertian /= MATH_PI; // convert irradiance to radiance https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/

        irradiance += lambertian;
    }
    irradiance /= float(SAMPLE_COUNT);

    imageStore(i_irradiance, ivec3(pixel.x, pixel.y, face), vec4(irradiance, 1.0));
}
