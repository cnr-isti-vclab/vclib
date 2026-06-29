// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_COLORS_H
#define VCL_MESH_VIEWS_COMPONENTS_COLORS_H

#include <vclib/mesh/components/color.h>
#include <vclib/mesh/components/wedge_colors.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto color = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->color();
    else
        return p.color();
};

struct ColorsView
{
    constexpr ColorsView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, ColorsView)
    {
        return std::forward<R>(r) | std::views::transform(color);
    }

    template<comp::HasWedgeColors R>
    friend constexpr auto operator|(R&& r, ColorsView)
    {
        if constexpr (IsPointer<R>)
            return r->wedgeColors();
        else
            return r.wedgeColors();
    }
};

} // namespace detail

inline constexpr detail::ColorsView colors;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_COLORS_H
