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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_NORMAL_H
#define VCL_MESH_COMPONENTS_OPTIONAL_NORMAL_H

#include <vclib/space/point.h>

#include "../../concepts/normal.h"
#include "../vertical_component.h"

namespace vcl::comp {

template<typename P, typename T>
class OptionalNormalT : public virtual VerticalComponent<T>
{
private:
	using B = VerticalComponent<T>;
	uint thisId() const { return B::index((T*)this); }

public:
	using NormalType = P;

	const NormalType&        normal() const;
	NormalType&              normal();

	bool isNormalEnabled() const;

	// dummy member to discriminate between non-optional and optional component
	void __optionalNormal() const {};

protected:
	template <typename Element>
	void importFrom(const Element& e);
};

template <typename Scalar, int N, typename T>
using OptionalNormal = OptionalNormalT<vcl::Point<Scalar, N>, T>;

template <typename Scalar, typename T>
using OptionalNormal3 = OptionalNormalT<vcl::Point3<Scalar>, T>;

template<typename T>
using OptionalNormal3f = OptionalNormal3<float, T>;

template<typename T>
using OptionalNormal3d = OptionalNormal3<double, T>;

} // namespace vcl::comp

#include "optional_normal.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_NORMAL_H
