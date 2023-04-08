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

#include "component_view.h"

namespace vcl {

namespace views {

namespace internal {

struct ScalarViewClosure
{
	constexpr ScalarViewClosure(){}

	inline static constexpr auto constScalarP = [](const auto* p) -> decltype(auto)
	{
		return p->scalar();
	};

	inline static constexpr auto constScalar = [](const auto& p) -> decltype(auto)
	{
		return p.scalar();
	};

	inline static constexpr auto scalarP = [](auto* p) -> decltype(auto)
	{
		return p->scalar();
	};

	inline static constexpr auto scalar = [](auto& p) -> decltype(auto)
	{
		return p.scalar();
	};

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		using It = std::ranges::iterator_t<R>;
		if constexpr(IteratesOverClass<It>) {
			if constexpr(std::is_const_v<typename It::value_type>)
				return r | std::views::transform(constScalar);
			else
				return r | std::views::transform(scalar);
		}
		else {
			if constexpr(std::is_const_v<It>)
				return r | std::views::transform(constScalarP);
			else
				return r | std::views::transform(scalarP);
		}
	}
};

} // namespace vcl::views::internal

inline constexpr internal::ScalarViewClosure scalars;

} // namespace vcl::views

} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_SCALARS_H
