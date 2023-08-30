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

/**
 * @brief Returns a const reference of the tex coord of the element.
 * @return a const reference of the tex coord of the element.
 */
template<typename Scalar, typename El, bool O>
const typename TexCoord<Scalar, El, O>::TexCoordType&
TexCoord<Scalar, El, O>::texCoord() const
{
	return Base::data();
}

/**
 * @brief Returns a reference of the tex coord of the element.
 * @return a reference of the tex coord of the element.
 */
template<typename Scalar, typename El, bool O>
typename TexCoord<Scalar, El, O>::TexCoordType&
TexCoord<Scalar, El, O>::texCoord()
{
	return Base::data();
}

template<typename Scalar, typename El, bool O>
template<typename Element>
void TexCoord<Scalar, El, O>::importFrom(const Element& e)
{
	if constexpr(HasTexCoord<Element>) {
		if (isTexCoordAvailableOn(e)){
			texCoord() = e.texCoord().template cast<Scalar>();
		}
	}
}

/**
 * @brief Checks if the given Element has TexCoord component
 * available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that
 * satisfies the ElementConcept.
 * @return `true` if the element has TexCoord component available,
 * `false` otherwise.
 */
bool isTexCoordAvailableOn(const ElementConcept auto& element)
{
	return isComponentAvailableOn<TEX_COORD>(element);
}

} // namespace vcl::comp
