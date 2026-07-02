// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$output v_color, v_texcoord1

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.sh>

// Input buffers (bound as compute buffers for vertex shader access)
BUFFER_RO(pointsBuffer, vec2, 0); // 2D point positions
#if POINTS_COLOR_PER_VERTEX
BUFFER_RO(pointColors, uint, 1); // colors
#endif

void main()
{
    // Calculate which point and which vertex of the quad we're processing
    // Each point generates 6 vertices (2 triangles)
    uint pointIndex = gl_VertexID / 6u;
    uint localVertex = gl_VertexID % 6u;

    // Fetch the center position of this point
    vec2 centerPos = pointsBuffer[pointIndex];

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

#ifdef POINTS_COLOR_PER_VERTEX
    v_color = uintABGRToVec4Color(pointColors[pointIndex]);
#else
    v_color = u_pointsGeneralColor;
#endif

    // Pass UV coordinates to fragment shader
    v_texcoord1 = quadUv;
}
