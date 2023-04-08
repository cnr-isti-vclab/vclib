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

#ifndef VCL_MESH_VIEWS_COMPONENTS_SELECTION_H
#define VCL_MESH_VIEWS_COMPONENTS_SELECTION_H

#include <vclib/types.h>

namespace vcl::views {

namespace internal {

inline constexpr auto isSelected = [](const auto& e)
{
	if constexpr(vcl::IsPointer<decltype(e)>) {
		return e->isSelected();
	}
	else {
		return e.isSelected();
	}
};

inline constexpr auto isNotSelected = [](const auto& e)
{
	if constexpr(vcl::IsPointer<decltype(e)>) {
		return !e->isSelected();
	}
	else {
		return !e.isSelected();
	}
};

struct SelectionViewClosure
{
	constexpr SelectionViewClosure(){}

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return r | std::views::transform(isSelected);
	}
};

struct SelectedViewClosure
{
	constexpr SelectedViewClosure(){}

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return r | std::views::filter(isSelected);
	}
};

struct NotSelectedViewClosure
{
	constexpr NotSelectedViewClosure(){}

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return r | std::views::filter(isNotSelected);
	}
};

} // namespace vcl::views::internal

inline constexpr internal::SelectionViewClosure selection;
inline constexpr internal::SelectedViewClosure selected;
inline constexpr internal::NotSelectedViewClosure notSelected;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_SELECTION_H
