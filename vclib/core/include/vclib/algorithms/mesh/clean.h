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

#ifndef VCL_ALGORITHMS_MESH_CLEAN_H
#define VCL_ALGORITHMS_MESH_CLEAN_H

#include <vclib/algorithms/core/polygon/ear_cut.h>
#include <vclib/algorithms/mesh/sort.h>
#include <vclib/algorithms/mesh/stat/topology.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/complex/mesh_pos.h>

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
 * You can access these algorithms by including `#include
 * <vclib/algorithms/clean.h>`
 */

namespace vcl {

namespace detail {

/* classe di confronto per l'algoritmo di eliminazione vertici duplicati*/
template<typename VertexPointer>
class VertPositionComparator
{
public:
    inline bool operator()(const VertexPointer& a, const VertexPointer& b)
    {
        return (a->position() == b->position()) ?
                   (a < b) :
                   (a->position() < b->position());
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
    SortedIndexContainer() {}

    template<Range RangeType>
    SortedIndexContainer(SentinelType s, RangeType rng) : s(s), v(rng)
    {
        std::sort(v.begin(), v.end());
    }

    bool operator<(const SortedIndexContainer& s) const
    {
        if constexpr (N >= 0) {
            for (uint i = 0; i < N; ++i) {
                if (v[i] != s.v[i])
                    return v[i] < s.v[i];
            }
            return false;
        }
        else {
            for (uint i = 0; i < v.size() && i < s.v.size(); ++i) {
                if (v[i] != s.v[i])
                    return v[i] < s.v[i];
            }
            return v.size() < s.v.size();
        }
    }

    bool operator==(const SortedIndexContainer& s) const
    {
        if constexpr (N >= 0) {
            for (uint i = 0; i < N; ++i) {
                if (v[i] != s.v[i])
                    return false;
            }
            return true;
        }
        else {
            if (v.size() != s.v.size())
                return false;
            for (uint i = 0; i < v.size(); ++i) {
                if (v[i] != s.v[i])
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
uint removeUnreferencedVertices(MeshType& m)
{
    using VertexType = MeshType::VertexType;

    // Generate a vector of boolean flags indicating whether each vertex is
    // referenced by any of the mesh's elements.

    uint              n           = 0;
    std::vector<bool> refVertices = referencedVertices<std::vector<bool>>(m, n);

    // need to mark as deleted vertices only if the number of unreferenced is
    // less than vn
    if (n < m.vertexNumber()) {
        // will store on this vector only the indices of the referenced vertices
        std::vector<uint> refVertIndices(m.vertexContainerSize(), UINT_NULL);
        // Iterate over all vertices in the mesh, and mark any unreferenced
        // vertex as deleted.
        for (const VertexType& v : m.vertices()) {
            if (!refVertices[m.index(v)]) {
                m.deleteVertex(m.index(v));
            }
            else {
                refVertIndices[m.index(v)] = m.index(v);
            }
        }

        // update the vertex indices of the mesh, setting to null the indices of
        // the unreferenced vertices (it may happen on adjacent vertices of some
        // container).
        m.updateVertexIndices(refVertIndices);
    }

    return n;
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
uint removeDuplicatedVertices(MeshType& m)
{
    using VertexType    = MeshType::VertexType;
    using VertexPointer = MeshType::VertexType*;

    if (m.vertexNumber() == 0)
        return 0;

    // a map that will be used to keep track of deleted vertices and their
    // corresponding pointers.
    std::vector<uint> newVertexIndices(m.vertexNumber());
    // assigning each vertex index to itself.
    std::iota(newVertexIndices.begin(), newVertexIndices.end(), 0);

    uint deleted = 0;

    std::vector<VertexPointer> perm(m.vertexNumber());

    // put all the vertices into a vector for sorting.
    uint k = 0;
    for (VertexType& v : m.vertices())
        perm[k++] = &v;

    // sort the vector based on the vertices' spatial positions.
    std::sort(
        std::execution::par_unseq,
        perm.begin(),
        perm.end(),
        detail::VertPositionComparator<VertexPointer>());

    uint i = 0;

    // compare the i-th position with the next ones while they are equal to the
    // i-th.
    while (i < perm.size() - 1) {
        uint j = i + 1;
        while (j < perm.size() && perm[i]->position() == perm[j]->position()) {
            // j will be deleted, so we map its pointer to the i-th vertex's
            // pointer.
            newVertexIndices[m.index(perm[j])] = m.index(perm[i]); // map j -> i
            m.deleteVertex(m.index(perm[j]));
            j++;
            deleted++;
        }
        // here perm[i] != perm[j], so we need to check perm[j] with the next
        // vertex.
        i = j;
    }

    // update the vertex pointers to point to the correct vertices, in every
    // container of the mesh
    m.updateVertexIndices(newVertexIndices);

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
 * @note This function currently only works for triangle meshes. It should be
 * made more general to work for polygonal meshes as well.
 *
 * @note This function does not update any topology relation that could be
 * affected by the removal of duplicate faces, such as the VF or FF relation.
 * Therefore, it is usually performed before building any topology information.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * TriangleMeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove duplicate faces. This
 * mesh will be modified in place, with all duplicate faces being marked as
 * deleted.
 * @return The number of duplicated faces that were marked as deleted.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint removeDuplicatedFaces(MeshType& m)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    // create a vector of sorted tuples of indices, where each tuple represents
    // a face's vertices and a pointer to the face.
    std::vector<detail::SortedIndexContainer<
        VertexType*,
        FaceType*,
        FaceType::VERTEX_NUMBER>>
        fvec;

    for (FaceType& f : m.faces()) {
        fvec.emplace_back(&f, f.vertices());
    }

    // sort the vector based on the face vertex indices.
    std::sort(std::execution::par_unseq, fvec.begin(), fvec.end());
    uint total = 0;

    // iterate over the sorted vector, and mark any duplicate faces as deleted.
    for (uint i = 0; i < fvec.size() - 1; ++i) {
        if (fvec[i] == fvec[i + 1]) {
            total++;
            m.deleteFace(fvec[i].sentinel());
        }
    }
    return total;
}

/**
 * @brief Removes all vertices that have position with invalid floating point
 * values (NaN or inf).
 *
 * This function removes all vertices in the input mesh that have position
 * with invalid floating point values, such as NaN or inf. If the input mesh has
 * faces, and if the flag `deleteAlsoFaces` is set to true, all faces incident
 * on deleted vertices are also deleted.
 *
 * @tparam MeshType: The type of the input Mesh. It must satisfy the
 * MeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove degenerated vertices.
 * This mesh will be modified in place, with all degenerated vertices being
 * marked as deleted.
 *
 * @param[in] deleteAlsoFaces: If true, all faces incident on deleted vertices
 * will also be deleted.
 *
 * @return The number of degenerated vertices that were marked as deleted.
 *
 * @ingroup clean
 */
template<MeshConcept MeshType>
uint removeDegeneratedVertices(MeshType& m, bool deleteAlsoFaces)
{
    using VertexType = MeshType::VertexType;

    int count_vd = 0;

    // iterate over all vertices in the mesh, and mark any with invalid floating
    // point values as deleted.
    for (VertexType& v : m.vertices()) {
        if (v.position().isDegenerate()) {
            count_vd++;
            m.deleteVertex(&v);
        }
    }

    // If the mesh has faces and the `deleteAlsoFaces` flag is true, delete all
    // faces incident on deleted vertices.
    if constexpr (HasFaces<MeshType>) {
        using FaceType = MeshType::FaceType;
        if (deleteAlsoFaces) {
            for (FaceType& f : m.faces()) {
                bool deg = false;
                for (VertexType* v : f.vertices()) {
                    if (v->deleted()) {
                        deg = true;
                    }
                }
                if (deg) {
                    m.deleteFace(&f);
                }
            }
        }
    }

    return count_vd;
}

/**
 * @brief Removes all degenerate faces from the input mesh.
 *
 * This function removes all faces in the input mesh that are topologically
 * degenerate, meaning that they have two or more vertex references that link
 * the same vertex. All degenerate faces are zero area faces, but not all zero
 * area faces are degenerate (for example, a face with three different vertex
 * references, but two of them have the same position). Therefore, if you
 * also want to remove these kinds of faces, you should call
 * `removeDuplicatedVertices(m)` first. This function does not adjust topology.
 *
 * @tparam MeshType The type of the input Mesh. It must satisfy the
 * FaceMeshConcept.
 *
 * @param[in,out] m: The input mesh for which to remove degenerate faces. This
 * mesh will be modified in place, with all degenerate faces being marked as
 * deleted.
 *
 * @return The number of degenerate faces that were marked as deleted.
 *
 * @ingroup clean
 */
template<FaceMeshConcept MeshType>
uint removeDegenerateFaces(MeshType& m)
{
    uint count     = 0;
    using FaceType = MeshType::FaceType;

    // iterate over all faces in the mesh, and mark any that are degenerate as
    // deleted.
    for (FaceType& f : m.faces()) {
        bool deg = false; // flag to check if a face is degenerate
        for (uint i = 0; i < f.vertexNumber() && !deg; ++i) {
            if (f.vertex(i) == f.vertexMod(i + 1)) {
                deg = true;
                m.deleteFace(m.index(f));
                count++;
            }
        }
    }
    return count;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CLEAN_H
