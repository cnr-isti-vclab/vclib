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

template<PointConcept P, typename El, bool O>
bool Normal<P, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<PointConcept P, typename El, bool O>
bool Normal<P, El, O>::isNormalEnabled() const
{
	return isEnabled();
}

template<PointConcept P, typename El, bool O>
const P& Normal<P, El, O>::normal() const
{
	return Base::data(this);
}

template<PointConcept P, typename El, bool O>
P& Normal<P, El, O>::normal()
{
	return Base::data(this);
}

template<PointConcept P, typename El, bool O>
template<typename Element>
void Normal<P, El, O>::importFrom(const Element& e)
{
	if constexpr(HasNormal<Element>) {
		if (isNormalEnabledOn(e)){
			normal() = e.normal().template cast<typename NormalType::ScalarType>();
		}
	}
}

template <typename T>
bool isNormalEnabledOn(const T& element)
{
	return isComponentEnabledOn<NORMAL>(element);
}

} // namespace vcl::comp
