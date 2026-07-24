// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$output v_normal, v_texcoord0, v_color, v_selected
$output v_worldPos, v_discardFlag

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/buffers/boolean_buffer.sh>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>
#include <vclib/bgfx/primitives/uniforms/points_uniforms.sh>

BUFFER_RO(pointsBuffer, vec4, 0); // 3D point positions
DECLARE_FETCH_VEC3(fetchPosition, pointsBuffer);

#if POINTS_SHADING_PER_VERTEX
BUFFER_RO(normalsBuffer, vec4, 1); // 3D normals
DECLARE_FETCH_VEC3(fetchNormal, normalsBuffer);
#endif

#if POINTS_COLOR_PER_VERTEX
BUFFER_RO(pointColors, uint, 2); // colors
#endif

#if POINTS_SELECTION_ON
BUFFER_RO(vertexSelected, uint, 3);
#endif

void main()
{
    // Calculate which point and which vertex of the quad we are processing
    // Each point generates 6 vertices (2 triangles)
    uint pointIndex = gl_VertexID / 6u;
    uint localVertex = gl_VertexID % 6u;

    vec3 centerPos = fetchPosition(pointIndex);

    const vec2 offsets[6] = {
        vec2(-1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0, -1.0),
        vec2( 1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0,  1.0)
    };

    vec2 quadUv = offsets[localVertex] * 0.5 + 0.5;

    vec4 pos = mul(u_modelViewProj, vec4(centerPos, 1.0));
    
    vec4 offset = vec4(
        offsets[localVertex].x * u_pointsWidth * u_viewTexel.x * pos.w,
        offsets[localVertex].y * u_pointsWidth * u_viewTexel.y * pos.w,
        0.0, 0.0);

    pos += offset;
    // Apply depth offset in clip space.
    // We scale the offset by w to maintain it consistently after the perspective divide.
    pos.z += -u_depthOffset * pos.w;
    gl_Position = pos;

    // Normal calculation
#if POINTS_SHADING_PER_VERTEX
    v_normal = fetchNormal(pointIndex);
    v_normal = normalize(mul(u_normalMatrix, v_normal));
#endif

    // Color calculation
#if POINTS_COLOR_PER_VERTEX
    v_color = uintABGRToVec4Color(pointColors[pointIndex]);
#else
    v_color = u_pointsGeneralColor;
#endif

    // Pass UV coordinates to fragment shader
    v_texcoord0 = quadUv;

#if POINTS_SELECTION_ON
    v_selected = float(getBoolFromBuffer(vertexSelected, pointIndex));
#endif

#if defined(POINTS_SECTION_ON) || defined(POINTS_ID_SECTION_ON)
    v_worldPos = mul(u_model[0], vec4(centerPos, 1.0)).xyz;
    v_discardFlag = computeDiscardFlag(v_worldPos);
#else
    v_discardFlag = 1.0;
#endif
}
