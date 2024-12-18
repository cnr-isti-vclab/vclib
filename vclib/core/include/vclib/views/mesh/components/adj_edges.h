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

#ifndef VCL_VIEWS_MESH_COMPONENTS_ADJ_EDGES_H
#define VCL_VIEWS_MESH_COMPONENTS_ADJ_EDGES_H

#include <vclib/concepts.h>
#include <vclib/types.h>

namespace vcl::views {

namespace detail {

struct AdjEdgesView
{
    constexpr AdjEdgesView() = default;

    template<comp::HasAdjacentEdges R>
    friend constexpr auto operator|(R&& r, AdjEdgesView)
    {
        if constexpr (IsPointer<R>)
            return r->adjEdges();
        else
            return r.adjEdges();
    }
};

} // namespace detail

/**
 * @brief The adjEdges view allows to obtain a view that access to the adjacent
 * edges of the object that has been piped. Every object having type that
 * satisfies the HasAdjacentEdges concept can be applied to this view.
 *
 * Resulting adjacent edges will be pointers to Edges, that may be `nullptr`.
 * If you are interested only on the not-null pointers, you can use the
 * `notNull` view:
 *
 * @code{.cpp}
 * for (auto* ae: f | views::adjEdges | views::notNull) { ... }
 * @endcode
 *
 * @ingroup views
 */
inline constexpr detail::AdjEdgesView adjEdges;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_COMPONENTS_ADJ_EDGES_H
