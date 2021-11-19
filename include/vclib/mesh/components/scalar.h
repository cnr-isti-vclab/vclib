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

#ifndef VCL_MESH_COMPONENTS_SCALAR_H
#define VCL_MESH_COMPONENTS_SCALAR_H

#include <type_traits>

namespace vcl::comp {

class ScalarTrigger
{
};

template<typename T>
class Scalar : public ScalarTrigger
{
public:
	using ScalarType = T;

	const ScalarType& scalar() const { return s; }
	ScalarType&       scalar() { return s; }

private:
	ScalarType s;
};

using Scalarf = Scalar<float>;
using Scalard = Scalar<double>;

/**
 * Detector to check if a class has (inherits) Scalar
 */

template<typename T>
using hasScalarT = std::is_base_of<ScalarTrigger, T>;

template<typename T>
bool constexpr hasScalar()
{
	return hasScalarT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_SCALAR_H
