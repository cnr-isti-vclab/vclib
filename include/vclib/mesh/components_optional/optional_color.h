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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_COLOR_H
#define VCL_MESH_COMPONENTS_OPTIONAL_COLOR_H

#include <vclib/space/color.h>

#include "optional_info.h"

namespace vcl::comp {

template<typename T>
class OptionalColor : public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	using ColorType = vcl::Color;
	const vcl::Color&  color() const { return B::optCont().color(thisId()); }
	vcl::Color&        color() { return B::optCont().color(thisId()); }

private:
	unsigned int thisId() const { return B::index((T*)this); }
};

/**
 * Detector to check if a class has (inherits) OptionalColor
 */

template<typename T>
using hasOptionalColorT = std::is_base_of<OptionalColor<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalColor = typename std::enable_if<hasOptionalColorT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalColor()
{
	return hasOptionalColorT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_OPTIONAL_COLOR_H
