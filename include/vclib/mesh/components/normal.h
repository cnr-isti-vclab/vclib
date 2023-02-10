/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_COMPONENTS_NORMAL_H
#define VCL_MESH_COMPONENTS_NORMAL_H

#include <vclib/space/point.h>

#include "concepts/normal.h"
#include "internal/component_data.h"

namespace vcl::comp {

template<PointConcept P, typename ElementType = void, bool optional = false>
class NormalT
{
	using ThisType = NormalT<P, ElementType, optional>;
public:
	using DataValueType = P;         // data that the component stores internally (or vertically)
	using NormalComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	using NormalType = P;

	bool isEnabled() const;
	bool isNormalEnabled() const;

	const P& normal() const;
	P&       normal();

protected:
	template<typename Element>
	void importFrom(const Element& e);

private:
	// members that allow to access the normal, trough data (horizontal) or trough parent (vertical)
	P& n();
	const P& n() const;

	// contians the actual data of the component, if the component is horizontal
	internal::ComponentData<DataValueType, IS_VERTICAL> data;
};

template<
	typename Scalar,
	int N,
	typename ElementType = void,
	bool optional        = false>
using Normal = NormalT<Point<Scalar, N>, ElementType, optional>;

template<
	typename Scalar,
	typename ElementType = void,
	bool optional        = false>
using Normal3 = NormalT<Point3<Scalar>, ElementType, optional>;

template<typename ElementType = void, bool optional = false>
using Normal3f = Normal3<float, ElementType, optional>;

template<typename ElementType = void, bool optional = false>
using Normal3d = Normal3<double, ElementType, optional>;

} // namespace vcl::comp

#include "normal.cpp"

#endif // VCL_MESH_COMPONENTS_NORMAL_H
