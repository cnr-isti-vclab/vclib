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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_NORMAL_H
#define VCL_MESH_COMPONENTS_OPTIONAL_NORMAL_H

#include <vclib/space/point.h>

#include "optional_info.h"

namespace vcl::comp {

class OptionalNormalTrigger
{
};

template<typename Scalar, int N, typename T>
class OptionalNormal : public OptionalNormalTrigger, public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	using NormalType = Point<Scalar, N>;
	const NormalType&        normal() const { return B::optCont().normal(thisId()); }
	NormalType&              normal() { return B::optCont().normal(thisId()); }

private:
	unsigned int thisId() const { return B::index((T*)this); }
};

template<typename Scalar, typename T>
class OptionalNormal3 : public OptionalNormal<Scalar, 3, T>
{
private:
	using B = OptionalInfo<T>;

public:
	using NormalType = Point3<Scalar>;
};

template<typename T>
using OptionalNormal3f = OptionalNormal3<float, T>;

template<typename T>
using OptionalNormal3d = OptionalNormal3<double, T>;


/**
 * Detector to check if a class has (inherits) OpionalNormal
 */

template<typename T>
using hasOptionalNormalT = std::is_base_of<OptionalNormalTrigger, T>;

template<typename U, typename T>
using ReturnIfHasOptionalNormal = typename std::enable_if<hasOptionalNormalT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalNormal()
{
	return hasOptionalNormalT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_OPTIONAL_NORMAL_H
