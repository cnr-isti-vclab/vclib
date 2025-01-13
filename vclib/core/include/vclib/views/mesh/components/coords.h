/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_VIEWS_MESH_COMPONENTS_COORDS_H
#define VCL_VIEWS_MESH_COMPONENTS_COORDS_H

#include <vclib/concepts/pointers.h>
#include <vclib/types.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto coord = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->coord();
    else
        return p.coord();
};

struct CoordsView
{
    constexpr CoordsView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, CoordsView)
    {
        return std::forward<R>(r) | std::views::transform(coord);
    }
};

} // namespace detail

inline constexpr detail::CoordsView coords;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_COMPONENTS_COORDS_H
