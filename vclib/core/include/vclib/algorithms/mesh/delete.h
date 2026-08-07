// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_DELETE_H
#define VCL_ALGORITHMS_MESH_DELETE_H

#include <vclib/mesh.h>
#include <ranges>

/**
 * @defgroup delete Deleting Algorithms
 *
 * @ingroup algorithms_mesh
 *
 * @brief List of Algorithms that delete elements from a mesh based on a given
 * filter condition.
 *
 * These algorithms allow to conditionally delete mesh elements (vertices,
 * faces, edges) in-place based on a filtering function or range. Elements
 * matching the condition will be deleted, and all internal topology references
 * pointing to the deleted elements will be automatically updated and
 * invalidated (set to `nullptr` / `UINT_NULL`).
 *
 * You can access these algorithms by including
 * `#include <vclib/algorithms/mesh.h>`
 */

namespace vcl {

namespace detail {

template<MeshConcept MeshType, uint ELEM_ID>
uint deleteElementsIf(MeshType& m, Range auto&& elemFilterRng)
{
    uint count = 0;

    if constexpr (MeshType::template hasContainerOf<ELEM_ID>()) {
        std::vector<uint> refIndices(
            m.template containerSize<ELEM_ID>(), UINT_NULL);

        for (const auto& [e, filter] :
             std::views::zip(m.template elements<ELEM_ID>(), elemFilterRng)) {
            if (filter) {
                m.template deleteElement<ELEM_ID>(m.index(e));
                count++;
            }
            else {
                refIndices[m.index(e)] = m.index(e);
            }
        }

        if (count > 0) {
            m.template updateReferences<ELEM_ID>(refIndices);
        }
    }

    return count;
}

template<MeshConcept MeshType, uint ELEM_ID>
uint deleteElementsIf(
    MeshType& m,
    const std::function<bool(
        const typename MeshType::template ElementType<ELEM_ID>&)>& elemFilter)
{
    auto view =
        m.template elements<ELEM_ID>() | std::views::transform(elemFilter);

    return deleteElementsIf<MeshType, ELEM_ID>(m, view);
}

} // namespace detail

/**
 * @brief Deletes all vertices from the mesh that satisfy the provided filter
 * function.
 *
 * This function iterates over all valid vertices and deletes those for which
 * the `vertexFilter` returns `true`. It also updates all the internal
 * references in the mesh (e.g., in faces or edges) so that references to the
 * deleted vertices are properly invalidated.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the `MeshConcept`.
 *
 * @param[in,out] m: input mesh, to be modified in-place.
 * @param[in] vertexFilter: a function that takes a vertex as input and returns
 * a boolean value that tells whether the vertex should be deleted (`true`) or
 * kept (`false`).
 *
 * @return The number of vertices deleted.
 *
 * @ingroup delete
 */
template<MeshConcept MeshType>
uint deleteVerticesIf(
    MeshType& m,
    const std::function<bool(const typename MeshType::VertexType&)>&
        vertexFilter)
{
    return detail::deleteElementsIf<MeshType, ElemId::VERTEX>(m, vertexFilter);
}

/**
 * @brief Deletes all vertices from the mesh whose corresponding flag in the
 * range is evaluated to `true`.
 *
 * This function iterates over all valid vertices and deletes those whose
 * corresponding flag in `vertexFilterRng` is `true`. It also updates all the
 * internal references in the mesh (e.g., in faces or edges) so that references
 * to the deleted vertices are properly invalidated.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the `MeshConcept`.
 *
 * @param[in,out] m: input mesh, to be modified in-place.
 * @param[in] vertexFilterRng: range of values that are evaluated as booleans,
 * one for each valid vertex of the input mesh. Its type must satisfy the
 * `Range` concept. If the value is `true`, the corresponding vertex is deleted.
 *
 * @return The number of vertices deleted.
 *
 * @ingroup delete
 */
template<MeshConcept MeshType>
uint deleteVerticesIf(MeshType& m, Range auto&& vertexFilterRng)
{
    return detail::deleteElementsIf<MeshType, ElemId::VERTEX>(
        m, vertexFilterRng);
}

/**
 * @brief Deletes all faces from the mesh that satisfy the provided filter
 * function.
 *
 * This function iterates over all valid faces and deletes those for which
 * the `faceFilter` returns `true`. It also updates all the internal
 * references in the mesh so that references to the deleted faces are properly
 * invalidated.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the
 * `FaceMeshConcept`.
 *
 * @param[in,out] m: input mesh, to be modified in-place.
 * @param[in] faceFilter: a function that takes a face as input and returns a
 * boolean value that tells whether the face should be deleted (`true`) or kept
 * (`false`).
 *
 * @return The number of faces deleted.
 *
 * @ingroup delete
 */
template<FaceMeshConcept MeshType>
uint deleteFacesIf(
    MeshType& m,
    const std::function<bool(const typename MeshType::FaceType&)>& faceFilter)
{
    return detail::deleteElementsIf<MeshType, ElemId::FACE>(m, faceFilter);
}

/**
 * @brief Deletes all faces from the mesh whose corresponding flag in the range
 * is evaluated to `true`.
 *
 * This function iterates over all valid faces and deletes those whose
 * corresponding flag in `faceFilterRng` is `true`. It also updates all the
 * internal references in the mesh so that references to the deleted faces are
 * properly invalidated.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the
 * `FaceMeshConcept`.
 *
 * @param[in,out] m: input mesh, to be modified in-place.
 * @param[in] faceFilterRng: range of values that are evaluated as booleans,
 * one for each valid face of the input mesh. Its type must satisfy the
 * `Range` concept. If the value is `true`, the corresponding face is deleted.
 *
 * @return The number of faces deleted.
 *
 * @ingroup delete
 */
template<FaceMeshConcept MeshType>
uint deleteFacesIf(MeshType& m, Range auto&& faceFilterRng)
{
    return detail::deleteElementsIf<MeshType, ElemId::FACE>(m, faceFilterRng);
}

/**
 * @brief Deletes all edges from the mesh that satisfy the provided filter
 * function.
 *
 * This function iterates over all valid edges and deletes those for which
 * the `edgeFilter` returns `true`. It also updates all the internal
 * references in the mesh so that references to the deleted edges are properly
 * invalidated.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the
 * `EdgeMeshConcept`.
 *
 * @param[in,out] m: input mesh, to be modified in-place.
 * @param[in] edgeFilter: a function that takes an edge as input and returns a
 * boolean value that tells whether the edge should be deleted (`true`) or kept
 * (`false`).
 *
 * @return The number of edges deleted.
 *
 * @ingroup delete
 */
template<EdgeMeshConcept MeshType>
uint deleteEdgesIf(
    MeshType& m,
    const std::function<bool(const typename MeshType::EdgeType&)>& edgeFilter)
{
    return detail::deleteElementsIf<MeshType, ElemId::EDGE>(m, edgeFilter);
}

/**
 * @brief Deletes all edges from the mesh whose corresponding flag in the range
 * is evaluated to `true`.
 *
 * This function iterates over all valid edges and deletes those whose
 * corresponding flag in `edgeFilterRng` is `true`. It also updates all the
 * internal references in the mesh so that references to the deleted edges are
 * properly invalidated.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the
 * `EdgeMeshConcept`.
 *
 * @param[in,out] m: input mesh, to be modified in-place.
 * @param[in] edgeFilterRng: range of values that are evaluated as booleans,
 * one for each valid edge of the input mesh. Its type must satisfy the
 * `Range` concept. If the value is `true`, the corresponding edge is deleted.
 *
 * @return The number of edges deleted.
 *
 * @ingroup delete
 */
template<EdgeMeshConcept MeshType>
uint deleteEdgesIf(MeshType& m, Range auto&& edgeFilterRng)
{
    return detail::deleteElementsIf<MeshType, ElemId::EDGE>(m, edgeFilterRng);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_DELETE_H
