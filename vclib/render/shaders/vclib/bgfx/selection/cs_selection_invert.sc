/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/bgfx/selection/uniforms.sh>

BUFFER_RW(primitive_selected, uint, 4);   // is vertex selected? 1 bit per vertex...

NUM_THREADS(VCL_COMPUTE_THREAD_COUNT_X, VCL_COMPUTE_THREAD_COUNT_Y, VCL_COMPUTE_THREAD_COUNT_Z)
// 1 'thread' per uint in the buffer,
// each thread will invert 32 primitives selection state
void main()
{
    uint bufferIndex = linearIndex(gl_GlobalInvocationID);
    // u_primitiveCount here is the number of uints in the buffer
    if(bufferIndex >= u_primitiveCount) { 
        return;
    }
    primitive_selected[bufferIndex] = ~primitive_selected[bufferIndex];
}
