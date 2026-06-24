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

$input v_color, v_normal

#include <bgfx_shader.sh>
#if LINES_SHADING_PER_VERTEX || LINES_SHADING_PER_LINE
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.sh>
#endif
#include <vclib/bgfx/shaders_common.sh> 

void main() {
    vec4 color = v_color;
    
#if LINES_SHADING_PER_VERTEX || LINES_SHADING_PER_LINE
    color *= computeLight(u_lightDir, u_lightColor, v_normal);
#endif

    gl_FragColor = color;
}
