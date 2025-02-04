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

#include <bgfx_compute.sh>

#include "../../utils.sh"

BUFFER_RO(pointsBuffer,      float,  0);
BUFFER_WO(vertexBuffer,      float,  1);
BUFFER_WO(indexBuffer,       uint,   2);

#define p(pos)        vec3(pointsBuffer[((pos) * 7) + 0], pointsBuffer[((pos) * 7) + 1], pointsBuffer[((pos) * 7) + 2])
#define color(pos)    pointsBuffer[((pos) * 7) + 3]
#define normal(pos)   vec3(pointsBuffer[((pos) * 7) + 4], pointsBuffer[((pos) * 7) + 5], pointsBuffer[((pos) * 7) + 6])     

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 48) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 12);

    vec3 p0        = p((gl_WorkGroupID.x * 2));
    vec3 p1        = p((gl_WorkGroupID.x * 2) + 1);
    float color    = color((gl_WorkGroupID.x * 2) + (1 - ((uint)(gl_LocalInvocationID.x + 1) % 2)));
    vec3 normal    = normal((gl_WorkGroupID.x * 2) + (1 - ((uint)(gl_LocalInvocationID.x + 1) % 2)));

    vertexBuffer[baseIndex]     = p0.x;
    vertexBuffer[baseIndex + 1] = p0.y;
    vertexBuffer[baseIndex + 2] = p0.z;

    vertexBuffer[baseIndex + 3] = p1.x;
    vertexBuffer[baseIndex + 4] = p1.y;
    vertexBuffer[baseIndex + 5] = p1.z;

    vertexBuffer[baseIndex + 6] = uintBitsToFloat(bitfieldReverse(floatBitsToUint(color)));

    vertexBuffer[baseIndex + 7] = normal.x;
    vertexBuffer[baseIndex + 8] = normal.y;
    vertexBuffer[baseIndex + 9] = normal.z;

    vertexBuffer[baseIndex + 10] = gl_LocalInvocationID.x;
    vertexBuffer[baseIndex + 11] = gl_LocalInvocationID.y;

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        indexBuffer[(6 * gl_WorkGroupID.x) + 0] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 1] = (gl_WorkGroupID.x * 4) + 3;
        indexBuffer[(6 * gl_WorkGroupID.x) + 2] = (gl_WorkGroupID.x * 4) + 1;

        indexBuffer[(6 * gl_WorkGroupID.x) + 3] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 4] = (gl_WorkGroupID.x * 4) + 2;
        indexBuffer[(6 * gl_WorkGroupID.x) + 5] = (gl_WorkGroupID.x * 4) + 3;
    }

}
