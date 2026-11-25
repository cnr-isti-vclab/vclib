// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_STAT_QUALITY_H
#define VCL_ALGORITHMS_MESH_STAT_QUALITY_H

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <numeric>

namespace vcl {

/**
 * @brief Returns a pair containing the min and the maximum quality of the
 * elements of the given type.
 *
 * The mesh is expected to have a Element Container of the given ELEM_ID, and
 * the elements of the given type are expected to have a Quality component.
 *
 * @param[in] m: the input Mesh on which compute the minimum and the maximum
 * quality.
 * @return A `std::pair` having as first element the minimum, and as second
 * element the maximum quality.
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
auto elementQualityMinMax(const MeshType& m)
{
    requireElementComponent<ELEM_ID, CompId::QUALITY>(m);

    auto [min, max] =
        std::ranges::minmax(m.template elements<ELEM_ID>() | views::quality);

    return std::pair {min, max};
}

/**
 * @brief Returns a scalar that is the average of the quality of the elements of
 * the given type.
 *
 * The mesh is expected to have a Element Container of the given ELEM_ID, and
 * the elements of the given type are expected to have a Quality component.
 *
 * @param[in] m: the input Mesh on which compute the average of the quality.
 * @return The average of the quality of the elements of the given type.
 */
template<ElemId ELEM_ID, MeshConcept MeshType>
auto elementQualityAverage(const MeshType& m)
{
    requireElementComponent<ELEM_ID, CompId::QUALITY>(m);

    auto eq = m.template elements<ELEM_ID>() | views::quality;
    return std::accumulate(std::ranges::begin(eq), std::ranges::end(eq), 0.0) /
           m.template count<ELEM_ID>();
}

template<ElemId ELEM_ID, MeshConcept MeshType>
auto elementQualityHistogram(
    const MeshType& m,
    bool            selectionOnly = false,
    uint            histSize      = 10000)
{
    using QualityType =
        typename MeshType::template ElementType<ELEM_ID>::QualityType;

    requireElementComponent<ELEM_ID, CompId::QUALITY>(m);

    auto minmax = elementQualityMinMax<ELEM_ID>(m);

    Histogram<QualityType> h(minmax.first, minmax.second, histSize);
    for (const auto& e : m.template elements<ELEM_ID>()) {
        if (!selectionOnly || e.selected()) {
            assert(!isDegenerate(e.quality()));
            h.addValue(e.quality());
        }
    }
    return h;
}

/**
 * @brief Returns a pair containing the min and the maximum vertex quality.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in] m: the input Mesh on which compute the minimum and the maximum
 * quality.
 * @return A `std::pair` having as first element the minimum, and as second
 * element the maximum quality.
 */
template<MeshConcept MeshType>
auto vertexQualityMinMax(const MeshType& m)
{
    return elementQualityMinMax<ElemId::VERTEX>(m);
}

/**
 * @brief Returns a pair containing the min and the maximum face quality.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Quality
 *
 * @param[in] m: the input Mesh on which compute the minimum and the maximum
 * quality.
 * @return A `std::pair` having as first element the minimum, and as second
 * element the maximum quality.
 */
template<FaceMeshConcept MeshType>
auto faceQualityMinMax(const MeshType& m)
{
    return elementQualityMinMax<ElemId::FACE>(m);
}

/**
 * @brief Returns a scalar that is the average of the vertex quality.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in] m: the input Mesh on which compute the average of the quality.
 * @return The average of the vertex quality of the given mesh.
 */
template<MeshConcept MeshType>
auto vertexQualityAverage(const MeshType& m)
{
    return elementQualityAverage<ElemId::VERTEX>(m);
}

/**
 * @brief Returns a scalar that is the average of the face quality.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - Quality
 *
 * @param[in] m: the input Mesh on which compute the average of the quality.
 * @return The average of the face quality of the given mesh.
 */
template<FaceMeshConcept MeshType>
auto faceQualityAverage(const MeshType& m)
{
    return elementQualityAverage<ElemId::FACE>(m);
}

/**
 * @brief When performing an adptive pruning for each sample we expect a varying
 * radius to be removed. The radius is a vertex attribute that we compute
 * from the current vertex quality component. The expected radius of the
 * sample is computed so that it linearly maps the quality between diskradius
 * and diskradius*variance
 *
 * @param m
 * @param diskRadius
 * @param radiusVariance
 * @param invert
 * @return
 */
template<MeshConcept MeshType>
auto vertexRadiusFromQuality(
    const MeshType& m,
    double          diskRadius,
    double          radiusVariance,
    bool            invert = false)
{
    requireVertexQuality(m);

    using QualityType = VertexType<MeshType>::QualityType;

    std::vector<QualityType>            radius(m.vertexContainerSize());
    std::pair<QualityType, QualityType> minmax = vertexQualityMinMax(m);

    QualityType minRad   = diskRadius;
    QualityType maxRad   = diskRadius * radiusVariance;
    QualityType deltaQ   = minmax.second - minmax.first;
    QualityType deltaRad = maxRad - minRad;
    for (const auto& v : m.vertices()) {
        radius[v.index()] =
            minRad + deltaRad * ((invert ? minmax.second - v.quality() :
                                           v.quality() - minmax.first) /
                                 deltaQ);
    }

    return radius;
}

template<MeshConcept MeshType>
auto vertexQualityHistogram(
    const MeshType& m,
    bool            selectionOnly = false,
    uint            histSize      = 10000)
{
    return elementQualityHistogram<ElemId::VERTEX>(m, selectionOnly, histSize);
}

template<FaceMeshConcept MeshType>
auto faceQualityHistogram(
    const MeshType& m,
    bool            selectionOnly = false,
    uint            histSize      = 10000)
{
    return elementQualityHistogram<ElemId::FACE>(m, selectionOnly, histSize);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_QUALITY_H
