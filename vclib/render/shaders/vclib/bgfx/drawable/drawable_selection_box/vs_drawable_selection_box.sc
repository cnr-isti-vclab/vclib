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

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

uniform vec4 u_selectionBox; // screen space

void main() {
    uint xIndex = uint(2) * (uint(gl_VertexID) / uint(2));
    uint yIndex = uint(1) + uint(2) * (uint(gl_VertexID) % uint(2));

    float xPos = u_selectionBox[xIndex];
    float yPos = u_selectionBox[yIndex];



    gl_Position = vec4((xPos - u_viewRect.x) / u_viewRect.z, (yPos - u_viewRect.y) / u_viewRect.w, 0, 1);
}