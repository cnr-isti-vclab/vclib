/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

$output v_color

#include <shaders_common.sh>

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

    // Compute the color
    if (gl_VertexID < 64) // x
        v_color = vec4(1.0, 0.0, 0.0, 1.0);
    else if (gl_VertexID < 128) // y
        v_color = vec4(0.0, 1.0, 0.0, 1.0);
    else // z
        v_color = vec4(0.0, 0.0, 1.0, 1.0);
}
