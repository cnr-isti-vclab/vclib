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
    // outward view direction in world space
    vec3 worldDir = normalize(mul(u_invView, mul(u_invProj, clipFragPos)).xyz);

    gl_FragColor = textureCube(s_env0, worldDir);
}
