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

$input a_position
$output v_texcoord1

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.sh>

void main()
{
    uint idx = uint(gl_VertexID) & 3u; // last 2 bits identify the quad corner
    vec2 quadUv = vec2(idx & 1u, (idx >> 1u) & 1u);

    gl_Position = vec4(
        (a_position.x - u_viewRect.x) / u_viewRect.z * 2.0 - 1.0,
        1.0 - (a_position.y - u_viewRect.y) / u_viewRect.w * 2.0,
        0.0,
        1.0);

    v_texcoord1 = quadUv;
}
