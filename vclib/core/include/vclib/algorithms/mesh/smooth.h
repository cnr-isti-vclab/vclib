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

#ifndef VCL_ALGORITHMS_MESH_SMOOTH_H
#define VCL_ALGORITHMS_MESH_SMOOTH_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/complex/kd_tree.h>

#include <cmath>
#include <vector>

namespace vcl {

namespace detail {

template<typename PositionType>
struct LaplacianInfo
{
    using ScalarType = PositionType::ScalarType;
    PositionType sum;
    ScalarType   cnt;
};

template<typename MeshType, typename PositionType>
void accumulateLaplacianInfo(
    const MeshType&                           m,
    std::vector<LaplacianInfo<PositionType>>& data,
    bool                                      cotangentFlag = false)
{
    using ScalarType = PositionType::ScalarType;
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    ScalarType weight = 1.0f;

    for (const FaceType& f : m.faces()) {
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (!f.edgeOnBorder(j)) {
                const VertexType&   v0 = *f.vertex(j);
                const VertexType&   v1 = *f.vertexMod(j + 1);
                const VertexType&   v2 = *f.vertexMod(j + 2);
                const PositionType& p0 = v0.position();
                const PositionType& p1 = v1.position();
                const PositionType& p2 = v2.position();
                if (cotangentFlag) {
                    ScalarType angle = PositionType(p1 - p2).angle(p0 - p2);
                    weight           = std::tan((M_PI * 0.5) - angle);
                }

                data[m.index(v0)].sum +=
                    f.vertexMod(j + 1)->position() * weight;
                data[m.index(v1)].sum += f.vertex(j)->position() * weight;
                data[m.index(v0)].cnt += weight;
                data[m.index(v1)].cnt += weight;
            }
        }
    }
    // si azzaera i dati per i vertici di bordo
    for (const FaceType& f : m.faces()) {
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (f.edgeOnBorder(j)) {
                const VertexType&   v0 = *f.vertex(j);
                const VertexType&   v1 = *f.vertexMod(j + 1);
                const PositionType& p0 = v0.position();
                const PositionType& p1 = v1.position();
                data[m.index(v0)].sum  = p0;
                data[m.index(v1)].sum  = p1;
                data[m.index(v0)].cnt  = 1;
                data[m.index(v1)].cnt  = 1;
            }
        }
    }

    // se l'edge j e' di bordo si deve mediare solo con gli adiacenti
    for (const FaceType& f : m.faces()) {
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (f.edgeOnBorder(j)) {
                const VertexType&   v0 = *f.vertex(j);
                const VertexType&   v1 = *f.vertexMod(j + 1);
                const PositionType& p0 = v0.position();
                const PositionType& p1 = v1.position();
                data[m.index(v0)].sum += p1;
                data[m.index(v1)].sum += p0;
                ++data[m.index(v0)].cnt;
                ++data[m.index(v1)].cnt;
            }
        }
    }
}

} // namespace detail

/**
 * @brief Performs the classical Laplacian smoothing. Each
 * vertex is moved onto the average of the adjacent vertices.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param m: the mesh that will be smoothed
 * @param step
 * @param smoothSelected
 * @param cotangentWeight
 */
template<FaceMeshConcept MeshType>
void laplacianSmoothing(
    MeshType& m,
    uint      step,
    bool      smoothSelected  = false,
    bool      cotangentWeight = false /*, CallBackPos *cb*/)
{
    using VertexType   = MeshType::VertexType;
    using PositionType = VertexType::PositionType;

    const detail::LaplacianInfo<PositionType> lpz = {PositionType(0, 0, 0), 0};

    for (uint i = 0; i < step; ++i) {
        std::vector<detail::LaplacianInfo<PositionType>> laplData(
            m.vertexContainerSize(), lpz);
        detail::accumulateLaplacianInfo(m, laplData, cotangentWeight);
        for (VertexType& v : m.vertices()) {
            if (laplData[m.index(v)].cnt > 0) {
                if (!smoothSelected || v.selected()) {
                    v.position() = (v.position() + laplData[m.index(v)].sum) /
                                   (laplData[m.index(v)].cnt + 1);
                }
            }
        }
    }
}

template<FaceMeshConcept MeshType>
void taubinSmoothing(
    MeshType& m,
    uint      step,
    float     lambda,
    float     mu,
    bool      smoothSelected = false /*, CallBackPos *cb*/)
{
    using VertexType   = MeshType::VertexType;
    using PositionType = VertexType::PositionType;

    const detail::LaplacianInfo<PositionType> lpz = {PositionType(0, 0, 0), 0};

    for (uint i = 0; i < step; ++i) {
        std::vector<detail::LaplacianInfo<PositionType>> laplData(
            m.vertexContainerSize(), lpz);
        detail::accumulateLaplacianInfo(m, laplData);
        for (VertexType& v : m.vertices()) {
            if (laplData[m.index(v)].cnt > 0) {
                if (!smoothSelected || v.selected()) {
                    PositionType delta =
                        laplData[m.index(v)].sum / laplData[m.index(v)].cnt -
                        v.position();
                    v.position() = v.position() + delta * lambda;
                }
            }
        }
        std::fill(laplData.begin(), laplData.end(), lpz);
        detail::accumulateLaplacianInfo(m, laplData);
        for (VertexType& v : m.vertices()) {
            if (laplData[m.index(v)].cnt > 0) {
                if (!smoothSelected || v.selected()) {
                    PositionType delta =
                        laplData[m.index(v)].sum / laplData[m.index(v)].cnt -
                        v.position();
                    v.position() = v.position() + delta * mu;
                }
            }
        }
    }
}

/**
 * @brief smoothPerVertexNormalsPointCloud
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param m
 * @param tree
 * @param neighborNum
 * @param iterNum
 */
template<MeshConcept MeshType, PointConcept PointType>
void smoothPerVertexNormalsPointCloud(
    MeshType&                m,
    const KDTree<PointType>& tree,
    uint                     neighborNum,
    uint                     iterNum)
{
    requirePerVertexNormal(m);

    using Scalar     = PointType::ScalarType;
    using VertexType = MeshType::VertexType;
    using NormalType = VertexType::NormalType;

    std::vector<NormalType> TD(m.vertexContainerSize(), NormalType(0, 0, 0));

    for (uint ii = 0; ii < iterNum; ++ii) {
        for (const VertexType& v : m.vertices()) {
            std::vector<Scalar> distances;

            std::vector<uint> neighbors = tree.kNearestNeighborsIndices(
                v.position(), neighborNum, distances);

            for (uint nid : neighbors) {
                if (m.vertex(nid).normal() * v.normal() > 0) {
                    TD[m.index(v)] += m.vertex(nid).normal();
                }
                else {
                    TD[m.index(v)] -= m.vertex(nid).normal();
                }
            }
        }
        for (VertexType& v : m.vertices()) {
            v.normal() = TD[m.index(v)];
        }
    }
}

/**
 * @brief smoothPerVertexNormalsPointCloud
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param m
 * @param neighborNum
 * @param iterNum
 */
template<MeshConcept MeshType>
void smoothPerVertexNormalsPointCloud(
    MeshType& m,
    uint      neighborNum,
    uint      iterNum)
{
    using PointType = MeshType::VertexType::PositionType;
    KDTree<PointType> tree(m);
    smoothPerVertexNormalsPointCloud(m, tree, neighborNum, iterNum);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_SMOOTH_H
