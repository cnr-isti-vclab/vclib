// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasAdjEdgesMember = requires(T&& t) {
    { t.adjEdges() } -> std::ranges::range;
};

template<typename R>
concept AdjEdgesRangeConcept =
    std::ranges::range<R> &&
    HasAdjEdgesMember<std::ranges::range_reference_t<R>>;

namespace views {

namespace detail {

inline constexpr auto getAdjEdges = [](auto&& p) -> decltype(auto) {
    return p.adjEdges();
};

struct AdjEdgesView : std::ranges::range_adaptor_closure<AdjEdgesView>
{
    template<typename R>
    requires (!std::ranges::range<R>) && HasAdjEdgesMember<R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r).adjEdges();
    }

    template<typename R>
    requires (!std::ranges::range<R>) && HasAdjEdgesMember<R>
    friend constexpr auto operator|(R&& r, AdjEdgesView)
    {
        return std::forward<R>(r).adjEdges();
    }

    template<AdjEdgesRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r) | std::views::transform(getAdjEdges);
    }
};

} // namespace detail

/**
 * @brief View adaptor that gives access to the adjacent edges of the
 * element that has been piped. Every element having the adjacency
 * component (i.e., having the `adjEdges()` member function) can be applied to
 * this view.
 *
 * The resulting range contains proxies to the adjacent edges. Proxies of
 * missing adjacencies are invalid (they evaluate to `false`). If you are
 * interested only on the valid ones, you can use the `notNull` view:
 *
 * @code{.cpp}
 * for (auto ae : e | views::adjEdges | views::notNull) { ... }
 * @endcode
 *
 * When applied to a range of elements (e.g. `mesh.edges() | views::adjEdges`), it
 * transforms each element to the range of its adjacent edges.
 *
 * @ingroup views
 */
inline constexpr detail::AdjEdgesView adjEdges;

template<>
inline constexpr auto component<CompId::ADJACENT_EDGES> = adjEdges;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_ADJ_EDGES_H
