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

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_VERTICES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_VERTICES_H

#include <vclib/concepts/mesh.h>
#include <vclib/types.h>

namespace vcl::views {

namespace detail {

template<typename T>
concept CleanAdjVerticesConcept =
    comp::HasAdjacentVertices<std::remove_cvref_t<T>>;

struct AdjVerticesView
{
    constexpr AdjVerticesView() = default;

    template<CleanAdjVerticesConcept R>
    friend constexpr auto operator|(R&& r, AdjVerticesView)
    {
        if constexpr (IsPointer<R>)
            return r->adjVertices();
        else
            return r.adjVertices();
    }
};

} // namespace detail

/**
 * @brief The adjVertices view allows to obtain a view that access to the
 * adjacent vertices of the object that has been piped. Every object having type
 * that satisfies the HasAdjacentVertices concept can be applied to this view.
 *
 * Resulting adjacent faces will be pointers to Vertices, that may be `nullptr`.
 * If you are interested only on the not-null pointers, you can use the
 * `notNull` view:
 *
 * @code{.cpp}
 * for (auto* av: v | views::adjVertices | views::notNull) { ... }
 * @endcode
 *
 * @ingroup views
 */
inline constexpr detail::AdjVerticesView adjVertices;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_ADJ_VERTICES_H
