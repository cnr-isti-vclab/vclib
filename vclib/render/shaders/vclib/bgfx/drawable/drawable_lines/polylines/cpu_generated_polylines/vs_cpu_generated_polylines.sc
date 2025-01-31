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

$input a_position, a_texcoord0, a_texcoord1, a_color0, a_normal, a_texcoord2
$output v_color, v_uv, v_length, v_is_start_end, v_normal

#include "../../polylines.sh"

uniform vec4 u_data;

#define a_prev                  a_position
#define a_curr                  a_texcoord0
#define a_next                  a_texcoord1
#define a_uv                    a_texcoord2

void main() { 
    vec4 u_general_color = uintToVec4FloatColor(floatBitsToUint(u_data.x));
    uint thickness_antialias_border_miterlimit = floatBitsToUint(u_data.y);
    uint caps_join_color = floatBitsToUint(u_data.w);
    
    float u_screenWidth  = u_viewRect.z;
    float u_screenHeigth = u_viewRect.w;

    float u_thickness    = float((thickness_antialias_border_miterlimit >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_miterlimit >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_miterlimit >> uint(8))  & uint(0xFF));
    float u_miter_limit  = float(thickness_antialias_border_miterlimit               & uint(0xFF));
    
    float u_leftCap      = float((caps_join_color >> uint(6)) & uint(0x3));
    float u_rigthCap     = float((caps_join_color >> uint(4)) & uint(0x3));
    float u_join         = float((caps_join_color >> uint(2)) & uint(0x3));
    float u_color_to_use = float((caps_join_color)            & uint(0x3));

    vec4 prev_px = calculatePointWithMVP(vec4(a_prev, 0.0), u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(vec4(a_curr, 0.0), u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(vec4(a_next, 0.0), u_screenWidth, u_screenHeigth);

    v_color = (a_color0 * (1 - sign(u_color_to_use))) + (u_general_color * sign(u_color_to_use));
    v_length = length(((next_px.xyz - curr_px.xyz) * (1 - a_uv.x)) + ((curr_px.xyz - prev_px.xyz) * (a_uv.x)));

    v_uv = calculatePolylinesUV(vec4(a_prev, 0.0), vec4(a_curr, 0), vec4(a_next, 0), a_uv, u_thickness, v_length, u_leftCap, u_rigthCap, u_join);

    bool is_start = a_curr.x == a_prev.x && a_curr.y == a_prev.y;
    bool is_end = a_curr.x == a_next.x && a_curr.y == a_next.y;
    v_is_start_end = (sign(length(a_curr - a_prev)) * (1 - a_uv.x)) + (sign(length(a_curr-a_next)) * a_uv.x);
    v_normal = a_normal;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_join, is_start, is_end);
} 
