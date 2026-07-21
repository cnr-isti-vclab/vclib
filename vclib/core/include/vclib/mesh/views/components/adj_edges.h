// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H

#include <vclib/mesh/components/adjacent_edges.h>

#include <vclib/base.h>

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

#endif // VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H
