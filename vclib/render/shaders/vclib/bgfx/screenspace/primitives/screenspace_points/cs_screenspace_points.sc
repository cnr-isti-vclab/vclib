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

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.sh>

BUFFER_RO(points, vec4, 0); // packed screen-space coords: points[i*2].xy + points[i*2+1].zw
BUFFER_WO(vOut, vec4, 2);   // per-splat vertex data (x, y, z, padding)

NUM_THREADS(1, 1, 1) // 1 'thread' per point
void main()
{
    uint pointId = gl_WorkGroupID.x;
    uint vecIdx  = pointId / 2u;
    vec2 p       = (pointId & 1u) ? points[vecIdx].zw : points[vecIdx].xy;

    UNROLL
    for (int i = 0; i < 4; ++i) {
        uint vertexId = pointId * 4u + uint(i);
        vec2 quadUv = vec2(uint(i) & 1u, (uint(i) >> 1u) & 1u);
        vec2 offset = (2.0 * quadUv - vec2(1.0, 1.0)) * u_pointsWidth;
        vOut[vertexId] = vec4(p + offset, 0.0, 0.0);
    }
}
