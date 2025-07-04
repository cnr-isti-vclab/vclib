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

#include <vclib/bgfx/drawable/lines.sh>

BUFFER_RO(coordsBuffer,      vec4,   0);
BUFFER_RO(colorBuffer,       uvec4,  1);
BUFFER_RO(normalsBuffer,     vec4,   2);

BUFFER_WO(vertexBuffer,      vec4,   3);
BUFFER_WO(indexBuffer,       uvec4,  4);

#define get_float_from_vec4(pos, myBuffer) myBuffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)        vec3(get_float_from_vec4(((pos) * 3) + 0, coordsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 1, coordsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 2, coordsBuffer))

#define color(pos)    get_float_from_vec4(pos, colorBuffer)

#define normal(pos)   vec3(get_float_from_vec4(((pos) * 3) + 0, normalsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 1, normalsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 2, normalsBuffer))     

NUM_THREADS(2, 2, 1)
void main() {
    uint baseIndex = (gl_WorkGroupID.x * 12) + ((gl_LocalInvocationID.y + (gl_LocalInvocationID.x * 2)) * 3);

    vec3 p0        = p((gl_WorkGroupID.x * 2));
    vec3 p1        = p((gl_WorkGroupID.x * 2) + 1);
    uint color    = color((gl_WorkGroupID.x * 2) + (1 - ((uint)(gl_LocalInvocationID.x + 1) % 2)));
    vec3 normal    = normal((gl_WorkGroupID.x * 2) + (1 - ((uint)(gl_LocalInvocationID.x + 1) % 2)));

    vertexBuffer[baseIndex]     = vec4(p0.x, p0.y, p0.z, p1.x);
    vertexBuffer[baseIndex + 1] = vec4(p1.y, p1.z, uintBitsToFloat(color), normal.x);
    vertexBuffer[baseIndex + 2] = vec4(normal.y, normal.z, gl_LocalInvocationID.x, gl_LocalInvocationID.y);

    if(gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0) {
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 0, indexBuffer) = (gl_WorkGroupID.x * 4);
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 1, indexBuffer) = (gl_WorkGroupID.x * 4) + 3;
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 2, indexBuffer) = (gl_WorkGroupID.x * 4) + 1;

        get_float_from_vec4((6 * gl_WorkGroupID.x) + 3, indexBuffer) = (gl_WorkGroupID.x * 4);
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 4, indexBuffer) = (gl_WorkGroupID.x * 4) + 2;
        get_float_from_vec4((6 * gl_WorkGroupID.x) + 5, indexBuffer) = (gl_WorkGroupID.x * 4) + 3;
    }

}
