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

#include "normal.h"

namespace vcl::comp {

template<PointConcept P, typename El, bool o>
bool NormalT<P, El, o>::isEnabled() const
{
	return data.template isComponentEnabled<El>(this);
}

template<PointConcept P, typename El, bool o>
bool NormalT<P, El, o>::isNormalEnabled() const
{
	return isEnabled();
}

template<PointConcept P, typename El, bool o>
const P& NormalT<P, El, o>::normal() const
{
	return n();
}

template<PointConcept P, typename El, bool o>
P& NormalT<P, El, o>::normal()
{
	return n();
}

template<PointConcept P, typename El, bool o>
template<typename Element>
void NormalT<P, El, o>::importFrom(const Element& e)
{
	if constexpr(HasNormal<Element>) {
		if (isNormalEnabledOn(e)){
			n() = e.normal().template cast<typename NormalType::ScalarType>();
		}
	}
}

template<PointConcept P, typename El, bool o>
P& NormalT<P, El, o>::n()
{
	return data.template get<El>(this);
}

template<PointConcept P, typename El, bool o>
const P& NormalT<P, El, o>::n() const
{
	return data.template get<El>(this);
}

template <typename T>
bool isNormalEnabledOn(const T& element)
{
	if constexpr (HasOptionalNormal<T>) {
		return element.isNormalEnabled();
	}
	else {
		return HasNormal<T>;
	}
}

} // namespace vcl::comp
