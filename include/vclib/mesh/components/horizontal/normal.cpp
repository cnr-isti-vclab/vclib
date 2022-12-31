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

#include "normal.h"

namespace vcl::comp {

template<PointConcept P>
const P& NormalT<P>::normal() const
{
	return n;
}

template<PointConcept P>
P& NormalT<P>::normal()
{
	return n;
}

template<PointConcept P>
template<typename Element>
void NormalT<P>::importFrom(const Element& e)
{
	if constexpr(HasNormal<Element>) {
		if (isNormalEnabledOn(e)){
			n = e.normal().template cast<typename NormalType::ScalarType>();
		}
	}
}

} // namespace vcl::comp
