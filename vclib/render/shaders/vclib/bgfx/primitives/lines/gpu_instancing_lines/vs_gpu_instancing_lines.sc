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
$output v_color, v_normal

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

#include <vclib/bgfx/shaders_common.sh> 

// read buffers must be read as vec4 arrays to ensure proper alignment on directx
BUFFER_RO(vertCoords,           vec4,  0);
BUFFER_RO(vertNormals,          vec4,  1);
BUFFER_RO(vertColors,           vec4,  2);
BUFFER_RO(lineIndex,            uvec4, 3);

uniform vec4 u_settings;

#define NEAR_EPSILON 0.001
#define LENGTH_EPSILON 0.0001

#define thickness             u_settings.x

#define uv                    a_position.xy

#define screenWidth           u_viewRect.z
#define screenHeight          u_viewRect.w


#define get_float_from_vec4(pos, Buffer) Buffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)             vec3(get_float_from_vec4(((pos) * 3) + 0, vertCoords), \
                                get_float_from_vec4(((pos) * 3) + 1, vertCoords), \
                                get_float_from_vec4(((pos) * 3) + 2, vertCoords))

#define normal(pos)        vec3(get_float_from_vec4(((pos) * 3) + 0, vertNormals), \
                                get_float_from_vec4(((pos) * 3) + 1, vertNormals), \
                                get_float_from_vec4(((pos) * 3) + 2, vertNormals))     

#define color(pos)         get_float_from_vec4(pos, vertColors)
#define index(pos)         get_float_from_vec4(pos, lineIndex)


uniform vec4 u_ActiveBuffers; // x = line indices, y = vertex normals, z = vertex colors, w = line colors
#define setIndices          (u_ActiveBuffers.x == 1)
#define setVertexNormals    (u_ActiveBuffers.y == 1)
#define setVertexColors     (u_ActiveBuffers.z == 1)
#define setLineColors       (u_ActiveBuffers.w == 1)

void main() {
    uint index0 = (gl_InstanceID * 2);
    uint index1 = (gl_InstanceID * 2) + 1;
    if(setIndices) {
        index0 = index((gl_InstanceID * 2));
        index1 = index((gl_InstanceID * 2) + 1);
    }

    vec3 p0 = p(index0);
    vec3 p1 = p(index1);

    vec4 color0 = vec4_splat(1.0);
    vec4 color1 = vec4_splat(1.0);
    if(setVertexColors) {
        color0 = uintABGRToVec4Color(floatBitsToUint(color(index0)));
        color1 = uintABGRToVec4Color(floatBitsToUint(color(index1)));
    }

    vec3 normal0 = vec3(0.0, 0.0, 1.0);
    vec3 normal1 = vec3(0.0, 0.0, 1.0);
    if(setVertexNormals) {
        normal0 = normal(index0);
        normal1 = normal(index1);
    }

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
        p.xy = p.xy + (offsetNDC * p.w);
    }

    v_color = color;
    v_normal = normal;
    gl_Position = p;
}
