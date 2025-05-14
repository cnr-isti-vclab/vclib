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

$input a_position, i_data0, i_data1, i_data2, i_data3, i_data4
$output v_color, v_uv, v_length, v_normal, v_is_start_end

#include <vclib/bgfx/drawable/polylines.sh>

uniform vec4 u_data;

#define a_uv                    a_position

#define a_prev                  vec4(i_data0.xyz, 0.0)
#define a_curr                  vec4(i_data1.xyz, 0.0)
#define a_next                  vec4(i_data2.xyz, 0.0)
#define color                   uintABGRToVec4Color(floatBitsToUint(i_data1.w))
#define normal                  vec3(i_data0.w, i_data3.w, i_data4.x)

void main() {
    vec4 u_general_color = uintABGRToVec4Color(floatBitsToUint(u_data.x));
    uint thickness_antialias_border_miterlimit = floatBitsToUint(u_data.y);
    uint caps_joint_color = floatBitsToUint(u_data.w);
    
    float u_screenWidth  = u_viewRect.z;
    float u_screenHeigth = u_viewRect.w;

    float u_thickness    = float((thickness_antialias_border_miterlimit >> uint(24)) & uint(0xFF));
    float u_antialias    = float((thickness_antialias_border_miterlimit >> uint(16)) & uint(0xFF));
    float u_border       = float((thickness_antialias_border_miterlimit >> uint(8))  & uint(0xFF));
    float u_miter_limit  = float(thickness_antialias_border_miterlimit               & uint(0xFF));
    
    float u_leftCap      = float((caps_joint_color >> uint(6)) & uint(0x3));
    float u_rigthCap     = float((caps_joint_color >> uint(4)) & uint(0x3));
    float u_joint        = float((caps_joint_color >> uint(2)) & uint(0x3));
    float u_color_to_use = float((caps_joint_color)            & uint(0x3));

    vec4 prev_px = calculatePointWithMVP(a_prev, u_screenWidth, u_screenHeigth);
    vec4 curr_px = calculatePointWithMVP(a_curr, u_screenWidth, u_screenHeigth);
    vec4 next_px = calculatePointWithMVP(a_next, u_screenWidth, u_screenHeigth);

    v_color = color;
    v_normal = normal;
    v_uv = vec4(0,0,0,0);
    v_length = 0;

    bool is_start = false;
    bool is_end = false;
    v_is_start_end = 1;
    gl_Position = calculatePolylines(prev_px, curr_px, next_px, a_uv, u_thickness, u_miter_limit, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap, u_joint, is_start, is_end);
}
