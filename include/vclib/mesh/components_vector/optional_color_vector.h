/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_COLOR_VECTOR_H
#define VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_COLOR_VECTOR_H

#include <vclib/mesh/components_optional/optional_color.h>

#include "optional_generic_vector.h"

namespace vcl::internal {

template<typename, typename = void>
class OptionalColorVector
{
public:
	void clear() {}
	void resize(unsigned int) {}
	void reserve(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class OptionalColorVector<T, std::enable_if_t<comp::hasOptionalColor<T>()>> :
		private OptionalGenericVector<typename T::ColorType>
{
	using ColorType = typename T::ColorType;
	using Base = OptionalGenericVector<ColorType>;

public:
	using Base::clear;
	using Base::reserve;
	using Base::resize;
	using Base::compact;
	bool             isColorEnabled() const { return Base::isEnabled(); };
	void             enableColor(unsigned int size) { Base::enable(size); }
	void             disableColor() { Base::disable(); }
	ColorType&       color(unsigned int i) { return Base::at(i); }
	const ColorType& color(unsigned int i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_VECTOR_OPTIONAL_COLOR_VECTOR_H
