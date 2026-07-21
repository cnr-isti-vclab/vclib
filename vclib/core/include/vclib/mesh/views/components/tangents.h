// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_TANGENTS_H
#define VCL_MESH_VIEWS_COMPONENTS_TANGENTS_H

#include <vclib/mesh/components/tangent.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto tangent = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->tangent();
    else
        return p.tangent();
};

struct TangentView
{
    constexpr TangentView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, TangentView)
    {
        using ElemType = std::ranges::range_value_t<R>;
        return std::forward<R>(r) | std::views::transform(tangent);
    }
};

} // namespace detail

inline constexpr detail::TangentView tangents;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_TANGENTS_H
