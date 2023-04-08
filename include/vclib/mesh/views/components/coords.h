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

#ifndef VCL_MESH_VIEWS_COMPONENTS_COORDS_H
#define VCL_MESH_VIEWS_COMPONENTS_COORDS_H

#include <vclib/types.h>

namespace vcl::views {

namespace internal {

struct CoordsViewClosure
{
	constexpr CoordsViewClosure(){}

	inline static constexpr auto constCoord = [](const auto& p) -> decltype(auto)
	{
		if constexpr(IsPointer<decltype(p)>)
			return p->coord();
		else
			return p.coord();
	};

	inline static constexpr auto coord = [](auto& p) -> decltype(auto)
	{
		if constexpr(IsPointer<decltype(p)>)
			return p->coord();
		else
			return p.coord();
	};

	template <std::ranges::range R>
	constexpr auto operator()(R&& r) const
	{
		using ElemType = std::ranges::range_value_t<R>;
		if constexpr(IsConst<ElemType>)
			return r | std::views::transform(constCoord);
		else
			return r | std::views::transform(coord);

	}
};

} // namespace vcl::views::internal

inline constexpr internal::CoordsViewClosure coords;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_COORDS_H
