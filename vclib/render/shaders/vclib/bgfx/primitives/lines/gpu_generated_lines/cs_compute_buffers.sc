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

BUFFER_RO(coords,            vec4,  0);
BUFFER_RO(lineIndex,         uvec4, 1);
BUFFER_RO(normals,           vec4,  2);
BUFFER_RO(colors,            uvec4, 3);

BUFFER_WO(vertexCoords,      vec4,  4);
BUFFER_WO(vertexNormals,     vec4,  5);
BUFFER_WO(vertexColors,      vec4,  6);
BUFFER_WO(indexBuffer,       uint,  7);

uniform vec4 u_ActiveBuffers; // x = line indices, y = normals, z = colors, w = num segments

#define numThreadsPerGroup 512

#define setIndices          (u_ActiveBuffers.x == 1)
#define setNormals          (u_ActiveBuffers.y == 1)
#define setColors           (u_ActiveBuffers.z == 1)
#define numSegments         (uint(u_ActiveBuffers.w))

#define get_float_from_vec4(pos, Buffer) Buffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)             vec3(get_float_from_vec4(((pos) * 3) + 0, coords), \
                                get_float_from_vec4(((pos) * 3) + 1, coords), \
                                get_float_from_vec4(((pos) * 3) + 2, coords))

#define normal(pos)        vec3(get_float_from_vec4(((pos) * 3) + 0, normals), \
                                get_float_from_vec4(((pos) * 3) + 1, normals), \
                                get_float_from_vec4(((pos) * 3) + 2, normals))     

#define color(pos)         get_float_from_vec4(pos, colors)
#define index(pos)         get_float_from_vec4(pos, lineIndex)

NUM_THREADS(numThreadsPerGroup, 1, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * numThreadsPerGroup) + gl_LocalInvocationID.x;
    if (baseIndex >= numSegments) {
        return;
    }

    uint index0 = (baseIndex * 2);
    uint index1 = (baseIndex * 2) + 1;
    if(setIndices) {
        index0 = index((baseIndex * 2));
        index1 = index((baseIndex * 2) + 1);
    }  
    
    float color0 = uintBitsToFloat(0xffffffff);
    float color1 = uintBitsToFloat(0xffffffff);
    if(setColors) {
        color0 = uintBitsToFloat(0xff0000ff);
        color1 = uintBitsToFloat(0xff0000ff);
    }

    vec3 normal0 = vec3(0.0, 0.0, 1.0);
    vec3 normal1 = vec3(0.0, 0.0, 1.0);
    if(setNormals) {    
        normal0 = normal(index0);
        normal1 = normal(index1);
    }

    vec3 p0 = p(index0);
    vec3 p1 = p(index1);     

    UNROLL
    for (int i = 0; i < 2; ++i) {   
        vertexCoords[(baseIndex * 6) + (i * 3)]      = vec4(p0.x, p0.y, p0.z, p1.x);
        vertexCoords[(baseIndex * 6) + (i * 3) + 1]  = vec4(p1.y, p1.z, p0.x, p0.y);
        vertexCoords[(baseIndex * 6) + (i * 3) + 2]  = vec4(p0.z, p1.x, p1.y, p1.z);

        vertexNormals[(baseIndex * 6) + (i * 3)]     = vec4(normal0.x, normal0.y, normal0.z, normal1.x);
        vertexNormals[(baseIndex * 6) + (i * 3) + 1] = vec4(normal1.y, normal1.z, normal0.x, normal0.y);
        vertexNormals[(baseIndex * 6) + (i * 3) + 2] = vec4(normal0.z, normal1.x, normal1.y, normal1.z);

        vertexColors[(baseIndex * 2) + (i * 1)]      = vec4(color0, color1, color0, color1);
    }

    indexBuffer[(6 * baseIndex) + 0] = (baseIndex * 4);
    indexBuffer[(6 * baseIndex) + 1] = (baseIndex * 4) + 3;
    indexBuffer[(6 * baseIndex) + 2] = (baseIndex * 4) + 1;

    indexBuffer[(6 * baseIndex) + 3] = (baseIndex * 4);
    indexBuffer[(6 * baseIndex) + 4] = (baseIndex * 4) + 2;
    indexBuffer[(6 * baseIndex) + 5] = (baseIndex * 4) + 3;
}