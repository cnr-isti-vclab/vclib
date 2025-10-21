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

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(face_selected, uint, 6);

void main() {
    gl_FragDepth = gl_FragCoord.z - 0.005;
    uint bufferIndex = uint(gl_PrimitiveID) / 32;
    uint bitMask = 0x1 << (31 - (uint(gl_PrimitiveID) % 32));
    if((face_selected[bufferIndex] & bitMask) != 0) {
        gl_FragColor = uintABGRToVec4Color(uint(0x330000FF));
    } else {
        gl_FragColor = uintABGRToVec4Color(uint(0x00000000));
    }
}