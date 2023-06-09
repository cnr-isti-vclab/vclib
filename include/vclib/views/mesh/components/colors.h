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

#ifndef VCL_VIEWS_MESH_COMPONENTS_COLORS_H
#define VCL_VIEWS_MESH_COMPONENTS_COLORS_H

#include <vclib/concepts/mesh.h>
#include <vclib/concepts/pointers.h>
#include <vclib/types.h>

#include <ranges>

namespace vcl::views {

namespace internal {

template<typename T>
concept CleanWedgeColorsConcept = comp::HasWedgeColors<std::remove_cvref_t<T>>;

struct ColorsView
{
	constexpr ColorsView() = default;

	inline static constexpr auto constColor = [](const auto& p) -> decltype(auto)
	{
		if constexpr(IsPointer<decltype(p)>)
			return p->color();
		else
			return p.color();
	};

	inline static constexpr auto color = [](auto& p) -> decltype(auto)
	{
		if constexpr(IsPointer<decltype(p)>)
			return p->color();
		else
			return p.color();
	};

#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, ColorsView)
	{
		using ElemType = std::ranges::range_value_t<R>;
		if constexpr(IsConst<ElemType>)
			return std::forward<R>(r) | std::views::transform(constColor);
		else
			return std::forward<R>(r) | std::views::transform(color);

	}
#endif

	template <CleanWedgeColorsConcept R>
	friend constexpr auto operator|(R&& r, ColorsView)
	{
		if constexpr(IsPointer<R>)
			return r->wedgeColors();
		else
			return r.wedgeColors();
	}
};

} // namespace vcl::views::internal

inline constexpr internal::ColorsView colors;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_COMPONENTS_COLORS_H
