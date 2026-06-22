/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

// Programmable Vertex Pulling: no vertex attributes, we pull data from SSBOs
$output v_color, v_texcoord1

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.sh>

// Input buffers (bound as compute buffers for vertex shader access)
BUFFER_RO(pointsBuffer, vec2, 0); // 2D point positions
BUFFER_RO(pointColors, uint, 1); // colors

void main()
{
    // Calculate which point and which vertex of the quad we're processing
    // Each point generates 6 vertices (2 triangles)
    uint pointIndex = gl_VertexID / 6u;
    uint localVertex = gl_VertexID % 6u;

    // Fetch the center position of this point
    vec2 centerPos = pointsBuffer[pointIndex];

    // Quad expansion: map localVertex (0-5) to quad corners and UVs
    // Triangle 1: verts 0, 1, 2
    // Triangle 2: verts 3, 4, 5
    // Vertex offset mapping for each of the 6 vertices:
    // localVertex=0: (-1, -1) -> UV (0, 0)   [bottom-left]
    // localVertex=1: (-1,  1) -> UV (0, 1)   [top-left]
    // localVertex=2: ( 1, -1) -> UV (1, 0)   [bottom-right]
    // localVertex=3: ( 1, -1) -> UV (1, 0)   [bottom-right]
    // localVertex=4: (-1,  1) -> UV (0, 1)   [top-left]
    // localVertex=5: ( 1,  1) -> UV (1, 1)   [top-right]
    
    const vec2 offsets[6] = {
        vec2(-1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0, -1.0),
        vec2( 1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0,  1.0)
    };

    vec2 offset = offsets[localVertex];
    vec2 quadUv = offset * 0.5 + 0.5;

    // Apply the splat width to the offset
    offset *= u_pointsWidth;

    // Calculate world position: center + offset
    vec2 vLocalPos = centerPos + offset;

    // Convert from screen space (0..1) to clip space (-1..1)
    gl_Position = vec4(
        (vLocalPos.x - u_viewRect.x) / u_viewRect.z * 2.0 - 1.0,
        1.0 - (vLocalPos.y - u_viewRect.y) / u_viewRect.w * 2.0,
        0.0,
        1.0);

    v_color = u_pointsGeneralColor;

    if (usePerPointColor()) {
        v_color = uintABGRToVec4Color(pointColors[pointIndex]);
    }

    // Pass UV coordinates to fragment shader
    v_texcoord1 = quadUv;
}
