// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_ADJ_FACES_H
#define VCL_MESH_VIEWS_COMPONENTS_ADJ_FACES_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasAdjFacesMember = requires(T&& t) {
    { t.adjFaces() } -> std::ranges::range;
};

template<typename R>
concept AdjFacesRangeConcept =
    std::ranges::range<R> &&
    HasAdjFacesMember<std::ranges::range_reference_t<R>>;

namespace views {

namespace detail {

inline constexpr auto getAdjFaces = [](auto&& p) -> decltype(auto) {
    return p.adjFaces();
};

struct AdjFacesView : std::ranges::range_adaptor_closure<AdjFacesView>
{
    template<typename R>
    requires (!std::ranges::range<R>) && HasAdjFacesMember<R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r).adjFaces();
    }

    template<typename R>
    requires (!std::ranges::range<R>) && HasAdjFacesMember<R>
    friend constexpr auto operator|(R&& r, AdjFacesView)
    {
        return std::forward<R>(r).adjFaces();
    }

    template<AdjFacesRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r) | std::views::transform(getAdjFaces);
    }
};

} // namespace detail

/**
 * @brief View adaptor that gives access to the adjacent faces of the
 * element that has been piped. Every element having the adjacency
 * component (i.e., having the `adjFaces()` member function) can be applied to
 * this view.
 *
 * The resulting range contains proxies to the adjacent faces. Proxies of
 * missing adjacencies are invalid (they evaluate to `false`). If you are
 * interested only on the valid ones, you can use the `notNull` view:
 *
 * @code{.cpp}
 * for (auto af : e | views::adjFaces | views::notNull) { ... }
 * @endcode
 *
 * When applied to a range of elements (e.g. `mesh.faces() | views::adjFaces`), it
 * transforms each element to the range of its adjacent faces.
 *
 * @ingroup views
 */
inline constexpr detail::AdjFacesView adjFaces;

template<>
inline constexpr auto component<CompId::ADJACENT_FACES> = adjFaces;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_ADJ_FACES_H
