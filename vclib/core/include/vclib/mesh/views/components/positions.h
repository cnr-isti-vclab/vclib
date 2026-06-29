// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_POSITIONS_H
#define VCL_MESH_VIEWS_COMPONENTS_POSITIONS_H

#include <vclib/mesh/components/position.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto position = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->position();
    else
        return p.position();
};

struct PositionsView
{
    constexpr PositionsView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, PositionsView)
    {
        return std::forward<R>(r) | std::views::transform(position);
    }
};

} // namespace detail

inline constexpr detail::PositionsView positions;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_POSITIONS_H
