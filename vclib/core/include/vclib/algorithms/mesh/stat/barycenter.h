// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_STAT_BARYCENTER_H
#define VCL_ALGORITHMS_MESH_STAT_BARYCENTER_H

#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief Returns the barycenter of the mesh, that is the simple average of all
 * the vertex positions of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param[in] m: input mesh on which compute the barycenter.
 * @return The barycenter of the input mesh.
 */
template<MeshConcept MeshType>
auto barycenter(const MeshType& m) -> MeshType::VertexType::PositionType
{
    using VertexType   = MeshType::VertexType;
    using PositionType = VertexType::PositionType;

    PositionType bar;

    for (const VertexType& v : m.vertices()) {
        bar += v.position();
    }

    return bar / m.vertexCount();
}

/**
 * @brief Returns the barycenter of the mesh weighted on the given range.
 *
 * The output barycenter is computed as a weighted average of the vertices of
 * the mesh, using the values in the input range as weights.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param[in] m: input mesh on which compute the barycenter.
 * @param[in] weights: range of weights to use for the weighted average.
 * @return The barycenter weighted on the input range.
 */
template<MeshConcept MeshType>
auto weightedBarycenter(const MeshType& m, Range auto&& weights)
    -> MeshType::VertexType::PositionType
{
    using VertexType   = MeshType::VertexType;
    using PositionType = VertexType::PositionType;
    using RType        = std::ranges::range_value_t<decltype(weights)>;

    assert(std::ranges::size(weights) == m.vertexCount());

    PositionType bar;
    RType        weightedSum = 0;

    for (const auto& [v, w] : std::views::zip(m.vertices(), weights)) {
        bar += v.position() * w;
        weightedSum += w;
    }

    return bar / weightedSum;
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
auto qualityWeightedBarycenter(const MeshType& m)
    -> MeshType::VertexType::PositionType
{
    requirePerVertexQuality(m);

    return weightedBarycenter(m, m.vertices() | views::quality);
}

/**
 * @brief Computes the barycenter of the surface thin-shell.
 * E.g. it assumes a 'empty' model where all the mass is located on the surface
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
auto shellBarycenter(const MeshType& m) -> MeshType::VertexType::PositionType
{
    using VertexType   = MeshType::VertexType;
    using FaceType     = MeshType::FaceType;
    using PositionType = VertexType::PositionType;
    using ScalarType   = PositionType::ScalarType;

    PositionType bar;
    bar.setZero();
    ScalarType areaSum = 0;

    for (const FaceType& f : m.faces()) {
        ScalarType area = faceArea(f);
        bar += faceBarycenter(f) * area;
        areaSum += area;
    }

    return bar / areaSum;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_BARYCENTER_H
