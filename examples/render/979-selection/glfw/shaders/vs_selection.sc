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

$input a_position, a_normal, a_color0, a_texcoord0, a_texcoord1
$output v_selected

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>

BUFFER_RO(vertex_selected, uint, 4);

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    int bufferIndex = gl_VertexID/32;
    int bitOffset = 31 - gl_VertexID%32;
    int bitMask = 0x1 << bitOffset;
    if ((vertex_selected[bufferIndex] & bitMask) != 0) {
        v_selected = 1;
    } else {
        v_selected = 0;
    }
}
