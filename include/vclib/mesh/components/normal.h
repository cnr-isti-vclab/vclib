/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#include <vclib/concepts/mesh/components/normal.h>
#include <vclib/space/point.h>

#include "component.h"

namespace vcl::comp {

/**
 * @brief The NormalT class
 *
 * @ingroup components
 */
template<PointConcept P, typename ElementType = void, bool optional = false>
class Normal : public Component<P, ElementType, optional>
{
	using Base = Component<P, ElementType, optional>;
	using ThisType = Normal<P, ElementType, optional>;

public:
	using NormalComponent = ThisType; // expose the type to allow access to this component

	using NormalType = P;

	bool isEnabled() const;
	bool isNormalEnabled() const;

	const P& normal() const;
	P&       normal();

protected:
	template<typename Element>
	void importFrom(const Element& e);
};

/* Detector function to check if a class has Normal enabled */

template <typename T>
bool isNormalEnabledOn(const T& element);

template<
	typename Scalar,
	typename ElementType = void,
	bool optional        = false>
using Normal3 = Normal<Point3<Scalar>, ElementType, optional>;

template<typename ElementType = void, bool optional = false>
using Normal3f = Normal3<float, ElementType, optional>;

template<typename ElementType = void, bool optional = false>
using Normal3d = Normal3<double, ElementType, optional>;

} // namespace vcl::comp

#include "normal.cpp"

#endif // VCL_MESH_COMPONENTS_NORMAL_H
