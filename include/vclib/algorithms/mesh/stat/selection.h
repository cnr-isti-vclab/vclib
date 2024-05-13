/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/mesh/requirements.h>
#include <vclib/views/mesh.h>

namespace vcl {

namespace detail {

template<uint ELEM_ID, MeshConcept MeshType>
uint elementSelectionNumber(const MeshType& m)
{
    return std::ranges::distance(
        m.template elements<ELEM_ID>() | views::selected);
}

} // namespace detail

template<MeshConcept MeshType>
uint vertexSelectionNumber(const MeshType& m)
{
    return detail::elementSelectionNumber<ElemId::VERTEX>(m);
}

template<FaceMeshConcept MeshType>
uint faceSelectionNumber(const MeshType& m)
{
    return detail::elementSelectionNumber<ElemId::FACE>(m);
}

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

template<EdgeMeshConcept MeshType>
uint edgeSelectionNumber(const MeshType& m)
{
    return detail::elementSelectionNumber<ElemId::EDGE>(m);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_SELECTION_H
