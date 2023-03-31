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

template<PointConcept P, typename El, bool o>
const P& CoordT<P, El, o>::coord() const
{
	return p();
}

template<PointConcept P, typename El, bool o>
P& CoordT<P, El, o>::coord()
{
	return p();
}

template<PointConcept P, typename El, bool o>
template<typename Element>
void CoordT<P, El, o>::importFrom(const Element& v)
{
	if constexpr (HasCoordinate<Element>) {
		p() = v.coord().template cast<typename CoordType::ScalarType>();
	}
}

template<PointConcept P, typename El, bool o>
P& CoordT<P, El, o>::p()
{
	return data.template get<El>(this);
}

template<PointConcept P, typename El, bool o>
const P& CoordT<P, El, o>::p() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
