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

#ifndef VCL_ALGORITHMS_MESH_STAT_GEOMETRY_H
#define VCL_ALGORITHMS_MESH_STAT_GEOMETRY_H

#include "barycenter.h"

#include <vclib/algorithms/mesh/face_topology.h>
#include <vclib/space/complex/mesh_inertia.h>
#include <vclib/space/core/matrix.h>

namespace vcl {

/**
 * @brief Computes the volume of a closed surface Mesh. Returned value is
 * meaningful only if the input mesh is watertight.
 *
 * @param[in] m: closed mesh on which compute the volume.
 * @return The volume of the given mesh.
 */
template<FaceMeshConcept MeshType>
double volume(const MeshType& m)
{
    MeshInertia<MeshType> i(m);
    return i.volume();
}

/**
 * @brief Computes the surface area of the given Mesh, that is the sum of the
 * areas of each face of the mesh.
 *
 * @param[in] m: mesh on which compute the surface area.
 * @return The surface area of the given mesh.
 */
template<FaceMeshConcept MeshType>
double surfaceArea(const MeshType& m)
{
    using FaceType = MeshType::FaceType;

    double area = 0;
    for (const FaceType& f : m.faces()) {
        area += faceArea(f);
    }
    return area;
}

/**
 * @brief Computes the border length of the given Mesh, that is the sum of the
 * length of the face edges that are on border in the given mesh.
 *
 * The function checks whether a face edge is on border by checking if the
 * adjacent face is nullptr, therefore the mesh must have the adjacent faces
 * computed.
 *
 * @param[in] m: mesh on which compute the border length.
 * @return The border length of the given mesh.
 */
template<FaceMeshConcept MeshType>
double borderLength(const MeshType& m)
{
    using FaceType = MeshType::FaceType;

    requirePerFaceAdjacentFaces(m);

    double l = 0;
    for (const FaceType& f : m.faces()) {
        for (uint i = 0; i < f.vertexNumber(); ++i) {
            if (f.adjFace(i) == nullptr) {
                l += f.vertex(i)->coord().dist(f.vertexMod(i + 1)->coord());
            }
        }
    }
    return l;
}

/**
 * @brief Compute the covariance matrix of a Point Cloud Mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param m
 * @return The 3x3 covariance matrix of the given point cloud.
 */
template<MeshConcept MeshType>
auto covarianceMatrixOfPointCloud(const MeshType& m)
{
    using VertexType = MeshType::VertexType;
    using CoordType  = VertexType::CoordType;
    using ScalarType = CoordType::ScalarType;

    CoordType bar = barycenter(m);

    Matrix33<ScalarType> mm;
    mm.setZero();
    // compute covariance matrix
    for (const VertexType& v : m.vertices()) {
        CoordType e = v.coord() - bar;
        mm += e.outerProduct(e);
    }
    return mm;
}

/**
 * @brief Compute covariance matrix of a mesh, i.e. the integral
 * int_{m} { (x-b)(x-b)^T }dx
 * where b is the barycenter and x spans over the mesh m
 *
 * @param m
 * @return The 3x3 covariance matrix of the given mesh.
 */
template<FaceMeshConcept MeshType>
auto covarianceMatrixOfMesh(const MeshType& m)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;
    using CoordType  = VertexType::CoordType;
    using ScalarType = CoordType::ScalarType;

    CoordType            bar = shellBarycenter(m);
    Matrix33<ScalarType> C;
    C.setZero();
    Matrix33<ScalarType> C0;
    C0.setZero();
    C0(0, 0) = C0(1, 1) = 2.0;
    C0(0, 1) = C0(1, 0) = 1.0;
    C0 *= 1 / 24.0;
    // integral of (x,y,0) in the same triangle
    Eigen::Vector3<ScalarType> x;
    x << 1 / 6.0, 1 / 6.0, 0;
    Matrix33<ScalarType> A; // matrix that bring the vertices to (v1-v0,v2-v0,n)
    Matrix33<ScalarType> DC;

    for (const FaceType& f : m.faces()) {
        const CoordType& p0 = f.vertex(0)->coord();
        const CoordType& p1 = f.vertex(1)->coord();
        const CoordType& p2 = f.vertex(2)->coord();
        CoordType        n  = (p1 - p0).cross(p2 - p0);
        double           da = n.norm();
        n /= da * da;

        CoordType tmpp = p1 - p0;
        for (uint j = 0; j < 3; j++)
            A(j, 0) = tmpp(j);
        tmpp = p2 - p0;
        for (uint j = 0; j < 3; j++)
            A(j, 1) = tmpp(j);
        for (uint j = 0; j < 3; j++)
            A(j, 2) = n(j);
        Eigen::Vector3<ScalarType> delta;
        tmpp = p0 - bar;
        for (uint j = 0; j < 3; j++)
            delta(j) = tmpp(j);

        /* DC is calculated as integral of (A*x+delta) * (A*x+delta)^T over the
         * triangle, where delta = v0-bary */
        DC.setZero();
        DC += A * C0 * A.transpose();
        Matrix33<ScalarType> tmp = (A * x) * delta.transpose();
        DC += tmp + tmp.transpose();
        DC += tmp;
        tmp = delta * delta.transpose();
        DC += tmp * 0.5;
        DC *= da; // the determinant of A is also the double area of f
        C += DC;
    }
    return C;
}

/**
 * @brief When performing an adptive pruning for each sample we expect a varying
 * radius to be removed. The radius is a PerVertex attribute that we compute
 * from the current per vertex weights given as argument. The expected radius of
 * the sample is computed so that it linearly maps the quality between
 * diskradius and diskradius*variance
 *
 * @param m
 * @param weights
 * @param diskRadius
 * @param radiusVariance
 * @param invert
 * @return
 */
template<MeshConcept MeshType, typename ScalarType>
std::vector<ScalarType> vertexRadiusFromWeights(
    const MeshType& m,
    Range auto&&    weights,
    double          diskRadius,
    double          radiusVariance,
    bool            invert = false)
{
    using VertexType = MeshType::VertexType;

    assert(std::ranges::size(weights) == m.vertexNumber());

    std::vector<ScalarType> radius(m.vertexContainerSize());
    const auto [min, max] = std::ranges::minmax_element(weights);

    double minRad   = diskRadius;
    double maxRad   = diskRadius * radiusVariance;
    double deltaQ   = max - min;
    double deltaRad = maxRad - minRad;
    for (const auto& [v, w] : std::views::zip(m.vertices(), weights)) {
        radius[m.index(v)] =
            minRad + deltaRad * ((invert ? max - w : w - min) / deltaQ);
    }

    return radius;
}

/**
 * @brief Returns a vector of pairs <face index; edge index> of the edges that
 * have a dihedral angle outside the given range.
 *
 * If alsoBorderEdges is true, border edges are also considered.
 *
 * The algorithm requires the adjacency information of the faces.
 *
 * @param[in] m: Input mesh.
 * @param[in] angleRadNeg: Minimum angle in radians.
 * @param[in] angleRadPos: Maximum angle in radians.
 * @param[in] alsoBorderEdges: If true, also border edges are considered.
 * @return A vector of pairs <face index; edge index> of the edges that have a
 * dihedral angle outside the given range.
 */
template<FaceMeshConcept MeshType>
std::vector<std::pair<uint, uint>> creaseFaceEdges(
    const MeshType& m,
    double          angleRadNeg,
    double          angleRadPos,
    bool            alsoBorderEdges = false)
{
    requirePerFaceAdjacentFaces(m);

    std::vector<std::pair<uint, uint>> creaseEdges;

    for (const auto& f : m.faces()) {
        for (uint i = 0; i < f.vertexNumber(); ++i) {
            if (f.adjFace(i) == nullptr) {
                // border edge
                if (alsoBorderEdges) {
                    creaseEdges.push_back({f.index(), i});
                }
            }
            else {
                // internal edge
                double angle = faceDihedralAngleOnEdge(f, i);
                if (angle < angleRadNeg || angle > angleRadPos) {
                    creaseEdges.push_back({f.index(), i});
                }
            }
        }
    }
    return creaseEdges;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_GEOMETRY_H
