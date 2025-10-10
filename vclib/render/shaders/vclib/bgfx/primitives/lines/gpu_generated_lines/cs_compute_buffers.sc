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

BUFFER_RO(vertCoords,        vec4,  0);
BUFFER_RO(lineIndex,         uvec4, 1);
BUFFER_RO(vertNormals,       vec4,  2);
BUFFER_RO(vertColors,        uvec4, 3);

BUFFER_WO(vertexBuffer,      vec4,  4);
BUFFER_WO(indexBuffer,       uint,  5);

uniform vec4 u_ActiveBuffers; // x = line indices, y = normals, z = colors, w = line colors

#define setIndices          (u_ActiveBuffers.x == 1)
#define setNormals          (u_ActiveBuffers.y == 1)
#define setColors           (u_ActiveBuffers.z == 1)

#define get_float_from_vec4(pos, Buffer) Buffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)             vec3(get_float_from_vec4(((pos) * 3) + 0, vertCoords), \
                                get_float_from_vec4(((pos) * 3) + 1, vertCoords), \
                                get_float_from_vec4(((pos) * 3) + 2, vertCoords))

#define normal(pos)        vec3(get_float_from_vec4(((pos) * 3) + 0, vertNormals), \
                                get_float_from_vec4(((pos) * 3) + 1, vertNormals), \
                                get_float_from_vec4(((pos) * 3) + 2, vertNormals))     

#define color(pos)         get_float_from_vec4(pos, vertColors)
#define index(pos)         get_float_from_vec4(pos, lineIndex)

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 16) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 4);
    uint index0 = (gl_WorkGroupID.x * 2);
    uint index1 = (gl_WorkGroupID.x * 2) + 1;

    if(setIndices) {
        index0 = index((gl_WorkGroupID.x * 2));
        index1 = index((gl_WorkGroupID.x * 2) + 1);
    }

    vec3 p0 = vec3(vertCoords[0][0],
                   vertCoords[0][1],
                   vertCoords[0][2]);

    vec3 p1 = vec3(vertCoords[0][3],
                   vertCoords[0][3],
                   vertCoords[1][1]);

    // p0 = p(index0);
    // p1 = p(index1);

    // p0 = vec3(0.0, 1.0, 0.0);
    // p1 = vec3(1.0, 1.0, 0.0);             

    uint color0 = 0xff0000ff;
    uint color1 = 0xff0000ff;
    if(setColors) {
        color0 = color(index0);
        color1 = color(index1);
    }

    vec3 normal0 = vec3(0.0, 0.0, 1.0);
    vec3 normal1 = vec3(0.0, 0.0, 1.0);
    if(setNormals) {
        normal0 = normal(index0);
        normal1 = normal(index1);
    }

    vertexBuffer[baseIndex]     = vec4(p0.x, p0.y, p0.z, p1.x);
    vertexBuffer[baseIndex + 1] = vec4(p1.y, p1.z, uintBitsToFloat(color0), uintBitsToFloat(color1));
    vertexBuffer[baseIndex + 2] = vec4(normal0.x, normal0.y, normal0.z, normal1.x);
    vertexBuffer[baseIndex + 3] = vec4(normal1.y, normal1.z, 0, 0);

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        indexBuffer[(6 * gl_WorkGroupID.x) + 0] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 1] = (gl_WorkGroupID.x * 4) + 3;
        indexBuffer[(6 * gl_WorkGroupID.x) + 2] = (gl_WorkGroupID.x * 4) + 1;

        indexBuffer[(6 * gl_WorkGroupID.x) + 3] = (gl_WorkGroupID.x * 4);
        indexBuffer[(6 * gl_WorkGroupID.x) + 4] = (gl_WorkGroupID.x * 4) + 2;
        indexBuffer[(6 * gl_WorkGroupID.x) + 5] = (gl_WorkGroupID.x * 4) + 3;
    }

}