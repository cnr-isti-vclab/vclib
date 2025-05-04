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

BUFFER_RO(coordsBuffer,              vec4,  0);
BUFFER_RO(colorBuffer,               uint,  1);
BUFFER_RO(normalsBuffer,             vec4,  2);

BUFFER_WO(vertexBuffer,              vec4,  3);
BUFFER_WO(segmentsIndexBuffer,       uint,  4);
BUFFER_WO(jointsIndexBuffer,         uint,  5);

uniform vec4 u_numWorksGroups;
#define numWorksGroups u_numWorksGroups.x

#define get_float_from_vec4(pos, myBuffer) myBuffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)        vec3(get_float_from_vec4(((pos) * 3) + 0, coordsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 1, coordsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 2, coordsBuffer))

#define color(pos)    colorBuffer[pos]

#define normal(pos)   vec3(get_float_from_vec4(((pos) * 3) + 0, normalsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 1, normalsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 2, normalsBuffer))     


NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 16) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 4);
    uint actualPoint = gl_WorkGroupID.x + gl_LocalInvocationID.x;
    uint numPoints = numWorksGroups;

    vec3 prev       = p(actualPoint - sign(actualPoint));
    vec3 curr       = p(actualPoint);
    vec3 next       = p(actualPoint + sign(numPoints - actualPoint));
    uint color      = color(actualPoint);
    vec3 normal     = normal(actualPoint);

    vertexBuffer[baseIndex + 0] = vec4(prev.x, prev.y, prev.z, curr.x);
    vertexBuffer[baseIndex + 1] = vec4(curr.y, curr.z, next.x, next.y);
    vertexBuffer[baseIndex + 2] = vec4(next.z, uintBitsToFloat(color), normal.x, normal.y);
    vertexBuffer[baseIndex + 3] = vec4(normal.z, gl_LocalInvocationID.x, gl_LocalInvocationID.y, 0);


    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6)]     = (gl_WorkGroupID.x * 4);
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 1] = (gl_WorkGroupID.x * 4) + 3;
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 2] = (gl_WorkGroupID.x * 4) + 1;

        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 3] = (gl_WorkGroupID.x * 4);
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 4] = (gl_WorkGroupID.x * 4) + 2;
        segmentsIndexBuffer[(gl_WorkGroupID.x * 6) + 5] = (gl_WorkGroupID.x * 4) + 3;

        if(gl_WorkGroupID.x != numWorksGroups - 1) {
            jointsIndexBuffer[(gl_WorkGroupID.x * 6)]      = (gl_WorkGroupID.x * 4) + 3;
            jointsIndexBuffer[(gl_WorkGroupID.x * 6) + 1]  = (gl_WorkGroupID.x * 4) + 4;
            jointsIndexBuffer[(gl_WorkGroupID.x * 6) + 2]  = (gl_WorkGroupID.x * 4) + 5;

            jointsIndexBuffer[(gl_WorkGroupID.x * 6) + 3]  = (gl_WorkGroupID.x * 4) + 4;
            jointsIndexBuffer[(gl_WorkGroupID.x * 6) + 4]  = (gl_WorkGroupID.x * 4) + 2;
            jointsIndexBuffer[(gl_WorkGroupID.x * 6) + 5]  = (gl_WorkGroupID.x * 4) + 5;
        }
    }
}
