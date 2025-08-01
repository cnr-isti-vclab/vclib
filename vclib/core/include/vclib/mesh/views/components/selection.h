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

#ifndef VCL_MESH_VIEWS_COMPONENTS_SELECTION_H
#define VCL_MESH_VIEWS_COMPONENTS_SELECTION_H

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto isSelected = [](auto&& e) -> decltype(auto) {
    if constexpr (vcl::IsPointer<decltype(e)>) {
        return e->selected();
    }
    else {
        return e.selected();
    }
};

inline constexpr auto isNotSelected = [](auto&& e) -> decltype(auto) {
    if constexpr (vcl::IsPointer<decltype(e)>) {
        return !e->selected();
    }
    else {
        return !e.selected();
    }
};

struct SelectionView
{
    constexpr SelectionView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, SelectionView)
    {
        return std::forward<R>(r) | std::views::transform(isSelected);
    }
};

struct SelectedView
{
    constexpr SelectedView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, SelectedView)
    {
        return std::forward<R>(r) | std::views::filter(isSelected);
    }
};

struct NotSelectedView
{
    constexpr NotSelectedView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, NotSelectedView)
    {
        return std::forward<R>(r) | std::views::filter(isNotSelected);
    }
};

} // namespace detail

inline constexpr detail::SelectionView   selection;
inline constexpr detail::SelectedView    selected;
inline constexpr detail::NotSelectedView notSelected;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_SELECTION_H
