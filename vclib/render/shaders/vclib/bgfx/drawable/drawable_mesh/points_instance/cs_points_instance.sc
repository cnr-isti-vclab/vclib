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

#include <vclib/bgfx/shaders_common.sh>

BUFFER_RO(positions, float, 0); // coordinates (3 floats)
BUFFER_RO(normals,  float, 1); // normals (3 floats)
BUFFER_RO(colors,   uint, 2); // colors (4 floats)

BUFFER_WO(vOut, vec4, 4); // output vertices


NUM_THREADS(1, 1, 1) // 1 thread per point
void main()
{
    uint pointId = gl_GlobalInvocationID.x;

    // Generate quad vertices
    for (int i = 0; i < 4; ++i) {
        uint vertexId = pointId * 4 + i;
        uint attrPosCol  = vertexId * 2;   // pos 3, col 1
        uint attrNormSca = attrPosCol + 1; // norm 3, sca 1
        vec4 col = colors[pointId];
        vec3 p = vec3(
            positions[pointId * 3],
            positions[pointId * 3 + 1],
            positions[pointId * 3 + 2]);
        vec3 n = vec3(
            normals[pointId * 3],
            normals[pointId * 3 + 1],
            normals[pointId * 3 + 2]);
        vOut[attrPosCol]  = vec4(
            p,
            uintBitsToFloat(colors[pointId]));
        vOut[attrNormSca] = vec4(n, 1.0);
    }
}
