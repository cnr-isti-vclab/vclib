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
    // back to world space
    vec4 clipFragPos = vec4(v_texcoord0, 1.0, 1.0);
    vec3 worldFragPos = mul(u_invViewProj, clipFragPos).xyz;
    vec3 worldCameraPos = mul(u_invView, vec4(0.0, 0.0, 0.0, 1.0)).xyz;

    // use the view direction in world space to sample the cubemap
    vec3 worldViewDir = normalize(worldFragPos - worldCameraPos);

    //worldViewDir.x *= -1.0; // necessary now but why??

    gl_FragColor = textureCube(s_env0, worldViewDir);
}
