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
	return data.componentExists(attrName, static_cast<const El*>(this));
}

//template<typename El>
//template<typename CompType>
//bool CustomComponents<El>::isCustomComponentOfType(const std::string& compName) const
//	requires(!IS_VERTICAL)
//{
//	return data.template isCustomComponentOfType<CompType>(compName);
//}

template<typename ElementType>
std::type_index CustomComponents<ElementType>::customComponentType(const std::string &compName) const
{
	return data.customComponentType(compName);
}

//template<typename El>
//template<typename CompType>
//std::vector<std::string> CustomComponents<El>::customComponentNamesOfType() const
//	requires(!IS_VERTICAL)
//{
//	return data.template customComponentNamesOfType<CompType>();
//}

template<typename El>
template<typename CompType>
const CompType& CustomComponents<El>::customComponent(const std::string& attrName) const
{
	return data.template get<CompType>(attrName, static_cast<const El*>(this));
}

template<typename El>
template<typename CompType>
CompType& CustomComponents<El>::customComponent(const std::string& attrName)
{
	return data.template get<CompType>(attrName, static_cast<El*>(this));
}

template<typename El>
template<typename Element>
void CustomComponents<El>::importFrom(const Element&)
{
}

//template<typename El>
//template<typename CompType>
//void CustomComponents<El>::addCustomComponent(const std::string& compName, const CompType& value) requires (!IS_VERTICAL)
//{
//	return data.template addCustomComponent<CompType>(compName, value);
//}

//template<typename El>
//void CustomComponents<El>::deleteCustomComponent(const std::string& compName)
//	requires(!IS_VERTICAL)
//{
//	return data.deleteCustomComponent(compName);
//}

} // namespace vcl::comp
