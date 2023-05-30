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

#include "tex_coord.h"

namespace vcl::comp {

template<typename Scalar, typename El, bool O>
bool TexCoord<Scalar, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

template<typename Scalar, typename El, bool O>
bool TexCoord<Scalar, El, O>::isTexCoordEnabled() const
{
	return isEnabled();
}

template<typename Scalar, typename El, bool O>
const typename TexCoord<Scalar, El, O>::TexCoordType& TexCoord<Scalar, El, O>::texCoord() const
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool O>
typename TexCoord<Scalar, El, O>::TexCoordType& TexCoord<Scalar, El, O>::texCoord()
{
	return Base::data(this);
}

template<typename Scalar, typename El, bool O>
template<typename Element>
void TexCoord<Scalar, El, O>::importFrom(const Element& e)
{
	if constexpr(HasTexCoord<Element>) {
		if (isTexCoordEnabledOn(e)){
			texCoord() = e.texCoord().template cast<Scalar>();
		}
	}
}

template <typename T>
bool isTexCoordEnabledOn(const T& element)
{
	return isComponentEnabledOn<TEX_COORD>(element);
}

} // namespace vcl::comp
