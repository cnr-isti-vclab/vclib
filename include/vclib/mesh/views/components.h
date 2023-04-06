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

#ifndef VCL_MESH_VIEWS_COMPONENTS_H
#define VCL_MESH_VIEWS_COMPONENTS_H

#include "pipe.h"

#include <vclib/mesh/iterators/component.h>

namespace vcl {
namespace internal {

struct CoordsViewClosure
{
	constexpr CoordsViewClosure(){}

	template <typename R>
	constexpr auto operator()(R && r) const
	{
		return CoordView(r);
	}
};

struct ScalarViewClosure
{
	constexpr ScalarViewClosure(){}

	template <typename R>
	constexpr auto operator()(R && r) const
	{
		return ScalarView(r);
	}
};

struct SelectionViewClosure
{
	constexpr SelectionViewClosure(){}

	template <typename R>
	constexpr auto operator()(R && r) const
	{
		return SelectionView(r);
	}
};

}

namespace views {

internal::CoordsViewClosure coords;
internal::ScalarViewClosure scalars;
internal::SelectionViewClosure selection;

} // namespace vcl::views

} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_H
