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

#include "coordinate.h"

namespace vcl::comp {

/**
 * @private
 * @brief Returns `true` if the component is enabled, `false` otherwise.
 * This member function can return `false` only if the component is optional.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is enabled, `false` otherwise.
 */
template<PointConcept P, typename El, bool O>
bool Coordinate<P, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns a const reference of the coordinate of the element.
 * @return a const reference of the coordinate of the element.
 */
template<PointConcept P, typename El, bool O>
const P& Coordinate<P, El, O>::coord() const
{
	return Base::data(this);
}

/**
 * @brief Returns a reference of the coordinate of the element.
 * @return a reference of the coordinate of the element.
 */
template<PointConcept P, typename El, bool O>
P& Coordinate<P, El, O>::coord()
{
	return Base::data(this);
}

template<PointConcept P, typename El, bool O>
template<typename Element>
void Coordinate<P, El, O>::importFrom(const Element& v)
{
	if constexpr (HasCoordinate<Element>) {
		coord() = v.coord().template cast<typename CoordType::ScalarType>();
	}
}

/**
 * @brief Checks if the given Element has Coordinate component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element has Coordinate component available, `false`
 * otherwise.
 */
bool isCoordinateEnabledOn(const ElementConcept auto& element)
{
	return isComponentEnabledOn<COORDINATE>(element);
}

} // namespace vcl::comp
