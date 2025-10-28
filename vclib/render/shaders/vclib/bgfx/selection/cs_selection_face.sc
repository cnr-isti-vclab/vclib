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
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(positions, vec4, VCL_MRB_VERTEX_POSITION_STREAM); // coordinates (3 floats)
BUFFER_RO(indices, uint, 5);
BUFFER_RW(face_selected, uint, 6); // is face selected? 1 bit per face...

uniform vec4 u_selectionBox; // screen space
uniform vec4 u_workgroupSizeAndVertexCount; // despite the name, w should contain the face count (i.e. numTris)

// THE SELECTION IS CHECKED IN NDC SPACE. I decided for this because this way i only need the viewRect and the modelViewProj uniforms.
// Possibility: uniform containing selection box passed already in NDC space? It's probably doable

// Slab method with extra restrictions on the possible values of tclose and tfar
// (Since we calculate the ray such that it identifies the segment in the range 0<=t<=1)
bool segmentIntersectsAABB(vec3 minBoxPoint, vec3 maxBoxPoint, vec3 p0, vec3 p1) {
    vec3 dir = p1 - p0;
    bvec3 considerAxis = bvec3(dir.x != 0, dir.y != 0, dir.z != 0);
    bool didInit = false;
    float tclose = 0;
    float tfar = 0;
    for (uint i = 0; i < 3; i++) {
        if (!considerAxis[i]) {
            continue;
        }
        float tlow = (minBoxPoint[i] - p0[i]) / dir[i];
        float thigh = (maxBoxPoint[i] - p0[i]) / dir[i];
        float tmax = max(tlow, thigh);
        float tmin = min(tlow, thigh);
        if (!didInit) {
            didInit = true;
            tclose = tmin;
            tfar = tmax;
            continue;
        }
        tclose = max(tclose, tmin);
        tfar = min(tfar, tmax);
    }
    if (!didInit || tclose > tfar) {
        return false;
    }
    if (
        (tclose <= 0 && tfar >= 1) 
        || (tclose >= 0 && tclose <= 1)
        || (tfar >= 0 && tfar <= 1)
    ) {
        return true;
    }
    return false;
}

bool triangleSegmentsIntersectAABB(vec3 minBoxPoint, vec3 maxBoxPoint, mat3 trngl) {
    for (uint i = 0; i < 3; i++) {
        if (segmentIntersectsAABB(minBoxPoint, maxBoxPoint, trngl[i], trngl[(i+1)%3])) {
            return true;
        }
    }
    return false;
}

mat4 planeFromTriangle(mat3 tri) {
    vec3 n = cross(tri[1] - tri[0], tri[2] - tri[0]);
    float d = -mul(n, tri[0]);
    return mat4(vec4(tri[0], 1), vec4(n.x, n.y, n.z, d), vec4_splat(0), vec4_splat(0));
}

vec3 projectOntoPlane(vec3 p, vec4 plane) {
    vec3 n = plane.xyz;
    float d = plane.w;
    float dist = mul(n, p) + d;
    return p - (n * dist);
}

vec3 barycentricCoords(mat3 tri, vec2 p) {
    vec3 newP = vec3(1, p.x, p.y);
    float mult = 1.0/(tri[0].x*(tri[1].y-tri[2].y) + tri[1].x*(tri[2].y-tri[0].y) + tri[2].x*(tri[0].y-tri[1].y));
    mat3 mt = mat3(
        tri[1].x*tri[2].y-tri[2].x*tri[1].y, tri[1].y-tri[2].y, tri[2].x-tri[1].x,
        tri[2].x*tri[0].y-tri[0].x*tri[2].y, tri[2].y-tri[0].y, tri[0].x-tri[2].x,
        tri[0].x*tri[1].y-tri[1].x*tri[0].y, tri[0].y-tri[1].y, tri[1].x-tri[0].x
    );
    return mult*mul(mt, newP);
}

mat3 mat3Inv(mat3 A) {
    vec3 A1crossA2 = cross(A[1], A[2]);
    float det = mul(A[0], A1crossA2);
    return (1/(det == 0 ? 1 : det)) * mtxFromRows(A1crossA2, cross(A[2], A[0]), cross(A[0], A[1]));
}

// FUNCTION TO CONVERT COORDINATES TO 2D
vec2 cooplanarCoordsTo2D(vec3 p, mat3 uvo) {
    return mul(mat3Inv(uvo), p).xy;
}

// FUNCTION THAT COMPUTES SEGMENT-PLANE INTERSECTION (OR RATHER, RAY-PLANE INTERSECTION WITH THE CONSTRAINT 0<=t<=1)
//     returns a vec4 in which the W coordinate is either 1 if the intersection was calculated or 0 if there is no intersection
vec4 segmentPlaneIntersection(mat4 plane, vec3 p0, vec3 p1) {
    vec3 o = p0;
    vec3 d = p1-p0;
    vec3 n = plane[1].xyz;
    float den = mul(n, d);
    if (abs(den) <= 0.00001f) {
        return vec4(0, 0, 0, 0);
    }
    float t = mul((plane[0].xyz - o), n) / den;
    if (t < 0 || t > 1) {
        return vec4(0, 0, 0, 0);
    }
    vec3 intersXYZ = o + d*t;
    return vec4(intersXYZ.x, intersXYZ.y, intersXYZ.z, 1);
}

// FUNCTION THAT CHECKS IF AT LEAST ONE OF THE INTERSECTIONS BETWEEN AN EDGE OF THE BOX AND THE TRIANGLE'S PLANE IS INSIDE THE TRIANGLE:
//     calculate triangle's plane;
//     calculate u (subtraction of two of the triangle's points), v (cross(u, plane_normal)), and o (any of the triangle's points) for 3d to 2d coordinate conversion;
//     for each of the box's edges:
//         calculate intersection between edge and plane;
//         if no intersection proceed to next edge;
//         convert intersection point coordinates to 2d;
//         calculate barycentric coordinates;
//         if all barycentric coordinates in [0, 1] (point inside triangle) return true;
//     return false;
bool AABBEdgesIntersectTriangle(mat3 tri, vec3 minBoxPoint, vec3 maxBoxPoint) {
    mat4 plane = planeFromTriangle(tri);
    vec3 u = tri[1] - tri[0];
    vec3 v = cross(u, plane[1].xyz);
    vec3 o = plane[0].xyz;
    mat3 uvo = mtxFromCols(u,v,o);
    mat3 tri2D = mat3(vec3(cooplanarCoordsTo2D(tri[0], uvo), 0), vec3(cooplanarCoordsTo2D(tri[1], uvo), 0), vec3(cooplanarCoordsTo2D(tri[2], uvo), 0));
    for (uint i = 0; i < 3; i++) {
        uint varCoordIdx1 = (i+1)%3;
        uint varCoordIdx2 = (i+2)%3;
        vec2 coord1 = vec2(minBoxPoint[varCoordIdx1], maxBoxPoint[varCoordIdx1]);
        vec2 coord2 = vec2(minBoxPoint[varCoordIdx2], maxBoxPoint[varCoordIdx2]);
        for (uint j = 0; j < 2; j++) {
            for (uint k = 0; k < 2; k++) {
                vec3 p0 = vec3(0,0,0);
                vec3 p1 = vec3(0,0,0);
                switch (i) {
                    case 0:
                        p0.x = minBoxPoint[i];
                        p1.x = maxBoxPoint[i];
                        break;
                    case 1:
                        p0.y = minBoxPoint[i];
                        p1.y = maxBoxPoint[i];
                        break;
                    case 2:
                        p0.z = minBoxPoint[i];
                        p1.z = maxBoxPoint[i];
                        break;
                }
                switch (varCoordIdx1) {
                    case 0:
                        p0.x = coord1[j];
                        p1.x = coord1[j];
                        break;
                    case 1:
                        p0.y = coord1[j];
                        p1.y = coord1[j];
                        break;
                    case 2:
                        p0.z = coord1[j];
                        p1.z = coord1[j];
                        break;
                }
                switch (varCoordIdx2) {
                    case 0:
                        p0.x = coord2[k];
                        p1.x = coord2[k];
                        break;
                    case 1:
                        p0.y = coord2[k];
                        p1.y = coord2[k];
                        break;
                    case 2:
                        p0.z = coord2[k];
                        p1.z = coord2[k];
                        break;
                }
                vec4 inters = segmentPlaneIntersection(plane, p0, p1);
                if (inters[3] == 0) {
                    continue;
                }
                vec2 inters2D = cooplanarCoordsTo2D(inters.xyz, uvo);
                vec3 baryCoords = barycentricCoords(tri2D, inters2D);
                if (
                    baryCoords[0] >= 0 && baryCoords[0] <= 1
                    && baryCoords[1] >= 0 && baryCoords[1] <= 1
                    && baryCoords[2] >= 0 && baryCoords[2] <= 1
                ) {
                    return true;
                }
            }
        }
    }
    return false;
}

// A triangle intersects the selection box if either:
//     1. At least one of its edges intersects the selection box (easier to calculate, the selection box is an AABB in NDC) or
//     2. The intersection of at least one of the selection box's edges with the triangle's plane lies inside the triangle
NUM_THREADS(1, 1, 1) // 1 'thread' per face, or 1 'thread' per 3 indices
void main()
{
    uint faceCount = floatBitsToUint(u_workgroupSizeAndVertexCount.w);
    uvec3 workGroupSize = uvec3(floatBitsToUint(u_workgroupSizeAndVertexCount.x), floatBitsToUint(u_workgroupSizeAndVertexCount.y), floatBitsToUint(u_workgroupSizeAndVertexCount.z));
    uint faceIndex = (gl_WorkGroupID.x + workGroupSize.x * gl_WorkGroupID.y + workGroupSize.x * workGroupSize.y * gl_WorkGroupID.z);
    uint indicesBaseIndex = 3 * faceIndex;
    uvec3 idcs = uvec3(indices[indicesBaseIndex], indices[indicesBaseIndex + 1], indices[indicesBaseIndex + 2]);

    if(faceIndex >= faceCount) {
        return;
    }

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

    vec4 p0NDC = mul(u_modelViewProj, vec4(poss[0].xyz, 1));
    p0NDC = p0NDC / (p0NDC.w == 0? 1 : p0NDC.w);

    vec4 p1NDC = mul(u_modelViewProj, vec4(poss[1].xyz, 1));
    p1NDC = p1NDC / (p1NDC.w == 0? 1 : p1NDC.w);

    vec4 p2NDC = mul(u_modelViewProj, vec4(poss[2].xyz, 1));
    p2NDC = p2NDC / (p2NDC.w == 0? 1 : p2NDC.w);

    mat3 tri = mtxFromRows(p0NDC.xyz, p1NDC.xyz, p2NDC.xyz);
    bool selected = triangleSegmentsIntersectAABB(minNDC, maxNDC, tri);
    if (!selected) {
        selected = AABBEdgesIntersectTriangle(tri, minNDC, maxNDC);
    }

    uint fBufferIndex = faceIndex/32;
    uint fBitOffset = 31-(faceIndex%32);
    uint fBitMask = 0x1 << fBitOffset;
    uint _useless;
    if (selected) {
        atomicFetchAndOr(face_selected[fBufferIndex], fBitMask, _useless);
    } else {
        atomicFetchAndAnd(face_selected[fBufferIndex], ~fBitMask, _useless);
    }
}