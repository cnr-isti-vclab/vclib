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

#include <vclib/mesh/iterators/component/coord_iterator.h>

#include "component_view.h"

namespace vcl {

// todo: remove this when clang will support P1814R0 (https://clang.llvm.org/cxx_status.html)
#ifdef __clang__
template<std::ranges::range RngType>
class CoordView : public vcl::View<CoordIterator<std::ranges::iterator_t<RngType>>>
{
	using Base = vcl::View<CoordIterator<std::ranges::iterator_t<RngType>>>;
public:
	CoordView(const RngType& r) :
			Base(CoordIterator(std::ranges::begin(r)), CoordIterator(std::ranges::end(r)))
	{
	}
};
#else
template<std::ranges::range RngType>
using CoordView = internal::ComponentView<RngType, CoordIterator>;
#endif

namespace views {

namespace internal {

struct CoordsViewClosure
{
	constexpr CoordsViewClosure(){}

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return CoordView(r);
	}
};

} // namespace vcl::views::internal

inline constexpr internal::CoordsViewClosure coords;

} // namespace vcl::views

} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_COORDS_H
