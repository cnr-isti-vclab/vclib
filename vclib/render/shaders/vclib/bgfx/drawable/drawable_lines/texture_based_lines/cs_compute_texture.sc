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

BUFFER_RO(coordsBuffer,      vec4,  0);
BUFFER_RO(colorBuffer,       uvec4, 1);
BUFFER_RO(normalsBuffer,     vec4,  2);

IMAGE2D_WO(textureBuffer,    rgba32f,  3);

uniform vec4 u_IndirectData;
#define maxTextureSize          u_IndirectData.x
#define instancingNum           u_IndirectData.y

#define get_float_from_vec4(pos, myBuffer) myBuffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)        vec3(get_float_from_vec4(((pos) * 3) + 0, coordsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 1, coordsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 2, coordsBuffer))

#define color(pos)    get_float_from_vec4(pos, colorBuffer)

#define normal(pos)   vec3(get_float_from_vec4(((pos) * 3) + 0, normalsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 1, normalsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 2, normalsBuffer))       

NUM_THREADS(1, 1, 1)
void main() {
    vec3 p0          = p((gl_WorkGroupID.x * 2));
    vec3 p1          = p((gl_WorkGroupID.x * 2) + 1);
    uint color0      = color((gl_WorkGroupID.x * 2));
    uint color1      = color((gl_WorkGroupID.x * 2) + 1);
    vec3 normal0     = normal((gl_WorkGroupID.x * 2));
    vec3 normal1     = normal((gl_WorkGroupID.x * 2) + 1);

    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) , maxTextureSize),    vec4(p0.xyz, uintBitsToFloat(color0)));
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 1, maxTextureSize), vec4(p1.xyz, uintBitsToFloat(color1)));
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 2, maxTextureSize), vec4(normal0.xyz, 0));
    imageStore(textureBuffer, calculateTextureCoord((gl_WorkGroupID.x * 4) + 3, maxTextureSize), vec4(normal1.xyz, 0));
} 
