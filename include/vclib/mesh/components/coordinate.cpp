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

#include "coordinate.h"

namespace vcl::comp {

template<PointConcept P, typename El, bool h>
const P& CoordT<P, El, h>::coord() const
{
	return p();
}

template<PointConcept P, typename El, bool h>
P& CoordT<P, El, h>::coord()
{
	return p();
}

template<PointConcept P, typename ElementType, bool horizontal>
P& CoordT<P, ElementType, horizontal>::p()
{
	if constexpr (horizontal) {
		return data.p;
	}
	else {
		return internal::getVerticalComponentData(this);
	}
}

template<PointConcept P, typename ElementType, bool horizontal>
const P& CoordT<P, ElementType, horizontal>::p() const
{
	if constexpr (horizontal) {
		return data.p;
	}
	else {
		return internal::getVerticalComponentData(this);
	}
}

template<PointConcept P, typename El, bool h>
template<typename Element>
void CoordT<P, El, h>::importFrom(const Element& v)
{
	if constexpr (HasCoordinate<Element>) {
		p() = v.coord().template cast<typename CoordType::ScalarType>();
	}
}

} // namespace vcl::comp
