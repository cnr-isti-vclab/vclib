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

NUM_THREADS(1, 1, 1) // 1 'thread' per uint in the buffer,
// each thread will set 32 vertices as not selected
void main()
{
    uint bufferIndex = getPrimitiveID(gl_WorkGroupID);
    // u_primitiveCount here is the number of uints in the buffer
    if(bufferIndex >= u_primitiveCount) { 
        return;
    }
    primitive_selected[bufferIndex] = uint(0);
}
