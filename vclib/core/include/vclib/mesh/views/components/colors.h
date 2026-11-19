// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_COLORS_H
#define VCL_MESH_VIEWS_COMPONENTS_COLORS_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasColorMember = requires(T&& t) {
    { t.color() };
};

template<typename R>
concept ColorRangeConcept =
    std::ranges::range<R> &&
    (requires(R&& r) {
        { std::forward<R>(r).colors() } -> std::ranges::range;
    } || HasColorMember<std::ranges::range_reference_t<R>>);

namespace views {

namespace detail {

inline constexpr auto getColor = [](auto&& p) -> decltype(auto) {
    return p.color();
};

struct ColorsView : std::ranges::range_adaptor_closure<ColorsView>
{
    template<ColorRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        if constexpr (requires {
                          {
                              std::forward<R>(r).colors()
                          } -> std::ranges::range;
                      }) {
            return std::forward<R>(r).colors();
        }
        else {
            return std::forward<R>(r) | std::views::transform(getColor);
        }
    }
};

} // namespace detail

/**
 * @brief View adaptor that transforms a range of elements into a range of their colors.
 *
 * When applied directly to an ElementContainerProxy (e.g. `mesh.vertices() | views::colors` or
 * `mesh.faces() | views::colors`), it directly returns a contiguous ComponentView over the SoA buffer
 * without proxy overhead.
 *
 * When applied to a filtered or transformed range (e.g. `mesh.vertices() | views::selected | views::colors`),
 * it transforms each element to its color reference.
 *
 * @ingroup views
 */
inline constexpr detail::ColorsView colors;

template<>
inline constexpr auto component<CompId::COLOR> = colors;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_COLORS_H
