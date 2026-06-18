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
$output v_color

#include <bgfx_shader.sh>
#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_lines_uniforms.sh>

// Input buffers (bound as compute buffers for vertex shader access)
BUFFER_RO(linesBuffer, vec2, 0); // 2D line endpoint positions (pairs of vertices)

void main()
{
    // Calculate which line and which vertex of the quad we're processing
    // Each line (pair of endpoints) generates 6 vertices (2 triangles)
    uint lineIndex = gl_VertexID / 6u;
    uint localVertex = gl_VertexID % 6u;

    // Fetch the two endpoints of this line
    vec2 p0 = linesBuffer[lineIndex * 2u];
    vec2 p1 = linesBuffer[lineIndex * 2u + 1u];

    // Quad expansion: map localVertex (0-5) to quad corners and UVs
    // Triangle 1: verts 0, 1, 2
    // Triangle 2: verts 3, 4, 5
    const vec2 offsets[6] = {
        vec2(-1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0, -1.0),
        vec2( 1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0,  1.0)
    };

    vec2 offset = offsets[localVertex];
    vec2 uv = offset * 0.5 + 0.5; // UV coordinates in [0,1]

    // Compute line direction in screen space
    vec2 screenDir = p1 - p0;
    float screenDirLen = length(screenDir);

    // Avoid division by zero for degenerate lines
    if (screenDirLen > 0.0001) {
        // Compute normal (perpendicular) to the line direction
        vec2 T = screenDir / screenDirLen;  // Tangent
        vec2 N = vec2(-T.y, T.x);           // Normal (90° rotation)

        // Position along the line (uv.x: 0 = p0, 1 = p1)
        vec2 linePos = mix(p0, p1, uv.x);

        // Side offset (-1 = one side, 1 = other side)
        float side = 2.0 * uv.y - 1.0;

        // Width is in pixels, screen coordinates are also in pixels
        // So we can apply the offset directly in screenspace
        vec2 offsetScreen = N * (0.5 * u_linesWidth) * side;

        // Final position in screenspace
        vec2 finalPos = linePos + offsetScreen;

        // Convert from screen space (0..1 relative to viewRect) to clip space (-1..1)
        gl_Position = vec4(
            (finalPos.x - u_viewRect.x) / u_viewRect.z * 2.0 - 1.0,
            1.0 - (finalPos.y - u_viewRect.y) / u_viewRect.w * 2.0,
            0.0,
            1.0);
    } else {
        // Degenerate line: render as a point at p0
        gl_Position = vec4(
            (p0.x - u_viewRect.x) / u_viewRect.z * 2.0 - 1.0,
            1.0 - (p0.y - u_viewRect.y) / u_viewRect.w * 2.0,
            0.0,
            1.0);
    }


    //if (usePerVertexColor())
        // TODO
    //else
        v_color = u_linesGeneralColor;
}
