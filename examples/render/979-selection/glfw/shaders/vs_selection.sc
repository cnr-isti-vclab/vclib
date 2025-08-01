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

$input a_position, a_color0
$output v_color

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(vertex_selected, vec4, 4);

void main() {
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

    uint pointId = uint(gl_VertexID);
    uint bufferIndex = pointId/128;
    uint vec4Index = (pointId%128)/32;
    uint bitOffset = 31-(pointId%32);
    uint bitMask = 0x1 << bitOffset;
    if ((floatBitsToUint(vertex_selected[bufferIndex][vec4Index]) & bitMask) == 0) {
        v_color = a_color0;
    } else {
        v_color = uintABGRToVec4Color(uint(0xFF0000FF));
    }
}