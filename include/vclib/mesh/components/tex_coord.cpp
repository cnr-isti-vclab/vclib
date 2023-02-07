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

#include "tex_coord.h"

namespace vcl::comp {

template<typename Scalar, typename El, bool h, bool o>
const typename TexCoord<Scalar, El, h, o>::TexCoordType& TexCoord<Scalar, El, h, o>::texCoord() const
{
	return t();
}

template<typename Scalar, typename El, bool h, bool o>
typename TexCoord<Scalar, El, h, o>::TexCoordType& TexCoord<Scalar, El, h, o>::texCoord()
{
	return t();
}

template<typename Scalar, typename El, bool h, bool o>
template<typename Element>
void TexCoord<Scalar, El, h, o>::importFrom(const Element& e)
{
	if constexpr(HasTexCoord<Element>) {
		if (isTexCoordEnabledOn(e)){
			t() = e.texCoord().template cast<Scalar>();
		}
	}
}

template<typename Scalar, typename El, bool h, bool o>
vcl::TexCoord<Scalar>& TexCoord<Scalar, El, h, o>::t()
{
	return data.template get<El>(this);
}

template<typename Scalar, typename El, bool h, bool o>
const vcl::TexCoord<Scalar>& TexCoord<Scalar, El, h, o>::t() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
