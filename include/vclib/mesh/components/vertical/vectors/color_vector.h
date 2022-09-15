/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_MESH_COMPONENTS_COLOR_VECTOR_H
#define VCL_MESH_COMPONENTS_COLOR_VECTOR_H

#include "../../concepts/color.h"

#include "generic_component_vector.h"

namespace vcl::internal {

template<typename>
class ColorVector
{
public:
	void clear() {}
	void resize(uint) {}
	void reserve(uint) {}
	void compact(const std::vector<int>&) {}
	bool isColorEnabled() const { return false; };
};

template<comp::HasOptionalColor T>
class ColorVector<T> : private GenericComponentVector<typename T::ColorType>
{
	using ColorType = typename T::ColorType;
	using Base = GenericComponentVector<ColorType>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;

	bool isColorEnabled() const { return Base::isEnabled(); };
	void enableColor(uint size) { Base::enable(size); }
	void disableColor() { Base::disable(); }

	ColorType&       color(uint i) { return Base::at(i); }
	const ColorType& color(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_COLOR_VECTOR_H
