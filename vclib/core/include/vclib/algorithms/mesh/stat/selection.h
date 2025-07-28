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

#ifndef VCL_ALGORITHMS_MESH_STAT_SELECTION_H
#define VCL_ALGORITHMS_MESH_STAT_SELECTION_H

#include <vclib/mesh.h>

namespace vcl {

/**
 * @brief Return the number of elements identified by `ELEM_ID` of the mesh that
 * are marked as selected.
 *
 * @tparam ELEM_ID: The element ID to be checked (e.g., vertex, edge, face).
 *
 * @param[in] m: The input mesh.
 * @return The number of selected elements.
 */
template<uint ELEM_ID, MeshConcept MeshType>
uint elementSelectionNumber(const MeshType& m)
{
    return std::ranges::distance(
        m.template elements<ELEM_ID>() | views::selected);
}

/**
 * @brief Return the number of vertices of the mesh that are marked as selected.
 * @param[in] m: The input mesh.
 * @return The number of selected vertices.
 */
template<MeshConcept MeshType>
uint vertexSelectionNumber(const MeshType& m)
{
    return elementSelectionNumber<ElemId::VERTEX>(m);
}

/**
 * @brief Return the number of faces of the mesh that are marked as selected.
 * @param[in] m: The input mesh.
 * @return The number of selected faces.
 */
template<FaceMeshConcept MeshType>
uint faceSelectionNumber(const MeshType& m)
{
    return elementSelectionNumber<ElemId::FACE>(m);
}

/**
 * @brief Return the number of face edges of the mesh that are marked as
 * selected.
 *
 * With 'face edges' it is intended the edges that are part of the faces of the
 * mesh (e.g. if a face has 4 vertices, it has 4 edges).
 *
 * @param[in] m: The input mesh.
 * @return The number of selected edges.
 */
template<FaceMeshConcept MeshType>
uint faceEdgesSelectionNumber(const MeshType& m)
{
    uint cnt = 0;
    for (const auto& f : m.faces()) {
        for (uint i = 0; i < f.vertexNumber(); ++i) {
            if (f.edgeSelected(i)) {
                ++cnt;
            }
        }
    }
    return cnt;
}

/**
 * @brief Return the number of edges of the mesh that are marked as selected.
 *
 * @param[in] m: The input mesh.
 * @return The number of selected edges.
 */
template<EdgeMeshConcept MeshType>
uint edgeSelectionNumber(const MeshType& m)
{
    return elementSelectionNumber<ElemId::EDGE>(m);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_SELECTION_H
