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

#ifndef VCL_BGFX_DRAWABLE_LINES_UTILS_SH
#define VCL_BGFX_DRAWABLE_LINES_UTILS_SH

#include <bgfx_shader.sh>

vec4 screenToClip(vec4 coordinate, float screen_width, float screen_heigth) {
    return vec4(
                  (2 * coordinate.x / screen_width), 
                  (2 * coordinate.y / screen_heigth), 
                  coordinate.z, 
                  coordinate.w
                );
}

vec4 clipToScreen(vec4 coordinate, float screen_width, float screen_heigth) {
    return vec4(
                  (coordinate.x * screen_width) / 2, 
                  (coordinate.y * screen_heigth) / 2, 
                  coordinate.z, 
                  coordinate.w
                );
}

vec4 calculatePointWithMVP(vec4 p, float screen_width, float screen_heigth) {
    vec4 NDC_p = mul(u_modelViewProj, vec4(p.xyz, 1.0));

    vec4 screen_p = vec4(((NDC_p.xy / NDC_p.w)).xy, 0.0, 0.0);

    vec4 p_px = clipToScreen(screen_p, screen_width, screen_heigth);
    return vec4(p_px.xy, NDC_p.z, NDC_p.w);
}

vec4 uintToVec4FloatColor(uint color) {
    return vec4(
        float((color >> uint(24)) & uint(0xFF)) / 255,
        float((color >> uint(16)) & uint(0xFF)) / 255,
        float((color >> uint(8))  & uint(0xFF)) / 255,
        float( color              & uint(0xFF)) / 255
    );
}

ivec2 calculateTextureCoord(uint index, uint maxTextureSize) {
    uint Y = index / maxTextureSize;
    uint X = index - (Y * maxTextureSize);
    return ivec2(X, Y);
}

#endif // VCL_BGFX_DRAWABLE_LINES_UTILS_SH
