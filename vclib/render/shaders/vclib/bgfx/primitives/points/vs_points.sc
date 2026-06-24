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

$output v_normal, v_texcoord1, v_color

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/primitives/uniforms/points_uniforms.sh>

BUFFER_RO(pointsBuffer, vec4, 0); // 3D point positions
BUFFER_RO(normalsBuffer, vec4, 1); // 3D normals
BUFFER_RO(pointColors, uint, 2); // colors

void main()
{
    // Calculate which point and which vertex of the quad we're processing
    // Each point generates 6 vertices (2 triangles)
    uint pointIndex = gl_VertexID / 6u;
    uint localVertex = gl_VertexID % 6u;

    uint idx30 = pointIndex * 3u;
    uint idx31 = idx30 + 1u;
    uint idx32 = idx30 + 2u;

    vec3 centerPos = vec3(
        pointsBuffer[idx30/4u][idx30%4u],
        pointsBuffer[idx31/4u][idx31%4u],
        pointsBuffer[idx32/4u][idx32%4u]);

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

    gl_Position = pos + offset;

    // Normal calculation
    vec3 normal = vec3(0.0, 0.0, 0.0);
    if (!useNoneShading()) {
        normal = vec3(
            normalsBuffer[idx30/4u][idx30%4u],
            normalsBuffer[idx31/4u][idx31%4u],
            normalsBuffer[idx32/4u][idx32%4u]);
    }
    v_normal = normalize(mul(u_normalMatrix, normal));

    // Color calculation
    v_color = u_pointsGeneralColor;
    if (usePerPointColor()) {
        v_color = uintABGRToVec4Color(pointColors[pointIndex]);
    }

    // Pass UV coordinates to fragment shader
    v_texcoord1 = quadUv;
}
