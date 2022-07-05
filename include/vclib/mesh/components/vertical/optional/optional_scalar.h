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

#include "../vertical_info.h"
#include "../../concepts/scalar.h"

namespace vcl::comp {

template<typename S, typename T>
class OptionalScalar : public virtual VerticalInfo<T>
{
private:
	using B = VerticalInfo<T>;
	uint thisId() const { return B::index((T*)this); }

public:
	using ScalarType = S;
	const ScalarType& scalar() const;
	ScalarType&       scalar();

	bool isScalarEnabled() const;

protected:
	template <typename Element>
	void importFrom(const Element& e);
};

template <typename T>
using OptionalScalarf = OptionalScalar<float, T>;

template <typename T>
using OptionalScalard = OptionalScalar<double, T>;

} // namespace vcl::comp

#include "optional_scalar.cpp"

#endif //  VCL_MESH_COMPONENTS_OPTIONAL_SCALAR_H
