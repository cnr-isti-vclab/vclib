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

/**
 * @brief Returns true if the element has a custom component with the given
 * name, false otherwise. The type of the custom component is not checked.
 *
 * @param[in] compName: the name of the custom component.
 * @return true if the element has a custom component with the given name,
 * false otherwise.
 */
template<typename El>
bool CustomComponents<El>::hasCustomComponent(const std::string& compName) const
{
	return data.componentExists(compName, static_cast<const El*>(this));
}

/**
 * @brief Returns true if the custom component of the given name is of the
 * type given as template argument, false otherwise.
 *
 * @note The element must have a custom component with the given name, otherwise
 * a std::out_of_range exception is thrown. If you don't know if the element
 * has a custom component with the given name, use the
 * CustomComponents::hasCustomComponent() member function.
 *
 * @throws std::out_of_range if the element does not have a custom component
 * with the given name.
 *
 * @tparam CompType: the type of the custom component to check.
 * @param[in] compName: the name of the custom component.
 * @return true if the custom component of the given name is of the type
 * CompType, false otherwise.
 */
template<typename El>
template<typename CompType>
bool CustomComponents<El>::isCustomComponentOfType(
	const std::string& compName) const
{
	return data.template isComponentOfType<CompType>(
		compName, static_cast<const El*>(this));
}

/**
 * @brief Returns the `std::type_index` of the custom component of the given
 * name.
 *
 * @note The element must have a custom component with the given name, otherwise
 * a std::out_of_range exception is thrown. If you don't know if the element
 * has a custom component with the given name, use the
 * CustomComponents::hasCustomComponent() member function.
 *
 * @throws std::out_of_range if the element does not have a custom component
 * with the given name.
 *
 * @param[in] compName: the name of the custom component.
 * @return the `std::type_index` of the custom component of the given name.
 */
template<typename El>
std::type_index
CustomComponents<El>::customComponentType(const std::string& compName) const
{
	return data.componentType(compName, static_cast<const El*>(this));
}

/**
 * @brief Returns a std::vector of std::strings containing the names of the
 * custom components of the type given as template argument.
 *
 * @tparam CompType: the type of the custom components to retrieve.
 * @return a std::vector of std::strings containing the names of the custom
 * components of the type given as template argument.
 */
template<typename El>
template<typename CompType>
std::vector<std::string>
CustomComponents<El>::customComponentNamesOfType() const
{
	return data.template componentNamesOfType<CompType>(
		static_cast<const El*>(this));
}

/**
 * @brief Returns the const reference to the custom component of the given
 * name having the type given as template argument.
 *
 * @note The element must have a custom component with the given name, otherwise
 * a std::out_of_range exception is thrown. If you don't know if the element
 * has a custom component with the given name, use the
 * CustomComponents::hasCustomComponent() member function.
 *
 * @note The custom component must be of the type given as template argument,
 * otherwise a std::bad_any_cast exception is thrown. The type of the custom
 * component must be known at compile time.
 *
 * @throws std::out_of_range if the element does not have a custom component
 * with the given name.
 * @throws std::bad_any_cast if the custom component is not of the type given
 * as template argument.
 *
 * @tparam CompType: the type of the custom component to retrieve.
 * @param[in] compName: the name of the custom component.
 * @return the const reference to the custom component of the given name having
 * the type given as template argument.
 */
template<typename El>
template<typename CompType>
const CompType&
CustomComponents<El>::customComponent(const std::string& compName) const
{
	return data.template get<CompType>(compName, static_cast<const El*>(this));
}

/**
 * @brief Returns the reference to the custom component of the given
 * name having the type given as template argument.
 *
 * @note The element must have a custom component with the given name, otherwise
 * a std::out_of_range exception is thrown. If you don't know if the element
 * has a custom component with the given name, use the
 * CustomComponents::hasCustomComponent() member function.
 *
 * @note The custom component must be of the type given as template argument,
 * otherwise a std::bad_any_cast exception is thrown. The type of the custom
 * component must be known at compile time.
 *
 * @throws std::out_of_range if the element does not have a custom component
 * with the given name.
 * @throws std::bad_any_cast if the custom component is not of the type given
 * as template argument.
 *
 * @tparam CompType: the type of the custom component to retrieve.
 * @param[in] compName: the name of the custom component.
 * @return the reference to the custom component of the given name having
 * the type given as template argument.
 */
template<typename El>
template<typename CompType>
CompType& CustomComponents<El>::customComponent(const std::string& compName)
{
	return data.template get<CompType>(compName, static_cast<El*>(this));
}

//template<typename El>
//template<typename CompType>
//void CustomComponents<El>::addCustomComponent(
//	const std::string& compName,
//	const CompType&    value) requires (!IS_VERTICAL)
//{
//	return data.template addCustomComponent<CompType>(compName, value);
//}

//template<typename El>
//void CustomComponents<El>::deleteCustomComponent(const std::string& compName)
//	requires (!IS_VERTICAL)
//{
//	return data.deleteCustomComponent(compName);
//}

template<typename El>
template<typename Element>
void CustomComponents<El>::importFrom(const Element&)
{
	// if the component is vertical, the import is managed by the container.
	// if is horizontal, it must be managed by the component itself.
	if constexpr (!IS_VERTICAL) {
		// TODO
	}
}

} // namespace vcl::comp
