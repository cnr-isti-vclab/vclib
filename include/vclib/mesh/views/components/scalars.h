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

#include <vclib/mesh/iterators/component/scalar_iterator.h>

#include "component_view.h"

namespace vcl {

// todo: remove this when clang will support P1814R0 (https://clang.llvm.org/cxx_status.html)
#ifdef __clang__
template<std::ranges::range RngType>
class ScalarView : public vcl::View<ScalarIterator<std::ranges::iterator_t<RngType>>>
{
	using Base = vcl::View<ScalarIterator<std::ranges::iterator_t<RngType>>>;
public:
	ScalarView(const RngType& r) :
			Base(ScalarIterator(std::ranges::begin(r)), ScalarIterator(std::ranges::end(r)))
	{
	}
};
#else
template<std::ranges::range RngType>
using ScalarView = internal::ComponentView<RngType, ScalarIterator>;
#endif

namespace views {

namespace internal {

struct ScalarViewClosure
{
	constexpr ScalarViewClosure(){}

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return ScalarView(r);
	}
};

} // namespace vcl::views::internal

inline constexpr internal::ScalarViewClosure scalars;

} // namespace vcl::views

} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_SCALARS_H
