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

#ifndef VCL_MESH_VIEWS_COMPONENTS_SCALARS_H
#define VCL_MESH_VIEWS_COMPONENTS_SCALARS_H

#include <vclib/types.h>

namespace vcl::views {

namespace internal {

struct ScalarViewClosure
{
	constexpr ScalarViewClosure(){}

	inline static constexpr auto constScalar = [](const auto& p) -> decltype(auto)
	{
		if constexpr(IsPointer<decltype(p)>)
			return p->scalar();
		else
			return p.scalar();
	};

	inline static constexpr auto scalar = [](auto& p) -> decltype(auto)
	{
		if constexpr(IsPointer<decltype(p)>)
			return p->scalar();
		else
			return p.scalar();
	};

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		using ElemType = std::ranges::range_value_t<R>;
		if constexpr(IsConst<ElemType>)
			return r | std::views::transform(constScalar);
		else
			return r | std::views::transform(scalar);
	}
};

} // namespace vcl::views::internal

inline constexpr internal::ScalarViewClosure scalars;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_SCALARS_H
