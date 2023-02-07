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

#include "custom_components.h"

namespace vcl::comp {

template<typename El>
bool CustomComponents<El>::hasCustomComponent(const std::string& attrName) const
{
	return ccVec().componentExists(attrName);
}

template<typename El>
template<typename CompType>
const CompType& CustomComponents<El>::customComponent(const std::string& attrName) const
{
	return std::any_cast<const CompType&>(
		ccVec().template componentVector<CompType>(attrName)[thisId()]);
}

template<typename El>
template<typename CompType>
CompType& CustomComponents<El>::customComponent(const std::string& attrName)
{
	return std::any_cast<CompType&>(
		ccVec().template componentVector<CompType>(attrName)[thisId()]);
}

template<typename El>
template<typename Element>
void CustomComponents<El>::importFrom(const Element&)
{
	// todo
}

template<typename El>
uint CustomComponents<El>::thisId() const
{
	const El* elem = static_cast<const El*>(this);
	assert(elem->parentMesh());
	return elem->index();
}

template<typename El>
auto& CustomComponents<El>::ccVec()
{
	El* elem = static_cast<El*>(this);
	assert(elem->parentMesh());
	// get the tuple of vector of vertical components
	return elem->parentMesh()->template customComponents<El>();
}

template<typename El>
const auto& CustomComponents<El>::ccVec() const
{
	const El* elem = static_cast<const El*>(this);
	assert(elem->parentMesh());
	// get the tuple of vector of vertical components
	return elem->parentMesh()->template customComponents<El>();
}

} // namespace vcl::comp
