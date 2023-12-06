/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_VIEWS_MESH_ELEMENTS_VERTEX_H
#define VCL_VIEWS_MESH_ELEMENTS_VERTEX_H

#include <vclib/concepts/mesh.h>

namespace vcl::views {
namespace detail {

template<typename T>
concept CleanMeshConcept = MeshConcept<std::remove_cvref_t<T>>;

template<typename T>
concept CleanVertexPtrsConcept =
    comp::HasVertexPointers<std::remove_cvref_t<T>>;

struct VerticesView
{
    constexpr VerticesView() = default;

    template<CleanMeshConcept R>
    friend constexpr auto operator|(R&& r, VerticesView)
    {
        return r.vertices();
    }

    template<CleanVertexPtrsConcept R>
    friend constexpr auto operator|(R&& r, VerticesView)
    {
        return r.vertices();
    }
};

} // namespace detail

/**
 * @brief A view that allows to iterate over the Vertex elements of an object.
 *
 * This view can be applied to objects having type that satisfies one of the
 * following concepts:
 * - MeshConcept
 * - HasVertexPointers
 *
 * @ingroup views
 */
inline constexpr detail::VerticesView vertices;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_ELEMENTS_VERTEX_H
