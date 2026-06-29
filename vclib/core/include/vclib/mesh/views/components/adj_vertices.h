// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_VERTICES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_VERTICES_H

#include <vclib/mesh/components/adjacent_vertices.h>

#include <vclib/base.h>

namespace vcl::views {

namespace detail {

struct AdjVerticesView
{
    constexpr AdjVerticesView() = default;

    template<comp::HasAdjacentVertices R>
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
