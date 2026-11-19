// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_VERTICES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_VERTICES_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasAdjVerticesMember = requires(T&& t) {
    { t.adjVertices() } -> std::ranges::range;
};

template<typename R>
concept AdjVerticesRangeConcept =
    std::ranges::range<R> &&
    HasAdjVerticesMember<std::ranges::range_reference_t<R>>;

namespace views {

namespace detail {

inline constexpr auto getAdjVertices = [](auto&& p) -> decltype(auto) {
    return p.adjVertices();
};

struct AdjVerticesView : std::ranges::range_adaptor_closure<AdjVerticesView>
{
    template<typename R>
    requires (!std::ranges::range<R>) && HasAdjVerticesMember<R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r).adjVertices();
    }

    template<typename R>
    requires (!std::ranges::range<R>) && HasAdjVerticesMember<R>
    friend constexpr auto operator|(R&& r, AdjVerticesView)
    {
        return std::forward<R>(r).adjVertices();
    }

    template<AdjVerticesRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r) | std::views::transform(getAdjVertices);
    }
};

} // namespace detail

/**
 * @brief View adaptor that gives access to the adjacent vertices of the
 * element that has been piped. Every element having the adjacency
 * component (i.e., having the `adjVertices()` member function) can be applied to
 * this view.
 *
 * The resulting range contains proxies to the adjacent vertices. Proxies of
 * missing adjacencies are invalid (they evaluate to `false`). If you are
 * interested only on the valid ones, you can use the `notNull` view:
 *
 * @code{.cpp}
 * for (auto av : e | views::adjVertices | views::notNull) { ... }
 * @endcode
 *
 * When applied to a range of elements (e.g. `mesh.vertices() | views::adjVertices`), it
 * transforms each element to the range of its adjacent vertices.
 *
 * @ingroup views
 */
inline constexpr detail::AdjVerticesView adjVertices;

template<>
inline constexpr auto component<CompId::ADJACENT_VERTICES> = adjVertices;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_ADJ_VERTICES_H
