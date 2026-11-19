// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_POSITIONS_H
#define VCL_MESH_VIEWS_COMPONENTS_POSITIONS_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasPositionMember = requires(T&& t) {
    { t.position() };
};

template<typename R>
concept PositionRangeConcept =
    std::ranges::range<R> &&
    (requires(R&& r) {
        { std::forward<R>(r).positions() } -> std::ranges::range;
    } || HasPositionMember<std::ranges::range_reference_t<R>>);

namespace views {

namespace detail {

inline constexpr auto getPosition = [](auto&& p) -> decltype(auto) {
    return p.position();
};

struct PositionsView : std::ranges::range_adaptor_closure<PositionsView>
{
    template<PositionRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        if constexpr (requires {
                          {
                              std::forward<R>(r).positions()
                          } -> std::ranges::range;
                      }) {
            return std::forward<R>(r).positions();
        }
        else {
            return std::forward<R>(r) | std::views::transform(getPosition);
        }
    }
};

} // namespace detail

/**
 * @brief View adaptor that transforms a range of elements into a range of their positions.
 *
 * When applied directly to an ElementContainerProxy (e.g. `mesh.vertices() | views::positions`),
 * it directly returns a contiguous ComponentView over the SoA buffer without proxy overhead.
 *
 * When applied to a filtered or transformed range (e.g. `mesh.vertices() | views::selected | views::positions`),
 * it transforms each element to its position reference.
 *
 * @ingroup views
 */
inline constexpr detail::PositionsView positions;

template<>
inline constexpr auto component<CompId::POSITION> = positions;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_POSITIONS_H
