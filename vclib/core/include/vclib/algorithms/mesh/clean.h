// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_CLEAN_H
#define VCL_ALGORITHMS_MESH_CLEAN_H

#include <vclib/algorithms/mesh/delete.h>
#include <vclib/algorithms/mesh/sort.h>
#include <vclib/algorithms/mesh/stat/topology.h>

#include <vclib/algorithms/core.h>
#include <vclib/mesh.h>
#include <vclib/space/complex.h>

#include <algorithm>
#include <numeric>
#include <vector>

/**
 * @defgroup clean Cleaning and Repairing Algorithms
 *
 * @ingroup algorithms_mesh
 *
 * @brief List of Mesh Cleaning and repairing algorithms.
 *
 * This group contains algorithms for cleaning and repairing meshes, along with
 * algorithms for checking the mesh topology that allow to identify issues
 * in the mesh structure.
 *
 * You can access these algorithms by including
 * `#include <vclib/algorithms/mesh.h>`
 */

namespace vcl {

namespace detail {

/* Comparison class for vertex position comparison */
template<typename MeshType>
class VertPositionComparator
{
    const MeshType* mMesh = nullptr;

public:
    VertPositionComparator(const MeshType& m) : mMesh(&m) {}

    inline bool operator()(
        typename MeshType::IndexType a,
        typename MeshType::IndexType b) const
    {
        const auto& pa = mMesh->vertex(a).position();
        const auto& pb = mMesh->vertex(b).position();
        return (pa == pb) ? (a < b) : (pa < pb);
    }
};

/**
 * @brief The SortedIndexContainer class stores a sorted container of indices of
 * type IndexType, plus a Sentinel value.
 *
 * The size of the container can be specified at compile time, or left
 * unspecified with the -1 value (in this case, the container is dynamically
 * allocated).
 *
 * The container provides the operator < and ==, that allow to sort and compare
 * two containers. Two containers with same indices but different Sentinel
 * values are considered equal.
 */
template<typename IndexType, typename SentinelType, int N>
class SortedIndexContainer
{
public:
    SortedIndexContainer() = default;

    template<Range RangeType>
    SortedIndexContainer(SentinelType s, RangeType&& rng) :
        s(s), v(std::forward<RangeType>(rng))
    {
        std::sort(v.begin(), v.end());
    }

    bool operator<(const SortedIndexContainer& other) const
    {
        if constexpr (N >= 0) {
            for (uint i = 0; i < N; ++i) {
                if (v[i] != other.v[i])
                    return v[i] < other.v[i];
            }
            return false;
        }
        else {
            for (std::size_t i = 0; i < v.size() && i < other.v.size(); ++i) {
                if (v[i] != other.v[i])
                    return v[i] < other.v[i];
            }
            return v.size() < other.v.size();
        }
    }

    bool operator==(const SortedIndexContainer& other) const
    {
        if constexpr (N >= 0) {
            for (uint i = 0; i < N; ++i) {
                if (v[i] != other.v[i])
                    return false;
            }
            return true;
        }
        else {
            if (v.size() != other.v.size())
                return false;
            for (std::size_t i = 0; i < v.size(); ++i) {
                if (v[i] != other.v[i])
                    return false;
            }
            return true;
        }
    }

    SentinelType sentinel() const { return s; }

private:
    Vector<IndexType, N> v;
    SentinelType         s;
};

template<typename Cont, MeshConcept MeshType>
void deleteElementsWithDeletedVertices(
    MeshType&                     m,
    typename MeshType::IndexType& deletedCount)
{
    if constexpr (comp::HasVertexReferences<typename Cont::ElementType>) {
        constexpr ElemId ELEM_ID = Cont::ElementType::ELEMENT_ID;
        deletedCount +=
            deleteElementsIf<MeshType, ELEM_ID>(m, [](auto el) {
                for (auto v : el.vertices()) {
                    if (v == nullptr || v.deleted()) {
                        return true;
                    }
                }
                return false;
            });
    }
}

template<typename... Cont, MeshConcept MeshType>
void deleteElementsWithDeletedVertices(
    MeshType&                     m,
    typename MeshType::IndexType& deletedCount,
    TypeWrapper<Cont...>)
{
    (deleteElementsWithDeletedVertices<Cont>(m, deletedCount), ...);
}

} // namespace detail

/**
 * @brief Marks as deleted all the non-deleted unreferenced vertices of the
 * mesh.
 *
 * This function marks as deleted all vertices in the input mesh that are not
 * referenced by any of the mesh's elements, and which have not already been
 * marked as deleted.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the MeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove the unreferenced
 * vertices. This mesh will be modified in place, with all unreferenced vertices
 * being marked as deleted.
 *
 * @return The number of non-deleted vertices that were marked as deleted.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
auto removeUnreferencedVertices(MeshType& m)
{
    using IndexType = typename MeshType::IndexType;

    // Generate a vector of boolean flags indicating whether each vertex is
    // referenced by any of the mesh's elements.
    IndexType         n           = 0;
    std::vector<bool> refVertices = referencedVertices(m, n);

    if (n > 0) {
        return deleteVerticesIf(
            m, [&](auto v) { return !refVertices[v.index()]; });
    }

    return IndexType(0);
}

/**
 * @brief Marks as deleted the duplicate vertices of the mesh, by looking only
 * at their spatial positions.
 *
 * This function marks as deleted all vertices in the input mesh that have the
 * same spatial position as another vertex in the mesh. The comparison of vertex
 * positions is based on the `position()` function of the vertex type, which
 * must return a 3D point representing the vertex position.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the MeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove duplicate vertices. This
 * mesh will be modified in place, with all duplicate vertices being marked as
 * deleted.
 * @return The number of duplicated vertices that were marked as deleted.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
auto removeDuplicateVertices(MeshType& m)
{
    using IndexType = typename MeshType::IndexType;

    if (m.vertexCount() == 0)
        return IndexType(0);

    // a map that will be used to keep track of deleted vertices and their
    // corresponding indices.
    std::vector<IndexType> newVertexIndices(m.vertexContainerSize());
    // assigning each vertex index to itself.
    std::iota(newVertexIndices.begin(), newVertexIndices.end(), IndexType(0));

    // mark any already deleted vertices as NULL_INDEX
    for (IndexType v = 0; v < m.vertexContainerSize(); ++v) {
        if (m.vertex(v).deleted()) {
            newVertexIndices[v] = MeshType::NULL_INDEX;
        }
    }

    IndexType deleted = 0;

    std::vector<IndexType> perm;
    perm.reserve(m.vertexCount());

    // put all the active vertices into a vector for sorting.
    for (auto v : m.vertices()) {
        perm.push_back(v.index());
    }

    // sort the vector based on the vertices' spatial positions.
    std::sort(
        std::execution::par_unseq,
        perm.begin(),
        perm.end(),
        detail::VertPositionComparator<MeshType>(m));

    std::size_t i = 0;

    // compare the i-th position with the next ones while they are equal to the
    // i-th.
    while (i + 1 < perm.size()) {
        std::size_t j = i + 1;
        while (j < perm.size() &&
               m.vertex(perm[i]).position() == m.vertex(perm[j]).position()) {
            // j will be deleted, so we map its index to the i-th vertex's index.
            newVertexIndices[perm[j]] = perm[i]; // map j -> i
            m.deleteVertex(perm[j]);
            j++;
            deleted++;
        }
        // here perm[i] != perm[j], so we need to check perm[j] with the next
        // vertex.
        i = j;
    }

    // update the vertex references to point to the correct vertices, in every
    // container of the mesh
    if (deleted > 0) {
        m.updateVertexReferences(newVertexIndices);
    }

    // todo:
    // - add a flag that removes degenerate elements after
    return deleted;
}

/**
 * @brief Removes all duplicate faces of the mesh by looking only at their
 * vertex references.
 *
 * This function removes all faces in the input mesh that have the same vertex
 * references as another face in the mesh. The comparison of face vertex
 * references is based on the indices of the face vertices, so it assumes that
 * the mesh's vertices have already been unified.
 *
 * @note This function automatically updates all internal topological references
 * to the deleted faces, invalidating them (setting them to `MeshType::NULL_INDEX`).
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * FaceMeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove duplicate faces. This
 * mesh will be modified in place, with all duplicate faces being marked as
 * deleted.
 * @return The number of duplicated faces that were marked as deleted.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
auto removeDuplicateFaces(MeshType& m)
{
    using IndexType = typename MeshType::IndexType;
    using FaceType  = typename MeshType::Face;

    // create a vector of sorted tuples of indices, where each tuple represents
    // a face's vertices and its face index.
    std::vector<detail::SortedIndexContainer<
        IndexType,
        IndexType,
        FaceType::VERTEX_COUNT>>
        fvec;
    fvec.reserve(m.faceCount());

    for (auto f : m.faces()) {
        fvec.emplace_back(f.index(), f.vertexIndices());
    }

    // sort the vector based on the face vertex indices.
    std::sort(std::execution::par_unseq, fvec.begin(), fvec.end());

    std::vector<bool> shouldDelete(m.faceContainerSize(), false);

    // iterate over the sorted vector, and mark any duplicate faces as deleted.
    for (std::size_t i = 0; i + 1 < fvec.size(); ++i) {
        if (fvec[i] == fvec[i + 1]) {
            shouldDelete[fvec[i].sentinel()] = true;
        }
    }

    return deleteFacesIf(m, [&](auto f) { return shouldDelete[f.index()]; });
}

/**
 * @brief Removes all vertices that have position with invalid floating point
 * values (NaN or inf).
 *
 * This function removes all vertices in the input mesh that have position
 * with invalid floating point values, such as NaN or inf. If the flag
 * `deleteAlsoIncidentElements` is set to true, all elements (e.g. faces or
 * edges) incident on deleted vertices are also deleted.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * MeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove degenerated vertices.
 * This mesh will be modified in place, with all degenerated vertices being
 * marked as deleted.
 *
 * @param[in] deleteAlsoIncidentElements: If true, all elements incident on
 * deleted vertices will also be deleted.
 *
 * @return The number of degenerated vertices that were marked as deleted.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
auto removeDegenerateVertices(MeshType& m, bool deleteAlsoIncidentElements = false)
{
    using IndexType = typename MeshType::IndexType;

    auto count_vd = deleteVerticesIf(
        m, [](auto v) { return v.position().isDegenerate(); });

    if (deleteAlsoIncidentElements && count_vd > 0) {
        IndexType dummy = 0;
        detail::deleteElementsWithDeletedVertices(
            m, dummy, typename MeshType::ElementContainers());
    }

    return count_vd;
}

/**
 * @brief Removes or repairs all degenerate faces from the input mesh.
 *
 * This function removes or repairs all faces in the input mesh that are
 * topologically degenerate, meaning that they have two or more consecutive
 * vertex references that link the same vertex.
 *
 * For fixed-size faces (e.g. Triangle meshes), any face containing consecutive
 * duplicated vertices is completely marked as deleted. For dynamic-size faces
 * (e.g. Polygon meshes), the function will attempt to "repair" the face by
 * erasing the duplicated vertex references. If after this process the polygon
 * ends up having fewer than 3 vertices, it will be marked as deleted.
 *
 * All degenerate faces are zero area faces, but not all zero area faces are
 * degenerate (for example, a face with three different vertex references, but
 * two of them have the same position). Therefore, if you also want to remove
 * these kinds of faces, you should call `removeDuplicateVertices(m)` first.
 *
 * @note This function automatically updates all internal topological references
 * to the deleted faces, invalidating them (setting them to `MeshType::NULL_INDEX`).
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the
 * FaceMeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove degenerate faces. This
 * mesh will be modified in place.
 *
 * @return The number of degenerate faces that were marked as deleted.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
auto removeDegenerateFaces(MeshType& m)
{
    using FaceType = typename MeshType::Face;

    if constexpr (PolygonFaceConcept<FaceType>) {
        for (auto f : m.faces()) {
            for (int i = int(f.vertexCount()) - 1; i >= 0; --i) {
                if (f.vertexCount() < 3)
                    break;
                if (f.vertexIndex(i) == f.vertexIndexMod(i + 1)) {
                    f.eraseVertex(i);
                }
            }
        }
    }

    return deleteFacesIf(m, [&](auto f) {
        if constexpr (PolygonFaceConcept<FaceType>) {
            if (f.vertexCount() < 3) {
                return true;
            }
        }
        for (uint i = 0; i < f.vertexCount(); ++i) {
            if (f.vertexIndex(i) == f.vertexIndexMod(i + 1)) {
                return true;
            }
        }
        return false;
    });
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CLEAN_H
