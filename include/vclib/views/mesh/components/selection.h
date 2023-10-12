/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_VIEWS_MESH_COMPONENTS_SELECTION_H
#define VCL_VIEWS_MESH_COMPONENTS_SELECTION_H

#include <vclib/concepts/pointers.h>
#include <vclib/types.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto isSelected = [](const auto& e)
{
    if constexpr(vcl::IsPointer<decltype(e)>) {
        return e->selected();
    }
    else {
        return e.selected();
    }
};

inline constexpr auto isNotSelected = [](const auto& e)
{
    if constexpr(vcl::IsPointer<decltype(e)>) {
        return !e->selected();
    }
    else {
        return !e.selected();
    }
};

struct SelectionView
{
    constexpr SelectionView() = default;

    template <std::ranges::range R>
    friend constexpr auto operator|(R&& r, SelectionView)
    {
        return std::forward<R>(r) | std::views::transform(isSelected);
    }
};

struct SelectedView
{
    constexpr SelectedView() = default;

    template <std::ranges::range R>
    friend constexpr auto operator|(R&& r, SelectedView)
    {
        return std::forward<R>(r) | std::views::filter(isSelected);
    }
};

struct NotSelectedView
{
    constexpr NotSelectedView() = default;

    template <std::ranges::range R>
    friend constexpr auto operator|(R&& r, NotSelectedView)
    {
        return std::forward<R>(r) | std::views::filter(isNotSelected);
    }
};

} // namespace vcl::views::detail

inline constexpr detail::SelectionView selection;
inline constexpr detail::SelectedView selected;
inline constexpr detail::NotSelectedView notSelected;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_COMPONENTS_SELECTION_H
