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

namespace vcl {

namespace views {

namespace internal {



struct CoordsViewClosure
{
	constexpr CoordsViewClosure(){}

	inline static constexpr auto constCoordP = [](const auto* p) -> decltype(auto)
	{
		return p->coord();
	};

	inline static constexpr auto constCoord = [](const auto& p) -> decltype(auto)
	{
		return p.coord();
	};

	inline static constexpr auto coordP = [](auto* p) -> decltype(auto)
	{
		return p->coord();
	};

	inline static constexpr auto coord = [](auto& p) -> decltype(auto)
	{
		return p.coord();
	};

	template <std::ranges::range R>
	constexpr auto operator()(R&& r) const
	{
		using It = std::ranges::iterator_t<R>;
		if constexpr(IteratesOverClass<It>) {
			if constexpr(std::is_const_v<typename It::value_type>)
				return r | std::views::transform(constCoord);
			else
				return r | std::views::transform(coord);
		}
		else {
			if constexpr(std::is_const_v<It>)
				return r | std::views::transform(constCoordP);
			else
				return r | std::views::transform(coordP);
		}
	}
};

} // namespace vcl::views::internal

inline constexpr internal::CoordsViewClosure coords;

} // namespace vcl::views

} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_COORDS_H
