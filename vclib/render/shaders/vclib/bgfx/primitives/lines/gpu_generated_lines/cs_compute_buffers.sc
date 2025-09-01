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

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

#include <vclib/bgfx/shaders_common.sh> 

BUFFER_RO(lineIndex,         uvec4, 0);
BUFFER_RO(vertCoords,        vec4,  1);
BUFFER_RO(vertNormals,       vec4,  2);
BUFFER_RO(vertColors,        uvec4, 3);
BUFFER_RO(lineColor,         uvec4, 4);

BUFFER_WO(vertexBuffer,      vec4,  5);
BUFFER_WO(indexBuffer,       uvec4, 6); 

uniform vec4 u_CustomIndices;

#define setIndices          bool(u_CustomIndices.x)

#define get_float_from_vec4(pos, Buffer) Buffer[uint(pos) / 4][uint(pos) % 4]


#define p(pos)             vec3(get_float_from_vec4(((pos) * 3) + 0, vertCoords), \
                                get_float_from_vec4(((pos) * 3) + 1, vertCoords), \
                                get_float_from_vec4(((pos) * 3) + 2, vertCoords))

#define normal(pos)        vec3(get_float_from_vec4(((pos) * 3) + 0, vertNormals), \
                                get_float_from_vec4(((pos) * 3) + 1, vertNormals), \
                                get_float_from_vec4(((pos) * 3) + 2, vertNormals))     

#define color(pos)         get_float_from_vec4(pos, vertColors)
#define lineColor(pos)     get_float_from_vec4(pos, lineColor)
#define index(pos)         get_float_from_vec4(pos, lineIndex)

NUM_THREADS(2, 2, 1)
void main() {
    uint index0 = (gl_WorkGroupID * 2);
    uint index1 = (gl_WorkGroupID * 2) + 1;
    if(setIndices) {
        index0 = index((gl_WorkGroupID * 2));
        index1 = index((gl_WorkGroupID * 2) + 1);
    }

    uint baseIndex = (gl_WorkGroupID.x * 60) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 15);

    vec3 p0         = p(index0);
    vec3 p1         = p(index1);
    uint color0     = color(index0);
    uint color1     = color(index1);
    vec3 normal0    = normal(index0);
    vec3 normal1    = normal(index1);

    uint lineColor  = lineColor(gl_WorkGroupID.x);


    get_float_from_vec4(baseIndex,     vertexBuffer) = p0.x;
    get_float_from_vec4(baseIndex + 1, vertexBuffer) = p0.y;
    get_float_from_vec4(baseIndex + 2, vertexBuffer) = p0.z;

    get_float_from_vec4(baseIndex + 3, vertexBuffer) = p1.x;
    get_float_from_vec4(baseIndex + 4, vertexBuffer) = p1.y;
    get_float_from_vec4(baseIndex + 5, vertexBuffer) = p1.z;

    get_float_from_vec4(baseIndex + 6, vertexBuffer) = uintBitsToFloat(color0);
    get_float_from_vec4(baseIndex + 7, vertexBuffer) = uintBitsToFloat(color1);

    get_float_from_vec4(baseIndex + 8, vertexBuffer) = normal0.x;
    get_float_from_vec4(baseIndex + 9, vertexBuffer) = normal0.y;
    get_float_from_vec4(baseIndex + 10, vertexBuffer) = normal0.z;

    get_float_from_vec4(baseIndex + 11, vertexBuffer) = normal1.x;
    get_float_from_vec4(baseIndex + 12, vertexBuffer) = normal1.y;
    get_float_from_vec4(baseIndex + 13, vertexBuffer) = normal1.z;

    get_float_from_vec4(baseIndex + 14, vertexBuffer) = uintBitsToFloat(lineColor);


    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 0, indexBuffer) = (gl_WorkGroupID.x * 4);
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 1, indexBuffer) = (gl_WorkGroupID.x * 4) + 3;
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 2, indexBuffer) = (gl_WorkGroupID.x * 4) + 1;

        get_float_from_vec4((6 * gl_WorkGroupID.x) + 3, indexBuffer) = (gl_WorkGroupID.x * 4);
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 4, indexBuffer) = (gl_WorkGroupID.x * 4) + 2;
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 5, indexBuffer) = (gl_WorkGroupID.x * 4) + 3;
    }

}