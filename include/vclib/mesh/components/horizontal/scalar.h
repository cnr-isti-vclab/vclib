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

#ifndef VCL_MESH_COMPONENTS_SCALAR_H
#define VCL_MESH_COMPONENTS_SCALAR_H

#include "../concepts/scalar.h"

namespace vcl::comp {

template<typename T>
class Scalar
{
public:
	using ScalarType = T;

	const ScalarType& scalar() const;
	ScalarType&       scalar();

	constexpr bool isScalarEnabled() const { return true; }

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	ScalarType s;
};

using Scalarf = Scalar<float>;
using Scalard = Scalar<double>;

} // namespace vcl::comp

#include "scalar.cpp"

#endif // VCL_MESH_COMPONENTS_SCALAR_H
