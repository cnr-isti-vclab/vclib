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

float trace4(mat4 A) {
    return A[0][0]+A[1][1]+A[2][2]+A[3][3];
}

// Cayley-Hamilton method
mat4 mat4Inverse(mat4 A) {
    mat4 A2 = mul(A, A);
    mat4 A3 = mul(A, A2);
    float traceA = trace4(A);
    float traceA2 = trace4(A2);
    float traceA3 = trace4(A3);
    mat4 I = mtxFromCols(vec4(1,0,0,0),vec4(0,1,0,0),vec4(0,0,1,0),vec4(0,0,0,1));
    mat4 temp = 
        1/6 * (pow(traceA,3) - 3*traceA*traceA2 + 2*traceA3) * I
        -1/2 * A * (pow(traceA, 2)-traceA2)
        + A2*traceA-A3;
    return temp/determinant(A);
}

mat3 mat3Inverse(mat3 A) {
    float a = A[0][0];
    float b = A[0][1];
    float c = A[0][2];
    float d = A[1][0];
    float e = A[1][1];
    float f = A[1][2];
    float g = A[2][0];
    float h = A[2][1];
    float i = A[2][2];
    mat3 temp = mat3(
        e*i-f*h, c*h-b*i, b*f-c*e,
        f*g-d*i, a*i-c*g, c*d-a*f,
        d*h-e*g, b*g-a*h, a*e-b*d
    );
    float det = a*temp[0][0] + b*temp[1][0] + c*temp[2][0];
    return temp/det;
}

mat2 mat2Inverse(mat2 A) {
    return (1/(A[0][0]*A[1][1]-A[0][1]*A[1][0])) * mat2(
        A[1][1], -A[0][1],
        -A[1][0], A[0][0]
    );
}

// returns the equation of a plane given 3 points.
// the returned vec4 is [a, b, c, d] where those variables are the ones in
// ax + by + cz + d = 0
vec4 planeEqFromPoints(vec3 p0, vec3 p1, vec3 p2) {
    vec3 v = p1 - p0;
    vec3 w = p2 - p0;
    vec3 n = normalize(cross(v, w));
    float d = -(mul(n, p0));
    return vec4(n.x, n.y, n.z, d);
}

// returns a matrix to convert from the "input space" to the "triangle space"
// (or rather, a space where we can ignore the z coordinate of the triangle)
mat4 triangleCoordinateSpaceMtx(vec4 plane, vec3 p0, vec3 p1) {
    vec4 zAx = vec4(plane.x, plane.y, plane.z, 0);
    vec4 xAx = vec4(normalize(p1 - p0), 0);
    vec4 yAx = vec4(normalize(cross(zAx.xyz, xAx.xyz)), 0);
    return mat4Inverse(mtxFromCols(xAx, yAx, zAx, vec4(p0, 1)));
}

// requires all the points to be in "triangle space"
vec3 barycentricCoords(vec2 p0, vec2 p1, vec2 p2, vec2 p) {
    mat2 Tinv = mat2Inverse(mat2(
        p0.x - p2.x, p1.x - p2.x,
        p0.y - p2.y, p1.y - p2.y
    ));
    vec2 lam12 = mul(Tinv, p - p2);
    return vec3(lam12, 1 - lam12[0] - lam12[1]);
}

// point and triangle need to be coplanar and in the same space
bool isCoplanarPointInsideTriangle(mat4 triSpace, mat3 trngl, vec3 p) {
    // since the point is coplanar to the triangle we can discard the z-axis
    vec2 pointTriSpace = mul(triSpace, vec4(p, 1.0)).xy;
    vec2 p0 = mul(triSpace, vec4(trngl[0], 1.0)).xy;
    vec2 p1 = mul(triSpace, vec4(trngl[1], 1.0)).xy;
    vec2 p2 = mul(triSpace, vec4(trngl[2], 1.0)).xy;
    vec3 baryCoords = barycentricCoords(p0, p1, p2, pointTriSpace);
    bool isInside = true;
    for(uint i = 0; i < 3; i++) {
        isInside = isInside && baryCoords[i] >= 0 && baryCoords[i] <= 1;
    }
    return isInside;
}

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

    uint slctd = 1;
    for (uint i = 0; i < 3; i++) {
        vec4 pNDC = mul(u_modelViewProj, vec4(poss[i].xyz, 1));
        pNDC = pNDC / (pNDC.w == 0? 1 : pNDC.w);
        if (!(pNDC.x >= minNDC.x && pNDC.x <= maxNDC.x && pNDC.y >= minNDC.y && pNDC.y <= maxNDC.y && pNDC.z >= minNDC.z && pNDC.z <= maxNDC.z)) {
            slctd = 0;
            break;
        }
    }

    uint fBufferIndex = faceIndex/32;
    uint fBitOffset = 31-(faceIndex%32);
    uint fBitMask = 0x1 << fBitOffset;
    uint _useless;
    if (slctd == 1) {
        atomicFetchAndOr(face_selected[fBufferIndex], fBitMask, _useless);
    } else {
        atomicFetchAndAnd(face_selected[fBufferIndex], ~fBitMask, _useless);
    }
}