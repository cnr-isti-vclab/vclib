// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/selection/uniforms.sh>

BUFFER_RW(primitive_selected, uint, 4);   // is vertex selected? 1 bit per vertex...

NUM_THREADS(VCL_COMPUTE_THREAD_COUNT_X, VCL_COMPUTE_THREAD_COUNT_Y, VCL_COMPUTE_THREAD_COUNT_Z)
// 1 'thread' per uint in the buffer,
// each thread will set 32 primitives as selected
void main()
{
    uint bufferIndex = linearIndex(gl_GlobalInvocationID);
    // u_primitiveCount here is number of uints in the buffer
    if(bufferIndex >= u_primitiveCount) { 
        return;
    }
    primitive_selected[bufferIndex] = 0xffffffff;
}
