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
$output v_color

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

uniform vec4 u_settings;

#define thickness             u_settings.x
// #define colorToUse            u_settings.y

#define p0                    a_position
#define p1                    a_texcoord0
#define color                 a_color0
// #define normal                a_normal
#define uv                    a_texcoord1.xy
// #define lineColor             uintABGRToVec4Color(floatBitsToUint(a_texcoord1.z))
#define screenWidth           u_viewRect.z
#define screenHeight          u_viewRect.w

// vec4 screenToClip(vec4 coordinate) {
//     return vec4(
//                   (2 * coordinate.x / screenWidth),
//                   (2 * coordinate.y / screenHeight),
//                   coordinate.z,
//                   coordinate.w
//                 );
// }

// vec4 clipToScreen(vec4 coordinate) {
//     return vec4(
//                   (coordinate.x * screenWidth) / 2,
//                   (coordinate.y * screenHeight) / 2,
//                   coordinate.z,
//                   coordinate.w
//                 );
// }

// vec4 calculatePointWithMVP(vec4 p) {
//     vec4 NDC_p = mul(u_modelViewProj, vec4(p.xyz, 1.0));

//     // vec4 screen_p = vec4(NDC_p.xy / NDC_p.w, 0.0, 0.0);

//     // vec4 p_px = clipToScreen(screen_p);
//     return vec4(p_px.xy, NDC_p.z, NDC_p.w);
// }

void main() {
    vec4 view_p0 = mul(u_modelView, vec4(p0, 1.0));
    vec4 view_p1 = mul(u_modelView, vec4(p1, 1.0));

    vec2 T = normalize(view_p0.xy - view_p1.xy);
    vec2 N = vec2(-T.y, T.x);

    vec4 view_p = (view_p0 * (1 - uv.x)) + (view_p1 * uv.x);

    // Extract horizontal and vertical scale from projection matrix
    float scaleX = u_proj[0][0]; // = 1 / (aspect * tan(fov/2))
    float scaleY = u_proj[1][1]; // = 1 / tan(fov/2)

    // Compute size of 1 pixel in view space at current depth
    float pixelSizeX = (2.0 * view_p.z) / (scaleX * screenWidth);
    float pixelSizeY = (2.0 * view_p.z) / (scaleY * screenHeight);

    float v = 2.0 * uv.y - 1.0;
    vec2 pixelOffset = v * N * (thickness / 2);
    vec3 offset = vec3(pixelOffset.x * pixelSizeX, pixelOffset.y * pixelSizeY, 0.0);

    vec4 posView = view_p + vec4(offset.xyz, 0);

    v_color = color;
    gl_Position = mul(u_proj, posView);

    // // vec4 p0_px = calculatePointWithMVP(vec4(p0, 0.0));
    // // vec4 p1_px = calculatePointWithMVP(vec4(p1, 0.0));
    
    // float length_px = length(p1_px.xyz - p0_px.xyz);
    // float half_thickness_px = thickness / 2.0;


    
    // // float u = 2.0 * uv.x - 1.0;
    // float v = 2.0 * uv.y - 1.0;

    // vec4 p;
    // p = p0_px + (uv.x * T * length_px) + (v * N * half_thickness_px); 
    // p = screenToClip(p);

    // float z = ((1 - uv.x) * (p0_px.z)) + (uv.x * (p1_px.z));
    // float w = ((1 - uv.x) * (p0_px.w)) + (uv.x * (p1_px.w));

    // v_color = color;
    // gl_Position = vec4(p.xy * w, z, w);
}
