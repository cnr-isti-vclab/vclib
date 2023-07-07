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

#ifndef VCL_MESH_COMPONENTS_COORDINATE_H
#define VCL_MESH_COMPONENTS_COORDINATE_H

#include <vclib/concepts/mesh/components/coordinate.h>
#include <vclib/space/point.h>

#include "bases/component.h"

namespace vcl::comp {
/**
 * @brief The Coordinate class represents a N-Dimensional point that will be
 * part of an Element (e.g. Vertex...).
 *
 * Allows to get and set an object that represents a coordinate. The type of
 * this object must satisfy the PointConcept.
 *
 * @tparam P: The type of the coordinate. This template argument must be a type
 * that satisfies the PointConcept.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<PointConcept P, typename ElementType = void, bool OPT = false>
class Coordinate : public Component<COORDINATE, P, ElementType, OPT>
{
	using Base = Component<COORDINATE, P, ElementType, OPT>;

public:
	using CoordType = P;

	bool isEnabled() const;

	const P& coord() const;
	P&       coord();

protected:
	// Component interface function
	template<typename Element>
	void importFrom(const Element& v);
};

/* Detector function to check if a class has Coordinate enabled */

bool isCoordinateEnabledOn(const ElementConcept auto& element);

/**
 * @brief The Coordinate3 class is an alias of the Coordinate component that
 * uses 3 dimensional Points.
 *
 * @tparam Scalar: The scalar type of the point.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<
	typename Scalar,
	typename ElementType = void,
	bool OPT        = false>
using Coordinate3 = Coordinate<Point3<Scalar>, ElementType, OPT>;

/**
 * @brief The Coordinate3f class is an alias of the Coordinate component that
 * uses 3 dimensional Points with float precision.
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
using Coordinate3f = Coordinate3<float, ElementType, OPT>;

/**
 * @brief The Coordinate3d class is an alias of the Coordinate component that
 * uses 3 dimensional Points with double precision.
 *
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename ElementType = void, bool OPT = false>
using Coordinate3d = Coordinate3<double, ElementType, OPT>;

} // namespace vcl::comp

#include "coordinate.cpp"

#endif // VCL_MESH_COMPONENTS_COORDINATE_H
