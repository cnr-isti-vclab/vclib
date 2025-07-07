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

BUFFER_RO(coordsBuffer,         vec4,   0);
BUFFER_RO(colorBuffer,          uvec4,  1);
BUFFER_RO(normalsBuffer,        vec4,   2);

BUFFER_WO(instanceDataSegments, vec4,  3);

uniform vec4 u_IndirectData;
#define maxInstanceSize         u_IndirectData.x

#define get_float_from_vec4(pos, myBuffer) myBuffer[uint(pos) / 4][uint(pos) % 4]

#define p(pos)        vec3(get_float_from_vec4(((pos) * 3) + 0, coordsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 1, coordsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 2, coordsBuffer))

#define color(pos)    get_float_from_vec4(pos, colorBuffer)

#define normal(pos)   vec3(get_float_from_vec4(((pos) * 3) + 0, normalsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 1, normalsBuffer), \
                           get_float_from_vec4(((pos) * 3) + 2, normalsBuffer))    

NUM_THREADS(1, 1, 1)
void main()
{
    vec3 prev = p(gl_WorkGroupID.x - sign(gl_WorkGroupID.x));
    vec3 curr = p(gl_WorkGroupID.x);
    vec3 next = p(gl_WorkGroupID.x + 1);
    vec3 next_next = p(gl_WorkGroupID.x + 1 + sign(maxInstanceSize - 1 - gl_WorkGroupID.x));

    uint color0 = color(gl_WorkGroupID.x);
    uint color1 = color(gl_WorkGroupID.x + 1);

    vec3 normal0 = normal(gl_WorkGroupID.x);
    vec3 normal1 = normal(gl_WorkGroupID.x + 1);

    instanceDataSegments[(gl_WorkGroupID.x * 5)]        = vec4(prev.xyz, normal0.x);
    instanceDataSegments[(gl_WorkGroupID.x * 5) + 1]    = vec4(curr.xyz, uintBitsToFloat(color0));
    instanceDataSegments[(gl_WorkGroupID.x * 5) + 2]    = vec4(next.xyz, uintBitsToFloat(color1));
    instanceDataSegments[(gl_WorkGroupID.x * 5) + 3]    = vec4(next_next.xyz, normal0.y);
    instanceDataSegments[(gl_WorkGroupID.x * 5) + 4]    = vec4(normal0.z, normal1.xyz);
}
