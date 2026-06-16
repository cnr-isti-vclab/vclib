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

$input a_position, a_color0
$output v_color

#include <bgfx_shader.sh>
#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_lines_uniforms.sh>

void main()
{
    gl_Position = vec4(
        (a_position[0] - u_viewRect.x) / u_viewRect.z * 2 -1,
        1 - (a_position[1] - u_viewRect.y) / u_viewRect.w * 2,
        0,
        1);
    
    // Use per-vertex color if enabled, otherwise use general color
    if (usePerVertexColor()) {
        v_color = a_color0;
    } else {
        v_color = u_linesGeneralColor;
    }
}
