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

$input a_position, a_normal, a_color0
$output v_position, v_normal, v_color

// cross section
$output v_worldPos, v_discardFlag

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_position = mul(u_modelView, vec4(a_position, 1.0)).xyz;
    v_normal = normalize(mul(u_normalMatrix, a_normal));
    v_worldPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;

    // default case - color is taken from buffer
    v_color = a_color0;

    // discard flag - used to discard the whole vertex, but in fragment shader
    v_discardFlag = computeDiscardFlag(v_worldPos);
}
