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

$input v_texcoord0

#include <vclib/bgfx/drawable/drawable_background/uniforms.sh>

// textures
SAMPLERCUBE(s_env0, 0);

void main()
{
    // bring the interpolated fragment and camera positions
    // back to world space ignoring any translation component in the view matrix
    vec4 clipFragPos = vec4(v_texcoord0, 1.0, 1.0);
    vec4 viewFragPos = mul(u_invProj, clipFragPos);
    mat4 invViewNoTrans = u_invView;
    invViewNoTrans[3] = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 worldFragPos = mul(invViewNoTrans, viewFragPos).xyz;
    // commented code is kept for clarity and already done implicitly
    //vec3 worldCameraPos = mul(invViewNoTrans, vec4(0.0, 0.0, 0.0, 1.0)).xyz; just the origin, we have just set it

    // use the view direction in world space to sample the cubemap
    vec3 worldViewDir = normalize(worldFragPos /*- worldCameraPos*/);

    // default 90 degrees rotation for environments just as:
    // https://github.com/KhronosGroup/glTF-Sample-Renderer/blob/e6b052db89fb2adbaf31da4565a08265c96c2b9f/source/GltfState/gltf_state.js#L91
    // but keep in mind that we are in a right-handed system as opposed to the left-handed system of cubemaps
    float temp = worldViewDir.z;
    worldViewDir.z = -worldViewDir.x;
    worldViewDir.x = temp;

    gl_FragColor = textureCube(s_env0, worldViewDir);
}
