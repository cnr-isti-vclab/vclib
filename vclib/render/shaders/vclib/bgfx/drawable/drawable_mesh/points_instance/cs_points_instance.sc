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

// convert vec4 color to uint
uint vec4ColToUint(vec4 color)
{
    return uint(color.x * 255.0) << 24 |
           uint(color.y * 255.0) << 16 |
           uint(color.z * 255.0) << 8  |
           uint(color.w * 255.0);
};

BUFFER_RO(position, vec3, 0); // coordinates
BUFFER_RO(normals,  vec3, 1); // normals
BUFFER_RO(colors,   vec4, 2); // colors

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
        vOut[attrPosCol].xyz  = position[pointId];
        vOut[attrPosCol].w    = uintBitsToFloat(vec4ColToUint(colors[pointId]));
        vOut[attrNormSca].xyz = normals[pointId];
    }
}
