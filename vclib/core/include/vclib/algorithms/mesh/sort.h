/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_ALGORITHMS_MESH_SORT_H
#define VCL_ALGORITHMS_MESH_SORT_H

#include "utility.h"

#include <vclib/space/complex.h>

#include <algorithm>

namespace vcl {

template<FaceMeshConcept MeshType>
std::vector<MeshEdgeUtil<MeshType>> fillAndSortMeshEdgeUtilVector(
    MeshType& m,
    bool      includeFauxEdges = true)
{
    using FaceType = MeshType::FaceType;

    std::vector<MeshEdgeUtil<MeshType>> vec;

    int n_edges = 0;
    for (const FaceType& f : m.faces())
        n_edges += f.vertexNumber();

    vec.reserve(n_edges);

    for (FaceType& f : m.faces()) { // fill it with face data
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (includeFauxEdges || !f.edgeFaux(j)) {
                vec.emplace_back(f, j);
            }
        }
    }

    // Sort it by vertices
    std::sort(std::execution::par_unseq, vec.begin(), vec.end());

    return vec;
}

template<FaceMeshConcept MeshType>
std::vector<ConstMeshEdgeUtil<MeshType>> fillAndSortMeshEdgeUtilVector(
    const MeshType& m,
    bool            includeFauxEdges = true)
{
    using FaceType = MeshType::FaceType;

    std::vector<ConstMeshEdgeUtil<MeshType>> vec;

    int n_edges = 0;
    for (const FaceType& f : m.faces())
        n_edges += f.vertexNumber();

    vec.reserve(n_edges);

    for (const FaceType& f : m.faces()) { // Lo riempio con i dati delle facce
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (includeFauxEdges || !f.edgeFaux(j)) {
                vec.emplace_back(f, j);
            }
        }
    }

    // Lo ordino per vertici
    std::sort(std::execution::par_unseq, vec.begin(), vec.end());

    return vec;
}

/**
 * @brief Sorts the indices of mesh elements by a custom comparison function.
 *
 * This function creates a vector of indices representing the elements of the
 * specified type (vertices, faces, edges, etc.) in the mesh, sorted according
 * to the provided comparison function. The indices can optionally be remapped
 * to represent their positions in a compacted container.
 *
 * @tparam ELEM_ID: The ID of the element type to sort (e.g., vertex, face).
 * @tparam MeshType: The type of the mesh, must satisfy MeshConcept.
 *
 * @param[in] mesh: The input mesh containing the elements to sort.
 * @param[in] func: A comparison function that takes two const references to
 * elements of type ELEM_ID and returns true if the first element should come
 * before the second in the sorted order.
 * @param[in] getIndicesAsIfContainerCompact: If true, the returned indices are
 * remapped to represent positions as if the container were compacted (i.e.,
 * with deleted elements removed). Default is false.
 *
 * @return A vector of sorted indices. Each index corresponds to an element in
 * the mesh, ordered according to the comparison function. If
 * getIndicesAsIfContainerCompact is true, the indices are remapped.
 */
template<uint ELEM_ID, MeshConcept MeshType>
std::vector<uint> sortElemIndicesByFunction(
    const MeshType&                                                mesh,
    const std::function<bool(
        const typename MeshType::template ElementType<ELEM_ID>&,
        const typename MeshType::template ElementType<ELEM_ID>&)>& func,
    bool getIndicesAsIfContainerCompact = false)
{
    using ElemType = typename MeshType::template ElementType<ELEM_ID>;

    auto compactIndices = detail::elemCompactIndices<ELEM_ID>(
        mesh, getIndicesAsIfContainerCompact);

    std::vector<uint> indices;

    // Initialize indices with sequential values
    indices.resize(mesh.template number<ELEM_ID>());
    std::iota(indices.begin(), indices.end(), 0u);

    std::sort(indices.begin(), indices.end(), [&](uint a, uint b) {
        return func(
            mesh.template element<ELEM_ID>(a),
            mesh.template element<ELEM_ID>(b));
    });

    if (!compactIndices.empty()) {
        for (auto& idx : indices) {
            idx = compactIndices[idx];
        }
    }

    return indices;
}

/**
 * @brief Sorts the indices of mesh vertices by a custom comparison function.
 *
 * This function creates a vector of indices representing the vertices in the
 * mesh, sorted according to the provided comparison function. The indices can
 * optionally be remapped to represent their positions in a compacted container.
 *
 * @tparam MeshType: The type of the mesh, must satisfy MeshConcept.
 *
 * @param[in] mesh: The input mesh containing the vertices to sort.
 * @param[in] func: A comparison function that takes two const references to
 * vertices and returns true if the first vertex should come before the second
 * in the sorted order.
 * @param[in] getIndicesAsIfContainerCompact: If true, the returned indices are
 * remapped to represent positions as if the container were compacted (i.e.,
 * with deleted vertices removed). Default is false.
 *
 * @return A vector of sorted indices. Each index corresponds to a vertex in
 * the mesh, ordered according to the comparison function. If
 * getIndicesAsIfContainerCompact is true, the indices are remapped.
 */
template<MeshConcept MeshType>
std::vector<uint> sortVertexIndicesByFunction(
    const MeshType&                             mesh,
    const std::function<bool(
        const typename MeshType::VertexType&,
        const typename MeshType::VertexType&)>& func,
    bool getIndicesAsIfContainerCompact = false)
{
    return sortElemIndicesByFunction<ElemId::VERTEX>(
        mesh, func, getIndicesAsIfContainerCompact);
}

/**
 * @brief Sorts the indices of mesh faces by a custom comparison function.
 *
 * This function creates a vector of indices representing the faces in the
 * mesh, sorted according to the provided comparison function. The indices can
 * optionally be remapped to represent their positions in a compacted container.
 *
 * @tparam MeshType: The type of the mesh, must satisfy FaceMeshConcept.
 *
 * @param[in] mesh: The input mesh containing the faces to sort.
 * @param[in] func: A comparison function that takes two const references to
 * faces and returns true if the first face should come before the second
 * in the sorted order.
 * @param[in] getIndicesAsIfContainerCompact: If true, the returned indices are
 * remapped to represent positions as if the container were compacted (i.e.,
 * with deleted faces removed). Default is false.
 *
 * @return A vector of sorted indices. Each index corresponds to a face in
 * the mesh, ordered according to the comparison function. If
 * getIndicesAsIfContainerCompact is true, the indices are remapped.
 */
template<FaceMeshConcept MeshType>
std::vector<uint> sortFaceIndicesByFunction(
    const MeshType&                           mesh,
    const std::function<bool(
        const typename MeshType::FaceType&,
        const typename MeshType::FaceType&)>& func,
    bool getIndicesAsIfContainerCompact = false)
{
    return sortElemIndicesByFunction<ElemId::FACE>(
        mesh, func, getIndicesAsIfContainerCompact);
}

/**
 * @brief Sorts the indices of mesh edges by a custom comparison function.
 *
 * This function creates a vector of indices representing the edges in the
 * mesh, sorted according to the provided comparison function. The indices can
 * optionally be remapped to represent their positions in a compacted container.
 *
 * @tparam MeshType: The type of the mesh, must satisfy EdgeMeshConcept.
 *
 * @param[in] mesh: The input mesh containing the edges to sort.
 * @param[in] func: A comparison function that takes two const references to
 * edges and returns true if the first edge should come before the second
 * in the sorted order.
 * @param[in] getIndicesAsIfContainerCompact: If true, the returned indices are
 * remapped to represent positions as if the container were compacted (i.e.,
 * with deleted edges removed). Default is false.
 *
 * @return A vector of sorted indices. Each index corresponds to a edge in
 * the mesh, ordered according to the comparison function. If
 * getIndicesAsIfContainerCompact is true, the indices are remapped.
 */
template<EdgeMeshConcept MeshType>
std::vector<uint> sortEdgeIndicesByFunction(
    const MeshType&                           mesh,
    const std::function<bool(
        const typename MeshType::EdgeType&,
        const typename MeshType::EdgeType&)>& func,
    bool getIndicesAsIfContainerCompact = false)
{
    return sortElemIndicesByFunction<ElemId::EDGE>(
        mesh, func, getIndicesAsIfContainerCompact);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_SORT_H
