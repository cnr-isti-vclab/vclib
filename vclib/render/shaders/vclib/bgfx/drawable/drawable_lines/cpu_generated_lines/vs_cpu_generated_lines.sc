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

$input a_position, a_texcoord0, a_color0, a_normal, a_texcoord1
$output v_color, v_uv, v_length, v_normal

#include <vclib/bgfx/drawable/lines.sh>

uniform vec4 u_data;

#define p0                    a_position
#define p1                    a_texcoord0
#define uv                    vec2(float(a_texcoord1.x), float(a_texcoord1.y))

void main() {
    vec4 u_general_color = uintRGBAToVec4FloatColor(floatBitsToUint(u_data.x));
    uint thickness_antialias_border_caps = floatBitsToUint(u_data.y);
    
    float u_screenWidth  = u_viewRect.z;
    float u_screenHeigth = u_viewRect.w;

    float u_thickness    = float((thickness_antialias_border_caps >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_caps >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_caps >> uint(8))  & uint(0xFF));
    float u_leftCap      = float((thickness_antialias_border_caps >> uint(4))  & uint(0x3));
    float u_rigthCap     = float((thickness_antialias_border_caps >> uint(2))  & uint(0x3));
    float u_color_to_use = float((thickness_antialias_border_caps >> uint(0))  & uint(0x3));

    vec4 p0_px = calculatePointWithMVP(vec4(p0, 0.0), u_screenWidth, u_screenHeigth);
    vec4 p1_px = calculatePointWithMVP(vec4(p1, 0.0), u_screenWidth, u_screenHeigth);
    
    v_color = (a_color0 * (1 - sign(u_color_to_use))) + (u_general_color * sign(u_color_to_use));
    v_normal = a_normal;
    v_length = length(p1_px.xyz - p0_px.xyz);

    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap);

    v_color = vec4(p0.x, 0, 0, 1);
    gl_Position = vec4(uv.xy, 0, 1);
}
