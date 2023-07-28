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

#include "color.h"

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
template<typename El, bool O>
bool Color<El, O>::isEnabled() const
{
	return Base::isAvailable(this);
}

/**
 * @brief Returns a const reference of the color of the element.
 * @return a const reference of the color of the element.
 */
template<typename El, bool O>
const vcl::Color& Color<El, O>::color() const
{
	return Base::data(this);
}

/**
 * @brief Returns a reference pf the color of the element.
 * @return a reference pf the color of the element.
 */
template<typename El, bool O>
vcl::Color& Color<El, O>::color()
{
	return Base::data(this);
}

template<typename El, bool O>
template<typename Element>
void Color<El, O>::importFrom(const Element& e)
{
	if constexpr (HasColor<Element>) {
		if (isColorEnabledOn(e)) {
			color() = e.color();
		}
	}
}

/**
 * @brief Checks if the given Element/Mesh has Color component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element/mesh to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element/mesh has Color component available, `false`
 * otherwise.
 */
bool isColorEnabledOn(const ElementOrMeshConcept auto& element)
{
	return isComponentAvailableOn<COLOR>(element);
}

} // namespace vcl::comp
