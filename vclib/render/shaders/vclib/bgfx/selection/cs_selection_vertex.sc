// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "selection_common.sh"

#include <vclib/bgfx/selection/uniforms.sh>

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next lines with:

BUFFER_RO(positions, vec4, VCL_MRB_VERTEX_POSITION_STREAM); // coordinates (3 floats)
*/
BUFFER_RO(positions, vec4, 0); // coordinates (3 floats)

BUFFER_RW(vertex_selected, uint, 4);   // is vertex selected? 1 bit per vertex...

// THE SELECTION IS CHECKED IN NDC SPACE. I decided for this because this way i only need the viewRect and the modelViewProj uniforms.
// Possibility: uniform containing selection box passed already in NDC space? It's probably doable

NUM_THREADS(VCL_COMPUTE_THREAD_COUNT_X, VCL_COMPUTE_THREAD_COUNT_Y, VCL_COMPUTE_THREAD_COUNT_Z)
// 1 'thread' per point
void main()
{
    uint pointId = linearIndex(gl_GlobalInvocationID);
    if(pointId >= u_primitiveCount) {
        return;
    }
    uint idx30 = pointId * 3;
    uint idx31 = idx30+1;
    uint idx32 = idx30+2;

    float minX = u_selectionBox[0];
    float minY = u_selectionBox[1];
    float maxX = u_selectionBox[2];
    float maxY = u_selectionBox[3];

    vec3 maxNDC = vec3(
        (maxX - u_viewRect.x) / u_viewRect.z * 2 - 1,
        1 - ((minY - u_viewRect.y) / u_viewRect.w * 2),
        1
    );
    vec3 minNDC = vec3(
        (minX - u_viewRect.x) / u_viewRect.z * 2 - 1,
        1 - ((maxY - u_viewRect.y) / u_viewRect.w * 2),
        0
    );

    vec4 p = vec4(
        positions[idx30/4][idx30%4],
        positions[idx31/4][idx31%4],
        positions[idx32/4][idx32%4],
        1
    );

    vec4 pNDC = mul(u_modelViewProj, p);
    pNDC = pNDC / pNDC.w;

    if (pointInAABB(pNDC.xyz, minNDC, maxNDC)) {
        if (u_selectionAction > 0.0) { // subtract
            atomicClearBoolInBuffer(vertex_selected, pointId);
        } else {
            atomicSetBoolInBuffer(vertex_selected, pointId);
        }
    }
}
