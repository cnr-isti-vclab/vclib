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

#ifndef VCL_VIEWS_MESH_COMPONENTS_PRINCIPAL_CURVATURES_H
#define VCL_VIEWS_MESH_COMPONENTS_PRINCIPAL_CURVATURES_H

#include <vclib/concepts/pointers.h>
#include <vclib/types.h>

#include <ranges>

namespace vcl::views {

namespace internal {

struct PrincipalCurvaturesView
{
	constexpr PrincipalCurvaturesView() = default;

	inline static constexpr auto constPrincipalCurvature = [](const auto& p) -> decltype(auto)
	{
		if constexpr(IsPointer<decltype(p)>)
			return p->principalCurvature();
		else
			return p.principalCurvature();
	};

	inline static constexpr auto principalCurvature = [](auto& p) -> decltype(auto)
	{
		if constexpr(IsPointer<decltype(p)>)
			return p->principalCurvature();
		else
			return p.principalCurvature();
	};
	
#ifdef VCLIB_USES_RANGES
	template <std::ranges::range R>
	friend constexpr auto operator|(R&& r, PrincipalCurvaturesView)
	{
		using ElemType = std::ranges::range_value_t<R>;
		if constexpr(IsConst<ElemType>)
			return std::forward<R>(r) | std::views::transform(constPrincipalCurvature);
		else
			return std::forward<R>(r) | std::views::transform(principalCurvature);
	}
#endif
};

} // namespace vcl::views::internal

inline constexpr internal::PrincipalCurvaturesView principalCurvatures;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_COMPONENTS_PRINCIPAL_CURVATURES_H
