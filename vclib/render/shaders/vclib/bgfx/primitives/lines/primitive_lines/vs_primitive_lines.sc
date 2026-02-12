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
$output v_color, v_normal

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

#include <vclib/bgfx/primitives/lines/uniforms.sh>
#include <vclib/bgfx/shaders_common.sh> 

#define p                     a_position
#define color                 a_color0
#define normal                a_normal

void main() {
    v_color = color;
    v_normal = normalize(mul(u_normalMatrix, normal));
    vec4 pos = mul(u_modelViewProj, vec4(p, 1.0));
    pos.z += -u_depthOffset * pos.w;
    gl_Position = pos;
}
