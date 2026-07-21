// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/selection/uniforms.sh>

// INFO: the two textures MUST be of the same size
IMAGE2D_RO(s_primIds, rgba8, 0);
IMAGE2D_RO(s_meshIds, rgba8, 1);
BUFFER_RW(face_selected, uint, 12);

BUFFER_RO(tri_to_poly, uint, 7);       // tri_to_poly[triIdx] = polyIdx
BUFFER_RO(poly_to_tri_begin, uint, 8); // poly_to_tri_begin[polyIdx] = first triangle index
BUFFER_RO(poly_to_tri_count, uint, 9); // poly_to_tri_count[polyIdx] = number of triangles

uint texVec4ToUint(vec4 pixel) {
    return
        ((uint(pixel.r * 255.0) & 0xff) << uint(24))
        | ((uint(pixel.g * 255.0) & 0xff) << uint(16))  
        | ((uint(pixel.b * 255.0) & 0xff) << uint(8)) 
        | (uint(pixel.a * 255.0) & 0xff);
}

// Polygon-level selection: if a visible pixel belongs to triangle T, select or
// deselect all triangles of T's polygon based on u_selectionAction (0 = ADD,
// 1 = SUBTRACT).
NUM_THREADS(VCL_COMPUTE_THREAD_COUNT_X, VCL_COMPUTE_THREAD_COUNT_Y, VCL_COMPUTE_THREAD_COUNT_Z)
void main() {
    // NOTE: u_meshID == UINT_MAX is reserved to indicate that no data is available
    if(u_meshID == uint(0xFFFFFFFF)) {
        return;
    }
    uint tex1DCoord = linearIndex(gl_GlobalInvocationID);
    uvec2 imSz = imageSize(s_primIds).xy;
    ivec2 tex2DCoord = ivec2(int(tex1DCoord % imSz.x), int(tex1DCoord / imSz.x));

    vec4 vecMeshId = imageLoad(s_meshIds, tex2DCoord);
    uint texMeshId = texVec4ToUint(vecMeshId);
    if (texMeshId != u_meshID) {
        return;
    }

    vec4 vecPrimId = imageLoad(s_primIds, tex2DCoord);
    uint texPrimId = texVec4ToUint(vecPrimId);

    // Select or deselect all triangles belonging to the same polygon as texPrimId
    uint polyIdx = tri_to_poly[texPrimId];
    uint firstTri = poly_to_tri_begin[polyIdx];
    uint count = poly_to_tri_count[polyIdx];
    for (uint t = firstTri; t < firstTri + count; t++) {
        if (u_selectionAction > 0.0) { // subtract
            atomicClearBoolInBuffer(face_selected, t);
        } else {
            atomicSetBoolInBuffer(face_selected, t);
        }
    }
}
