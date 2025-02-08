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
#include <vclib/bgfx/drawable/utils.sh>

#include <bgfx_compute.sh>

BUFFER_RO(pointsBuffer,             float,    0);
IMAGE2D_WO(textureBufferSegments,   rgba32f,  1);
IMAGE2D_WO(textureBufferJoints,     rgba32f,  2);
BUFFER_WO(segmentsIndirectBuffer,   uvec4,    3);
BUFFER_WO(jointsIndirectBuffer,     uvec4,    4);

uniform vec4 u_IndirectData;
#define maxTextureSize          u_IndirectData.x
#define maxInstanceSize         u_IndirectData.y

#define p(pos)        vec3(pointsBuffer[((pos) * 7) + 0], pointsBuffer[((pos) * 7) + 1], pointsBuffer[((pos) * 7) + 2])
#define color(pos)    pointsBuffer[((pos) * 7) + 3]
#define normal(pos)   vec3(pointsBuffer[((pos) * 7) + 4], pointsBuffer[((pos) * 7) + 5], pointsBuffer[((pos) * 7) + 6])

NUM_THREADS(1, 1, 1)
void main() {
    vec3 prev = p(gl_WorkGroupID.x - sign(gl_WorkGroupID.x));
    vec3 curr = p(gl_WorkGroupID.x);
    vec3 next = p(gl_WorkGroupID.x + 1);
    vec3 next_next = p(gl_WorkGroupID.x + 1 + sign(maxInstanceSize - 1 - gl_WorkGroupID.x));

    float color0 = color(gl_WorkGroupID.x);
    float color1 = color(gl_WorkGroupID.x + 1);

    vec3 normal0 = normal(gl_WorkGroupID.x);
    vec3 normal1 = normal(gl_WorkGroupID.x + 1);

    imageStore(textureBufferSegments, calculateTextureCoord((gl_WorkGroupID.x * 5),     maxTextureSize), vec4(prev.xyz, normal0.x));
    imageStore(textureBufferSegments, calculateTextureCoord((gl_WorkGroupID.x * 5) + 1, maxTextureSize), vec4(curr.xyz, normal0.y));
    imageStore(textureBufferSegments, calculateTextureCoord((gl_WorkGroupID.x * 5) + 2, maxTextureSize), vec4(next.xyz, normal0.z));
    imageStore(textureBufferSegments, calculateTextureCoord((gl_WorkGroupID.x * 5) + 3, maxTextureSize), vec4(next_next.xyz, color0));
    imageStore(textureBufferSegments, calculateTextureCoord((gl_WorkGroupID.x * 5) + 4, maxTextureSize), vec4(normal1.xyz, color1));

    if(gl_WorkGroupID.x > 0) {
        prev    = p(gl_WorkGroupID.x - 1);
        curr    = p(gl_WorkGroupID.x);
        next    = p(gl_WorkGroupID.x + 1);
        color0  = color(gl_WorkGroupID.x);
        normal0 = normal(gl_WorkGroupID.x);

        imageStore(textureBufferJoints, calculateTextureCoord(((gl_WorkGroupID.x - 1) * 4),     maxTextureSize), vec4(prev.xyz, 0));
        imageStore(textureBufferJoints, calculateTextureCoord(((gl_WorkGroupID.x - 1) * 4) + 1, maxTextureSize), vec4(curr.xyz, color0));
        imageStore(textureBufferJoints, calculateTextureCoord(((gl_WorkGroupID.x - 1) * 4) + 2, maxTextureSize), vec4(next.xyz, 0));
        imageStore(textureBufferJoints, calculateTextureCoord(((gl_WorkGroupID.x - 1) * 4) + 3, maxTextureSize), vec4(normal0.xyz, 0));
    }

    if(gl_WorkGroupID.x == 0) {
        drawIndexedIndirect(segmentsIndirectBuffer, 0, 6, maxInstanceSize, 0, 0, 0);
        drawIndexedIndirect(jointsIndirectBuffer,   0, 6, maxInstanceSize - 1, 0, 0, 0);
    }
}
