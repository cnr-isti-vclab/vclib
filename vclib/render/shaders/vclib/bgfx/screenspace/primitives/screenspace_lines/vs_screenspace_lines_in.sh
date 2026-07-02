// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

// Programmable Vertex Pulling: no vertex attributes, we pull data from SSBOs
$output v_color

#include <bgfx_shader.sh>
#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_lines_uniforms.sh>

// Input buffers (bound as compute buffers for vertex shader access)
BUFFER_RO(vertexPosBuffer, vec2, 0); // vertices
#ifdef SCREENSPACE_LINES_COLOR_PER_VERTEX
BUFFER_RO(vertexColBuffer, uint, 1); // vert colors
#endif
#ifdef SCREENSPACE_LINES_INDICES_ON
BUFFER_RO(indexBuffer, uint, 2);     // line indices
#endif
#ifdef SCREENSPACE_LINES_COLOR_PER_LINE
BUFFER_RO(lineColBuffer, uint, 3);   // line colors
#endif

// Helper function to get vertex index based on indexing mode
uint getVind(uint vind)
{
#ifdef SCREENSPACE_LINES_INDICES_ON
    return indexBuffer[vind];
#else
    return vind;
#endif
}

void main()
{
    // Calculate which line and which vertex of the quad we're processing
    // Each line (pair of endpoints) generates 6 vertices (2 triangles)
    uint lineIndex = gl_VertexID / 6u;
    uint localVertex = gl_VertexID % 6u;

    // Topology-based vertex indexing
#ifdef SCREENSPACE_LINES_TOPO_LINES
    uint vertexIndex0 = getVind(lineIndex * 2u);
    uint vertexIndex1 = getVind(lineIndex * 2u + 1u);
#else
    uint vertexIndex0 = getVind(lineIndex);
    uint vertexIndex1 = getVind(lineIndex + 1u);
#endif

    // Fetch the two endpoints of this line
    vec2 p0 = vertexPosBuffer[vertexIndex0];
    vec2 p1 = vertexPosBuffer[vertexIndex1];

    // Quad expansion: map localVertex (0-5) to quad corners and UVs
    // Triangle 1: verts 0, 1, 2
    // Triangle 2: verts 3, 4, 5
    const vec2 offsets[6] = {
        vec2(-1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0, -1.0),
        vec2( 1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0,  1.0)
    };
    const uint endpointIndices[6] = {
        0u, 0u, 1u,
        1u, 0u, 1u
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

    // Color selection based on compile-time macros
#if SCREENSPACE_LINES_COLOR_PER_VERTEX
    uint endpoint = endpointIndices[localVertex];
    uint vertIdx = endpoint == 0u ? vertexIndex0 : vertexIndex1;
    v_color = uintABGRToVec4Color(vertexColBuffer[vertIdx]);
#elif SCREENSPACE_LINES_COLOR_PER_LINE
    v_color = uintABGRToVec4Color(lineColBuffer[lineIndex]);
#else
    v_color = u_linesGeneralColor;
#endif
}
