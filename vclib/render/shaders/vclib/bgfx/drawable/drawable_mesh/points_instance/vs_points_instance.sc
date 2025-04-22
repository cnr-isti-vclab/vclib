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

$input a_position, a_color0, a_normal, a_texcoord0
$output v_normal, v_color

#include <vclib/bgfx/shaders_common.sh>

void main()
{
    uint idx = uint(gl_VertexID) & 3u; // last 2 bits
    vec4 pos = mul(u_modelViewProj, vec4(a_position, 1.0));
    vec4 offset = vec4(
        (idx & 1u) * 2.0f * u_viewTexel.x,
        ((idx >> 1) & 1u) * 2.0f * u_viewTexel.y, 0, 0);

    gl_Position = pos + offset;
    v_normal = normalize(mul(u_normalMatrix, a_normal));

    // default case - color is taken from buffer
    v_color = a_color0;
}
