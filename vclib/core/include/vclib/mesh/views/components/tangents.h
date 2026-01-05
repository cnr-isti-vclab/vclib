/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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
