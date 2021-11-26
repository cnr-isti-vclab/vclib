/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_SCALAR_H
#define VCL_MESH_COMPONENTS_OPTIONAL_SCALAR_H

#include "optional_info.h"

namespace vcl::comp {

class OptionalScalarTrigger
{
};

template<typename S, typename T>
class OptionalScalar : public OptionalScalarTrigger, public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	using ScalarType = S;
	const ScalarType& scalar() const { return B::optCont().scalar(thisId()); }
	ScalarType&       scalar() { return B::optCont().scalar(thisId()); }

private:
	uint thisId() const { return B::index((T*)this); }
};

template <typename T>
using OptionalScalarf = OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = OptionalScalar<double, T>;

/**
 * Detector to check if a class has (inherits) OpionalScalar
 */

template<typename T>
using hasOptionalScalarT = std::is_base_of<OptionalScalarTrigger, T>;

template<typename T>
bool constexpr hasOptionalScalar()
{
	return hasOptionalScalarT<T>::value;
}

} // namespace vcl::comp

#endif //  VCL_MESH_COMPONENTS_OPTIONAL_SCALAR_H
