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

#include "name.h"

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
bool Name<El, O>::isEnabled() const
{
	return Base::isAvailable(this);
}

/**
 * @brief Returns the name of this object.
 * @return The name of this object.
 */
template<typename El, bool O>
std::string& Name<El, O>::name()
{
	return Base::data(this);
}

/**
 * @brief Returns the name of this object.
 * @return The name of this object.
 */
template<typename El, bool O>
const std::string& Name<El, O>::name() const
{
	return Base::data(this);
}

template<typename El, bool O>
template<typename Element>
void Name<El, O>::importFrom(const Element &e)
{
	if constexpr(HasName<Element>) {
		name() = e.name();
	}
}

/**
 * @brief Checks if the given Element/Mesh has Name component available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element/mesh to check. Must be of a type that
 * satisfies the ElementOrMeshConcept.
 * @return `true` if the element/mesh has Name component available, `false`
 * otherwise.
 */
bool isNameEnabledOn(const ElementOrMeshConcept auto& element)
{
	return isComponentAvailableOn<NAME>(element);
}

} // namespace vcl::comp
