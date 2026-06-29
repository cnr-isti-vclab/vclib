// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_NORMALS_H
#define VCL_MESH_VIEWS_COMPONENTS_NORMALS_H

#include <vclib/mesh/components/normal.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto normal = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->normal();
    else
        return p.normal();
};

struct NormalsView
{
    constexpr NormalsView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, NormalsView)
    {
        return std::forward<R>(r) | std::views::transform(normal);
    }
};

} // namespace detail

inline constexpr detail::NormalsView normals;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_NORMALS_H
