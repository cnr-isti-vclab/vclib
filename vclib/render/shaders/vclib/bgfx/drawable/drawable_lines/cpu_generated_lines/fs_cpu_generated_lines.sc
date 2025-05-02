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

$input v_color, v_uv, v_length, v_normal

#include <vclib/bgfx/drawable/lines.sh>

uniform vec4 u_data;

void main() {
    // uint thickness_antialias_border_caps = floatBitsToUint(u_data.y);
    // vec4 u_borderColor   = uintRGBAToVec4FloatColor(floatBitsToUint(u_data.z));

    // float u_thickness    = float((thickness_antialias_border_caps >> uint(24)) & uint(0xFF));
    // float u_antialias    = float((thickness_antialias_border_caps >> uint(16)) & uint(0xFF));
    // float u_border       = float((thickness_antialias_border_caps >> uint(8))  & uint(0xFF));
    
    // float u_leftCap      = float((thickness_antialias_border_caps >> uint(4))  & uint(0x3));
    // float u_rigthCap     = float((thickness_antialias_border_caps >> uint(2))  & uint(0x3));

    // vec4 color = calculateLinesColor(v_uv, v_length, u_thickness, u_antialias, u_border, u_leftCap, u_rigthCap, v_color, u_borderColor);

    // if(color.w == 0)
    //     discard;
    // else
    gl_FragColor = v_color;
}
