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
    // segment points in clip space
    vec4 C0 = mul(u_modelViewProj, vec4(p0, 1.0));
    vec4 C1 = mul(u_modelViewProj, vec4(p1, 1.0));

    // clip segment to near plane (w = 0) if needed
    vec4 clippedC0 = C0;
    vec4 clippedC1 = C1;

    float nearEpsilon = 0.001;  // small positive value to avoid w=0

    if (C0.w < nearEpsilon && C1.w >= nearEpsilon) {
        // C0 is behind camera, C1 is in front - clip C0 to near plane
        float t = (nearEpsilon - C0.w) / (C1.w - C0.w);
        clippedC0 = mix(C0, C1, t);
    } else if (C1.w < nearEpsilon && C0.w >= nearEpsilon) {
        // C1 is behind camera, C0 is in front - clip C1 to near plane
        float t = (nearEpsilon - C1.w) / (C0.w - C1.w);
        clippedC1 = mix(C1, C0, t);
    } else if (C0.w < nearEpsilon && C1.w < nearEpsilon) {
        // both behind camera - discard by moving to far away
        gl_Position = vec4(0.0, 0.0, 100.0, 1.0);
        return;
    }

    // pick this vertex's position by uv.x along clipped segment */
    vec4 C = mix(clippedC0, clippedC1, uv.x);

    // compute direction in NDC space (now safe since both points have w > 0) */
    vec2 ndc0 = clippedC0.xy / clippedC0.w;
    vec2 ndc1 = clippedC1.xy / clippedC1.w;
    vec2 screenDir = ndc1 - ndc0;
    float screenDirLen = length(screenDir);

    if (screenDirLen > 0.0001) {
        vec2 T = screenDir / screenDirLen;
        vec2 N = vec2(-T.y, T.x);
        
        float pixelsToNDCX = 2.0 / screenWidth;
        float pixelsToNDCY = 2.0 / screenHeight;
        
        float side = 2.0 * uv.y - 1.0;
        vec2 offsetNDC = N * (0.5 * thickness) * side * vec2(pixelsToNDCX, pixelsToNDCY);
        
        // Convert back to clip space
        vec2 newNDC = (C.xy / C.w) + offsetNDC;
        C.xy = newNDC * C.w;
    }

    v_color = color;
    gl_Position = C;
}
