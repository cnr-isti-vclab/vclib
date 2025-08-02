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

$input a_position, a_texcoord0, a_color0, a_color1, a_normal, a_texcoord1, a_color2
$output v_color, v_normal

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

#include <vclib/bgfx/shaders_common.sh> 

uniform vec4 u_settings;

#define NEAR_EPSILON 0.001
#define LENGTH_EPSILON 0.0001

#define thickness             u_settings.x
#define colorToUse            u_settings.y
#define generalColor          uintABGRToVec4Color(floatBitsToUint(u_settings.z))

#define p0                    a_position
#define p1                    a_texcoord0

#define color0                a_color0
#define color1                a_color1

#define normal0               a_normal
#define normal1               a_texcoord1

#define lineColor             a_color2

#define screenWidth           u_viewRect.z
#define screenHeight          u_viewRect.w

void main() {
    int generalIndex = gl_VertexID % 4;
    vec2 uv = vec2((generalIndex >> 1) & 0x1, generalIndex & 0x1);

    // segment points in clip space
    vec4 p0_NDC = mul(u_modelViewProj, vec4(p0, 1.0));
    vec4 p1_NDC = mul(u_modelViewProj, vec4(p1, 1.0));

    vec3 n0_NDC = normalize(mul(u_normalMatrix, normal0));
    vec3 n1_NDC = normalize(mul(u_normalMatrix, normal1));

    // clip segment to near plane (w = 0) if needed
    vec4 clippedP0 = p0_NDC;
    vec4 clippedP1 = p1_NDC;

    vec4 clippedColor0 = color0;
    vec4 clippedColor1 = color1;

    vec3 clippedNormal0 = n0_NDC;
    vec3 clippedNormal1 = n1_NDC;

    if (p0_NDC.w < NEAR_EPSILON) {
        float t = (NEAR_EPSILON - p0_NDC.w) / (p1_NDC.w - p0_NDC.w);

        clippedP0 = mix(p0_NDC, p1_NDC, t);
        clippedColor0 = mix(color0, color1, t);
        clippedNormal0 = mix(normal0, normal1, t);
    }

    if (p1_NDC.w < NEAR_EPSILON) {
        float t = (NEAR_EPSILON - p1_NDC.w) / (p0_NDC.w - p1_NDC.w);

        clippedP1 = mix(p1_NDC, p0_NDC, t);
        clippedColor1 = mix(color1, color0, t);
        clippedNormal1 = mix(normal1, normal0, t);
    }

    // pick this vertex's position by uv.x along clipped segment */
    vec4 p = mix(clippedP0, clippedP1, uv.x);
    vec4 color = mix(clippedColor0, clippedColor1, uv.x);
    vec3 normal = mix(clippedNormal0, clippedNormal1, uv.x);

    // compute direction in NDC space (now safe since both points have w > 0) */
    vec2 ndp0 = clippedP0.xy / clippedP0.w;
    vec2 ndp1 = clippedP1.xy / clippedP1.w;
    vec2 screenDir = ndp1 - ndp0;
    float screenDirLen = length(screenDir);

    if (screenDirLen > LENGTH_EPSILON) {
        vec2 T = screenDir / screenDirLen;
        vec2 N = vec2(-T.y, T.x);
        
        float pixelsToNDCX = 2.0 / screenWidth;
        float pixelsToNDCY = 2.0 / screenHeight;
        
        float side = 2.0 * uv.y - 1.0;
        vec2 offsetNDC = N * (0.5 * thickness) * side * vec2(pixelsToNDCX, pixelsToNDCY);
        
        // Convert back to clip space
        vec2 newNDC = (p.xy / p.w) + offsetNDC;
        p.xy = newNDC * p.w;
    }

    if (colorToUse == 0)
        v_color = color;
    else if (colorToUse == 1) 
        v_color = lineColor;
    else
        v_color = generalColor;

    v_normal = normal;
    gl_Position = p;
}
