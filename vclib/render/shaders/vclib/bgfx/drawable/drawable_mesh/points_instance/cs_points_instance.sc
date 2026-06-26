// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next lines with:

BUFFER_RO(positions, vec4, VCL_MRB_VERTEX_POSITION_STREAM); // coordinates (3 floats)
BUFFER_RO(normals,   vec4, VCL_MRB_VERTEX_NORMAL_STREAM);   // normals (3 floats)
*/
BUFFER_RO(positions, vec4, 0); // coordinates (3 floats)
BUFFER_RO(normals,   vec4, 1);   // normals (3 floats)

DECLARE_FETCH_VEC3(fetchPosition, positions);
DECLARE_FETCH_VEC3(fetchNormal, normals);

BUFFER_WO(vOut, vec4, 4); // output vertices
// 2 vec4 per vertex:
// - 3 floats for position + 1 float (padding)
// - 3 floats for normal   + 1 float for scale


NUM_THREADS(1, 1, 1) // 1 'thread' per point
void main()
{
    uint pointId = gl_WorkGroupID.x;

    vec3 p = fetchPosition(pointId);
    vec3 n = fetchNormal(pointId);

    // Generate quad vertices
    UNROLL
    for (int i = 0; i < 4; ++i) {
        // Offset for quad vertices
        uint vertexId = pointId * 4 + i;
        // pos 3, padding 1
        // norm 3, sca 1
        vOut[vertexId * 2]     = vec4(p, 0.0);
        vOut[vertexId * 2 + 1] = vec4(n, 1.0);
    }
}
