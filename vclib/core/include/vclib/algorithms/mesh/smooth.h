// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_SMOOTH_H
#define VCL_ALGORITHMS_MESH_SMOOTH_H

#include <vclib/mesh.h>
#include <vclib/space/complex.h>

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

template<typename MeshType>
void accumulateLaplacianInfo(
    const MeshType&                           m,
    std::vector<LaplacianInfo<PositionType<MeshType>>>& data,
    bool                                      cotangentFlag = false)
{
    using PositionType = PositionType<MeshType>;
    using ScalarType = PositionType::ScalarType;

    ScalarType weight = 1.0f;

    for (auto&& f : m.faces()) {
        for (uint j = 0; j < f.vertexCount(); ++j) {
            if (!f.edgeOnBorder(j)) {
                auto   v0 = f.vertex(j);
                auto   v1 = f.vertexMod(j + 1);
                auto   v2 = f.vertexMod(j + 2);
                const PositionType& p0 = v0.position();
                const PositionType& p1 = v1.position();
                const PositionType& p2 = v2.position();
                if (cotangentFlag) {
                    ScalarType angle = PositionType(p1 - p2).angle(p0 - p2);
                    weight           = std::tan((M_PI * 0.5) - angle);
                }

                data[v0.index()].sum += p1 * weight;
                data[v1.index()].sum += p0 * weight;
                data[v0.index()].cnt += weight;
                data[v1.index()].cnt += weight;
            }
        }
    }
    // si azzaera i dati per i vertici di bordo
    for (auto&& f : m.faces()) {
        for (uint j = 0; j < f.vertexCount(); ++j) {
            if (f.edgeOnBorder(j)) {
                auto   v0 = f.vertex(j);
                auto   v1 = f.vertexMod(j + 1);
                data[v0.index()].sum  = v0.position();
                data[v1.index()].sum  = v1.position();
                data[v0.index()].cnt  = 1;
                data[v1.index()].cnt  = 1;
            }
        }
    }

    // se l'edge j e' di bordo si deve mediare solo con gli adiacenti
    for (auto&& f : m.faces()) {
        for (uint j = 0; j < f.vertexCount(); ++j) {
            if (f.edgeOnBorder(j)) {
                auto   v0 = f.vertex(j);
                auto   v1 = f.vertexMod(j + 1);
                data[v0.index()].sum += v1.position();
                data[v1.index()].sum += v0.position();
                ++data[v0.index()].cnt;
                ++data[v1.index()].cnt;
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
    using PositionType = PositionType<MeshType>;

    const detail::LaplacianInfo<PositionType> lpz = {PositionType(0, 0, 0), 0};

    for (uint i = 0; i < step; ++i) {
        std::vector<detail::LaplacianInfo<PositionType>> laplData(
            m.vertexContainerSize(), lpz);
        detail::accumulateLaplacianInfo(m, laplData, cotangentWeight);
        for (auto v : m.vertices()) {
            if (laplData[v.index()].cnt > 0) {
                if (!smoothSelected || v.selected()) {
                    v.position() = (v.position() + laplData[v.index()].sum) /
                                   (laplData[v.index()].cnt + 1);
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
    using PositionType = PositionType<MeshType>;

    const detail::LaplacianInfo<PositionType> lpz = {PositionType(0, 0, 0), 0};

    for (uint i = 0; i < step; ++i) {
        std::vector<detail::LaplacianInfo<PositionType>> laplData(
            m.vertexContainerSize(), lpz);
        detail::accumulateLaplacianInfo(m, laplData);
        for (auto v : m.vertices()) {
            if (laplData[v.index()].cnt > 0) {
                if (!smoothSelected || v.selected()) {
                    PositionType delta =
                        laplData[v.index()].sum / laplData[v.index()].cnt -
                        v.position();
                    v.position() = v.position() + delta * lambda;
                }
            }
        }
        std::fill(laplData.begin(), laplData.end(), lpz);
        detail::accumulateLaplacianInfo(m, laplData);
        for (auto v : m.vertices()) {
            if (laplData[v.index()].cnt > 0) {
                if (!smoothSelected || v.selected()) {
                    PositionType delta =
                        laplData[v.index()].sum / laplData[v.index()].cnt -
                        v.position();
                    v.position() = v.position() + delta * mu;
                }
            }
        }
    }
}

/**
 * @brief smoothVertexNormalsPointCloud
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param m
 * @param tree
 * @param neighborCount
 * @param iterCount
 */
template<MeshConcept MeshType, PointConcept PointType>
void smoothVertexNormalsPointCloud(
    MeshType&                m,
    const KDTree<PointType>& tree,
    uint                     neighborCount,
    uint                     iterCount)
{
    requireVertexNormal(m);

    using Scalar     = PointType::ScalarType;
    using NormalType = VertexType<MeshType>::NormalType;

    std::vector<NormalType> TD(m.vertexContainerSize(), NormalType(0, 0, 0));

    for (uint ii = 0; ii < iterCount; ++ii) {
        for (const auto& v : m.vertices()) {
            std::vector<Scalar> distances;

            std::vector<std::size_t> neighbors = tree.kNearestNeighborsIndices(
                v.position(), neighborCount, distances);

            for (auto nid : neighbors) {
                if (m.vertex(nid).normal() * v.normal() > 0) {
                    TD[v.index()] += m.vertex(nid).normal();
                }
                else {
                    TD[v.index()] -= m.vertex(nid).normal();
                }
            }
        }
        for (auto v : m.vertices()) {
            v.normal() = TD[v.index()];
        }
    }
}

/**
 * @brief smoothVertexNormalsPointCloud
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Normal
 *
 * @param m
 * @param neighborCount
 * @param iterCount
 */
template<MeshConcept MeshType>
void smoothVertexNormalsPointCloud(
    MeshType& m,
    uint      neighborCount,
    uint      iterCount)
{
    using PointType = PositionType<MeshType>;
    KDTree<PointType> tree(m);
    smoothVertexNormalsPointCloud(m, tree, neighborCount, iterCount);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_SMOOTH_H
