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

$input v_color, v_normal

#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.sh>
#include <vclib/bgfx/shaders_common.sh> 

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

uniform vec4 u_settings;
#define u_shadingPerVertex bool(u_settings.w)

void main() {
    vec4 color = v_color;
    if (u_shadingPerVertex) {
        color *= computeLight(u_lightDir, u_lightColor, v_normal);
    }
    gl_FragColor = color;
}
