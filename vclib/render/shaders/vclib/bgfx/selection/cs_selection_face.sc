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

#include "face_selection_common.sh"

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/selection/uniforms.sh>

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next lines with:

BUFFER_RO(positions, vec4, VCL_MRB_VERTEX_POSITION_STREAM); // coordinates (3 floats)
*/
BUFFER_RO(positions, vec4, 0); // coordinates (3 floats)

BUFFER_RO(indices, uint, 5);
BUFFER_RW(face_selected, uint, 6); // is face selected? 1 bit per triangle (MSb first)

BUFFER_RO(tri_to_poly, uint, 7);       // tri_to_poly[triIdx] = polyIdx
BUFFER_RO(poly_to_tri_begin, uint, 8); // poly_to_tri_begin[polyIdx] = first triangle index
BUFFER_RO(poly_to_tri_count, uint, 9); // poly_to_tri_count[polyIdx] = number of triangles

// Polygon-level ADD selection:
// If any triangle of a polygon intersects the selection box, all triangles of
// that polygon are marked as selected.
NUM_THREADS(1, 1, 1) // 1 thread per triangle
void main()
{
    uint faceIndex = getPrimitiveID(gl_WorkGroupID);
    if(faceIndex >= u_primitiveCount) {
        return;
    }

    uint indicesBaseIndex = 3 * faceIndex;
    uvec3 idcs = uvec3(indices[indicesBaseIndex], indices[indicesBaseIndex + 1], indices[indicesBaseIndex + 2]);

    float minX = u_selectionBox[0];
    float minY = u_selectionBox[1];
    float maxX = u_selectionBox[2];
    float maxY = u_selectionBox[3];

    vec3 maxNDC = vec3(
        (maxX - u_viewRect.x) / max(1, u_viewRect.z) * 2 - 1,
        1 - ((minY - u_viewRect.y) / max(1, u_viewRect.w)) * 2,
        1
    );
    vec3 minNDC = vec3(
        (minX - u_viewRect.x) / max(1, u_viewRect.z) * 2 - 1,
        1 - ((maxY - u_viewRect.y) / max(1, u_viewRect.w) * 2),
        0
    );

    mat3 poss = mat3(
        positions[(idcs[0] * 3) / 4][(idcs[0] * 3) % 4], positions[(idcs[0] * 3 + 1) / 4][(idcs[0] * 3 + 1) % 4], positions[(idcs[0] * 3 + 2) / 4][(idcs[0] * 3 + 2) % 4],
        positions[(idcs[1] * 3) / 4][(idcs[1] * 3) % 4], positions[(idcs[1] * 3 + 1) / 4][(idcs[1] * 3 + 1) % 4], positions[(idcs[1] * 3 + 2) / 4][(idcs[1] * 3 + 2) % 4],
        positions[(idcs[2] * 3) / 4][(idcs[2] * 3) % 4], positions[(idcs[2] * 3 + 1) / 4][(idcs[2] * 3 + 1) % 4], positions[(idcs[2] * 3 + 2) / 4][(idcs[2] * 3 + 2) % 4]
    );

    vec4 p0Clip = mul(u_modelViewProj, vec4(poss[0].xyz, 1));
    vec4 p1Clip = mul(u_modelViewProj, vec4(poss[1].xyz, 1));
    vec4 p2Clip = mul(u_modelViewProj, vec4(poss[2].xyz, 1));

    // Conservative reject for fully out-of-frustum triangles before NDC divide
    if (!pointInClipSpace(p0Clip) && !pointInClipSpace(p1Clip) && !pointInClipSpace(p2Clip)) {
        if (
            (p0Clip.x < -p0Clip.w && p1Clip.x < -p1Clip.w && p2Clip.x < -p2Clip.w)
            || (p0Clip.x > p0Clip.w && p1Clip.x > p1Clip.w && p2Clip.x > p2Clip.w)
            || (p0Clip.y < -p0Clip.w && p1Clip.y < -p1Clip.w && p2Clip.y < -p2Clip.w)
            || (p0Clip.y > p0Clip.w && p1Clip.y > p1Clip.w && p2Clip.y > p2Clip.w)
            || (p0Clip.z < 0 && p1Clip.z < 0 && p2Clip.z < 0)
            || (p0Clip.z > p0Clip.w && p1Clip.z > p1Clip.w && p2Clip.z > p2Clip.w)
        ) {
            return;
        }
    }

    vec4 p0NDC = p0Clip;
    vec4 p1NDC = p1Clip;
    vec4 p2NDC = p2Clip;

    p2NDC = p2NDC / (p2NDC.w == 0? 1 : p2NDC.w);
    p0NDC = p0NDC / (p0NDC.w == 0? 1 : p0NDC.w);
    p1NDC = p1NDC / (p1NDC.w == 0? 1 : p1NDC.w);

    bool selected = false;

    mat3 tri = mtxFromRows(p0NDC.xyz, p1NDC.xyz, p2NDC.xyz);
    selected = triangleSegmentsIntersectAABB(minNDC, maxNDC, tri);
    if (!selected) {
        selected = AABBEdgesIntersectTriangle(tri, minNDC, maxNDC);
    }

    if (selected) {
        // Mark all triangles of the same polygon as selected
        uint polyIdx = tri_to_poly[faceIndex];
        uint firstTri = poly_to_tri_begin[polyIdx];
        uint count = poly_to_tri_count[polyIdx];
        uint _useless;
        for (uint t = firstTri; t < firstTri + count; t++) {
            uint tBufIndex = t / 32;
            uint tBitMask = 0x1 << (31 - (t % 32));
            atomicFetchAndOr(face_selected[tBufIndex], tBitMask, _useless);
        }
    }
}
