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

template<PointConcept P, typename El, bool h>
const P& NormalT<P, El, h>::normal() const
{
	return n();
}

template<PointConcept P, typename El, bool h>
P& NormalT<P, El, h>::normal()
{
	return n();
}

template<PointConcept P, typename El, bool h>
template<typename Element>
void NormalT<P, El, h>::importFrom(const Element& e)
{
	if constexpr(HasNormal<Element>) {
		if (isNormalEnabledOn(e)){
			n() = e.normal().template cast<typename NormalType::ScalarType>();
		}
	}
}

template<PointConcept P, typename El, bool h>
P& NormalT<P, El, h>::n()
{
	if constexpr (h) {
		return data.n;
	}
	else {
		return internal::getVerticalComponentData<El>(this);
	}
}

template<PointConcept P, typename El, bool h>
const P& NormalT<P, El, h>::n() const
{
	if constexpr (h) {
		return data.n;
	}
	else {
		return internal::getVerticalComponentData<El>(this);
	}
}

} // namespace vcl::comp
