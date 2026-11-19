// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_SHUFFLE_H
#define VCL_ALGORITHMS_MESH_SHUFFLE_H

#include <vclib/base/random.h>
#include <vclib/mesh.h>

/**
 * @defgroup shuffle Shuffling Algorithms
 *
 * @ingroup algorithms_mesh
 *
 * @brief List of Algorithms that create shuffled vectors of elements or indices
 * of a mesh.
 *
 * These algorithms allow to retrieve elements or element indices in a randomly
 * shuffled order, without mutating the container or order of elements in the
 * mesh.
 *
 * You can access these algorithms by including
 * `#include <vclib/algorithms/mesh.h>`
 */

namespace vcl {

namespace detail {

template<ElemId ELEM_ID, MeshConcept MeshType>
auto genericFillAndShuffleElementIndexVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    using Index = IndexType<MeshType>;

    std::vector<Index> vec;
    vec.reserve(m.template count<ELEM_ID>());

    for (const auto& el : m.template elements<ELEM_ID>()) {
        vec.push_back(el.index());
    }

    shuffle(vec, config);

    return vec;
}

template<ElemId ELEM_ID, typename MeshRef>
auto genericFillAndShuffleElementVector(
    MeshRef&&    m,
    RandomConfig config = std::monostate())
{
    using MeshType = std::remove_cvref_t<MeshRef>;
    auto indices = genericFillAndShuffleElementIndexVector<ELEM_ID>(m, config);

    using Elem = std::conditional_t<
        std::is_const_v<std::remove_reference_t<MeshRef>>,
        typename MeshType::template ConstElementType<ELEM_ID>,
        typename MeshType::template ElementType<ELEM_ID>>;

    std::vector<Elem> vec;
    vec.reserve(indices.size());

    for (auto idx : indices) {
        vec.push_back(m.template element<ELEM_ID>(idx));
    }

    return vec;
}

} // namespace detail

// ============================================================================
// Generic Element Functions
// ============================================================================

template<ElemId ELEM_ID, MeshConcept MeshType>
requires HasContainerOf<MeshType, ELEM_ID>
std::vector<IndexType<MeshType>> fillAndShuffleElementIndexVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    return detail::genericFillAndShuffleElementIndexVector<ELEM_ID>(m, config);
}

template<ElemId ELEM_ID, MeshConcept MeshType>
requires HasContainerOf<MeshType, ELEM_ID>
std::vector<typename MeshType::template ElementType<ELEM_ID>>
fillAndShuffleElementVector(MeshType& m, RandomConfig config = std::monostate())
{
    return detail::genericFillAndShuffleElementVector<ELEM_ID>(m, config);
}

template<ElemId ELEM_ID, MeshConcept MeshType>
requires HasContainerOf<MeshType, ELEM_ID>
std::vector<typename MeshType::template ConstElementType<ELEM_ID>>
fillAndShuffleElementVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    return detail::genericFillAndShuffleElementVector<ELEM_ID>(m, config);
}

// ============================================================================
// Vertex Shuffling
// ============================================================================

template<MeshConcept MeshType>
std::vector<VertexType<MeshType>> fillAndShuffleVertexVector(
    MeshType&    m,
    RandomConfig config = std::monostate())
{
    return fillAndShuffleElementVector<ElemId::VERTEX>(m, config);
}

template<MeshConcept MeshType>
std::vector<ConstVertexType<MeshType>> fillAndShuffleVertexVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    return fillAndShuffleElementVector<ElemId::VERTEX>(m, config);
}

template<MeshConcept MeshType>
std::vector<IndexType<MeshType>> fillAndShuffleVertexIndexVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    return fillAndShuffleElementIndexVector<ElemId::VERTEX>(m, config);
}

// ============================================================================
// Face Shuffling
// ============================================================================

template<FaceMeshConcept MeshType>
std::vector<FaceType<MeshType>> fillAndShuffleFaceVector(
    MeshType&    m,
    RandomConfig config = std::monostate())
{
    return fillAndShuffleElementVector<ElemId::FACE>(m, config);
}

template<FaceMeshConcept MeshType>
std::vector<ConstFaceType<MeshType>> fillAndShuffleFaceVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    return fillAndShuffleElementVector<ElemId::FACE>(m, config);
}

template<FaceMeshConcept MeshType>
std::vector<IndexType<MeshType>> fillAndShuffleFaceIndexVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    return fillAndShuffleElementIndexVector<ElemId::FACE>(m, config);
}

// ============================================================================
// Edge Shuffling
// ============================================================================

template<EdgeMeshConcept MeshType>
std::vector<EdgeType<MeshType>> fillAndShuffleEdgeVector(
    MeshType&    m,
    RandomConfig config = std::monostate())
{
    return fillAndShuffleElementVector<ElemId::EDGE>(m, config);
}

template<EdgeMeshConcept MeshType>
std::vector<ConstEdgeType<MeshType>> fillAndShuffleEdgeVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    return fillAndShuffleElementVector<ElemId::EDGE>(m, config);
}

template<EdgeMeshConcept MeshType>
std::vector<IndexType<MeshType>> fillAndShuffleEdgeIndexVector(
    const MeshType& m,
    RandomConfig    config = std::monostate())
{
    return fillAndShuffleElementIndexVector<ElemId::EDGE>(m, config);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_SHUFFLE_H
