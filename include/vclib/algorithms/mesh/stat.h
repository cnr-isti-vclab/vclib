/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_ALGORITHMS_MESH_STAT_H
#define VCL_ALGORITHMS_MESH_STAT_H

#include <vector>

#include <vclib/mesh/utils/mesh_inertia.h>
#include <vclib/space/matrix.h>
#include <vclib/space/polygon.h>

#include "stat/quality.h"
#include "stat/selection.h"

namespace vcl {

/**
 * @brief Returns the barycenter of the mesh, that is the simple average of all
 * the vertex coordintes of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param[in] m: input mesh on which compute the barycenter.
 * @return The barycenter of the input mesh.
 */
template<MeshConcept MeshType>
typename MeshType::VertexType::CoordType barycenter(const MeshType& m)
{
    using VertexType = MeshType::VertexType;
    using CoordType  = VertexType::CoordType;

    CoordType bar;

    for (const VertexType& v : m.vertices()) {
        bar += v.coord();
    }

    return bar / m.vertexNumber();
}

/**
 * @brief Returns the barycenter of the mesh weighted on the per vertex quality
 * values.
 *
 * The output baryceter is computed as a weighted average of the vertices of the
 * mesh, using the per Vertex Quality values as weights.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in] m: input mesh on which compute the barycenter.
 * @return The barycenter weighted on the per vertex quality.
 */
template<MeshConcept MeshType>
typename MeshType::VertexType::CoordType scalarWeightedBarycenter(
    const MeshType& m)
{
    vcl::requirePerVertexQuality(m);

    using VertexType  = MeshType::VertexType;
    using CoordType   = VertexType::CoordType;
    using QualityType = VertexType::QualityType;

    CoordType   bar;
    QualityType weightedSum = 0;

    for (const VertexType& v : m.vertices()) {
        bar += v.coord() * v.quality();
        weightedSum += v.quality();
    }

    return bar / weightedSum;
}

/**
 * @brief Computes the barycenter of the surface thin-shell.
 * E.g. it assume a 'empty' model where all the mass is located on the surface
 * and compute the barycenter of that thinshell. Works for any polygonal model
 * (no problem with open, nonmanifold selfintersecting models). Useful for
 * computing the barycenter of 2D planar figures.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param m
 * @return
 */
template<FaceMeshConcept MeshType>
typename MeshType::VertexType::CoordType shellBarycenter(const MeshType& m)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;
    using CoordType  = VertexType::CoordType;
    using ScalarType = CoordType::ScalarType;

    CoordType bar;
    bar.setZero();
    ScalarType areaSum = 0;

    for (const FaceType& f : m.faces()) {
        ScalarType area = faceArea(f);
        bar += faceBarycenter(f) * area;
        areaSum += area;
    }

    return bar / areaSum;
}

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
 * length of the edges that are on border in the given mesh.
 *
 * @param[in] m: mesh on which compute the border length.
 * @return The border length of the given mesh.
 */
template<FaceMeshConcept MeshType>
double borderLength(const MeshType& m)
{
    using FaceType = MeshType::FaceType;

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
 * @brief Compute the covariance matrix of a set of points.
 * @param pointVec
 * @return The 3x3 covariance matrix of the given set of points.
 */
template<PointConcept PointType>
auto covarianceMatrixOfPointCloud(const std::vector<PointType>& pointVec)
{
    Matrix33<typename PointType::ScalarType> m;
    m.setZero();
    PointType barycenter =
        Polygon<PointType>::barycenter(pointVec.begin(), pointVec.end());

    // compute covariance matrix
    for (const PointType& p : pointVec) {
        PointType e = p - barycenter;
        m += e.outerProduct(e);
    }
    return m;
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
    using ScalarType = VertexType::CoordType::ScalarType;

    auto barycenter = vcl::barycenter(m);

    Matrix33<ScalarType> mm;
    mm.setZero();
    // compute covariance matrix
    for (const VertexType& v : m.vertices()) {
        auto e = v.coord() - barycenter;
        m += e.outerProduct(e);
    }
    return m;
}

/**
 * @brief Compute the weighted covariance matrix of a set of points.
 * @param pointVec
 * @param weights
 * @return
 */
template<PointConcept PointType>
auto weightedCovarianceMatrixOfPointCloud(
    const std::vector<PointType>&                      pointVec,
    const std::vector<typename PointType::ScalarType>& weights)
{
    Matrix33<typename PointType::ScalarType> m;
    m.setZero();
    PointType barycenter = polygonWeighedBarycenter(
        pointVec.begin(), pointVec.end(), weights.begin());

    // compute covariance matrix
    typename PointType::ScalarType wsum = 0;
    for (uint i = 0; i < pointVec.size(); ++i) {
        PointType e = (pointVec[i] - barycenter) * weights[i];
        m += e.outerProduct(e);
        wsum += weights[i];
    }
    return m / wsum;
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
    const MeshType&                m,
    const std::vector<ScalarType>& weights,
    double                         diskRadius,
    double                         radiusVariance,
    bool                           invert = false)
{
    using VertexType = MeshType::VertexType;

    std::vector<ScalarType> radius(m.vertexContainerSize());
    auto minmax = std::minmax_element(weights.begin(), weights.end());

    float minRad   = diskRadius;
    float maxRad   = diskRadius * radiusVariance;
    float deltaQ   = *minmax.second - *minmax.first;
    float deltaRad = maxRad - minRad;
    for (const VertexType& v : m.vertices()) {
        ScalarType w = weights[m.index(v)];
        radius[m.index(v)] =
            minRad +
            deltaRad *
                ((invert ? *minmax.second - w : w - *minmax.first) / deltaQ);
    }

    return radius;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_H
