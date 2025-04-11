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

$output v_normal, v_color

#include <vclib/bgfx/drawable/drawable_trackball/uniforms.sh>

void main()
{
    uint nVerticesPerCircle = floatBitsToUint(u_nVerticesPerCircleFloat);
    uint isDragging = floatBitsToUint(u_isDraggingFloat);
    float alpha = isDragging != 0u ? 0.9f : 0.5f;

    v_normal = normalize(mul(u_model[0], vec4(a_position, 0.0)).xyz);
    gl_Position = mul(u_proj, mul(u_model[0], vec4(a_position, 1.0)));

    // Compute the color
    if (gl_VertexID < nVerticesPerCircle) // x
        v_color = vec4(1.0, 0.0, 0.0, alpha);
    else if (gl_VertexID < nVerticesPerCircle * 2) // y
        v_color = vec4(0.0, 1.0, 0.0, alpha);
    else // z
        v_color = vec4(0.0, 0.0, 1.0, alpha);
}
