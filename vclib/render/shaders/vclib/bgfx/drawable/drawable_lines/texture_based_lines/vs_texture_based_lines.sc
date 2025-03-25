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

$input a_position
$output v_color, v_uv, v_length, v_normal

#include <vclib/bgfx/drawable/lines.sh>

uniform vec4 u_data;
uniform vec4 u_IndirectData;

#define maxTextureSize        u_IndirectData.x
#define uv                    a_position

IMAGE2D_RO(textureBuffer, rgba32f, 0);

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

    vec4 p0          = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 4), maxTextureSize));
    vec4 p1          = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 4) + 1, maxTextureSize));
    vec4 normal0     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 4) + 2, maxTextureSize));
    vec4 normal1     = imageLoad(textureBuffer, calculateTextureCoord((gl_InstanceID * 4) + 3, maxTextureSize));

    vec4 color0 = uintRGBAToVec4FloatColor(floatBitsToUint(p0.w));
    vec4 color1 = uintRGBAToVec4FloatColor(floatBitsToUint(p1.w));

    vec4 p0_px = calculatePointWithMVP(p0, u_screenWidth, u_screenHeigth);
    vec4 p1_px = calculatePointWithMVP(p1, u_screenWidth, u_screenHeigth);
    
    v_color = (((color0 * (1 - uv.x)) + (color1 * uv.x)) * (1 - sign(u_color_to_use))) + (u_general_color * sign(u_color_to_use));
    v_normal = ((normal0 * (1 - uv.x)) + (normal1 * uv.x)).xyz;
    v_length = length(p1_px.xyz - p0_px.xyz);
    v_uv = calculateLinesUV(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap);
    gl_Position = calculateLines(p0_px, p1_px, uv, v_length, u_thickness, u_antialias, u_border, u_screenWidth, u_screenHeigth, u_leftCap, u_rigthCap);
}
