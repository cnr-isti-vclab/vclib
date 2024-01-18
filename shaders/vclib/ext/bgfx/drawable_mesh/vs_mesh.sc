/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

$input a_position, a_normal, a_color0
$output v_pos, v_normal, v_color

#include <drawable_mesh/uniforms.sh>
#include <vclib/render/mesh_render_settings_macros.h>

void main()
{
    uint primitive = floatBitsToUint(u_primitiveFloat);
    uint drawMode = floatBitsToUint(u_drawModeFloat);

    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_normal = normalize(mul(u_modelView, vec4(a_normal, 0.0) ).xyz);

    // default case - color is taken from buffer
    v_color = a_color0;

    if (!bool(primitive & VCL_MRS_PRIMITIVE_TRIANGLES)) {
        // offset - do it only if we will draw also surface
        if (bool(drawMode & VCL_MRS_DRAW_SURF)) {
            vec3 n = normalize(mul(u_modelViewProj, vec4(a_normal, 0.0) ).xyz);
            gl_Position += vec4(n * 0.0002, 0.0);
        }
    }

    v_pos = gl_Position.xyz;
}
