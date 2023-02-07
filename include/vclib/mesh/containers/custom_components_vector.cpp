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

#include "custom_components_vector.h"

#include <vclib/exception/mesh_exception.h>
#include <vclib/misc/compactness.h>

namespace vcl::mesh {

/**
 * @brief Removes all the custom components associated to the T element
 */
template<typename T>
void CustomComponentsVector<T, true>::clear()
{
	map.clear();
	needToInitialize.clear();
	componentType.clear();
}

/**
 * @brief When the method reserve() is called on the container of the T element, it is necessary to
 * reserve also all the vectors for each custom component of the T element.
 * @param size
 */
template<typename T>
void CustomComponentsVector<T, true>::reserve(uint size)
{
	for (auto& p : map) {
		p.second.reserve(size);
	}
}

/**
 * @brief When the method resize() is called on the container of the T element, it is necessary to
 * resize also all the vectors for each custom component of the T element.
 *
 * At this call, we don't know statically the types of each custo component, therefore we cannot
 * initialize them (std::any of each vector will be in an invalid state). Therefore, we mark all the
 * resized custom components as 'needToInitialize'. Initiaization will be made just on the
 * uninitialized values of the vectors at the first access of each custom component.
 * @param size
 */
template<typename T>
void CustomComponentsVector<T, true>::resize(uint size)
{
	for (auto& p : map) {
		if (p.second.size() < size)
			needToInitialize.at(p.first) = true;
		p.second.resize(size);
	}
}

/**
 * @brief When the method compact() is called on the container of the T element, it is necessary to
 * compact also the vectors for each vustom component of the T element.
 *
 * We just need to compact each custom component vector according to the new indices given. Moving a
 * std::any object from a vector position to another is perfectly safe.
 * @param newIndices
 */
template<typename T>
void CustomComponentsVector<T, true>::compact(const std::vector<int>& newIndices)
{
	for (auto& p : map) {
		vcl::compactVector(p.second, newIndices);
	}
}

/**
 * @brief Adds a new custom component to the T element, with the given name and with the template
 * argumet CompType. The size is the same of the Container of the T element.
 *
 * A new vector of custom components is added, and data are initialized with the empty constructor
 * of CompType.
 *
 * @param name
 * @param size
 */
template<typename T>
template<typename CompType>
void CustomComponentsVector<T, true>::addNewComponent(
	const std::string& name,
	uint       size)
{
	std::vector<std::any>& v = map[name];
	v.resize(size, CompType());
	needToInitialize[name] = false;
	componentType.insert({name, typeid(CompType)});
}

/**
 * @brief Deletes the custom component with the given name from the T element.
 * It does nothing if the element does not exist.
 * @param name
 */
template<typename T>
void CustomComponentsVector<T, true>::deleteComponent(const std::string& name)
{
	map.erase(name);
	needToInitialize.erase(name);
	componentType.erase(name);
}

/**
 * @brief Asserts that the compName component exists.
 * @param compName
 */
template<typename T>
void CustomComponentsVector<T, true>::assertComponentExists(
	const std::string& compName) const
{
	(void) (compName);
	assert(map.find(compName) != map.end());
}

/**
 * @brief Returns true if the compName exists.
 * @param compName
 * @return
 */
template<typename T>
bool CustomComponentsVector<T, true>::componentExists(
	const std::string& compName) const
{
	return (map.find(compName) != map.end());
}

/**
 * @brief Returns a vector of all the custom components associated to the T element, regardless the
 * types of the custom components.
 * @return
 */
template<typename T>
std::vector<std::string> CustomComponentsVector<T, true>::allComponentNames() const
{
	std::vector<std::string> names(map.size());
	for (const auto& p : map)
		names.push_back(p.first);
	return names;
}

/**
 * @brief Returns true if the type associated to the compName custom component is the same of the
 * given template argument CompType.
 * @param compName
 * @return
 */
template<typename T>
template<typename CompType>
bool CustomComponentsVector<T, true>::isComponentOfType(
	const std::string& compName) const
{
	std::type_index t(typeid(CompType));
	return t == componentType.at(compName);
}

/**
 * @brief Returns a vector of all the custom components associated to the T element having as type
 * the given template argument CompType.
 * @return
 */
template<typename T>
template<typename CompType>
std::vector<std::string> CustomComponentsVector<T, true>::allComponentNamesOfType() const
{
	std::vector<std::string> names;
	std::type_index t(typeid(CompType));
	for (const auto& p : componentType) {
		if (p.second == t)
			names.push_back(p.first);
	}
	return names;
}

/**
 * @brief Returns a const reference of std::vector<std::any> of the custom component with the given
 * name and the given template argument CompType.
 *
 * If the CompType does not mach with the type associated with compName, thows a
 * vcl::BadCustomComponentTypeException.
 *
 * This function will initialize all the uninitialized values of the accessed custom component.
 *
 * @param compName
 * @return
 */
template<typename T>
template<typename CompType>
const std::vector<std::any>&
CustomComponentsVector<T, true>::componentVector(const std::string& compName) const
{
	checkComponentType<CompType>(compName);
	std::vector<std::any>& v = const_cast<std::vector<std::any>&>(map.at(compName));
	if (needToInitialize.at(compName)) {
		for (std::any& a : v) {
			if (!a.has_value())
				a = CompType();
		}
		needToInitialize.at(compName) = false;
	}
	return v;
}

/**
 * @brief Returns a reference of std::vector<std::any> of the custom component with the given
 * name and the given template argument CompType.
 *
 * If the CompType does not mach with the type associated with compName, thows a
 * vcl::BadCustomComponentTypeException.
 *
 * This function will initialize all the uninitialized values of the accessed custom component.
 *
 * @param compName
 * @return
 */
template<typename T>
template<typename CompType>
std::vector<std::any>&
CustomComponentsVector<T, true>::componentVector(const std::string& compName)
{
	checkComponentType<CompType>(compName);
	std::vector<std::any>& v = map.at(compName);
	if (needToInitialize.at(compName)) {
		for (std::any& a : v) {
			if (!a.has_value())
				a = CompType();
		}
		needToInitialize.at(compName) = false;
	}
	return v;
}

template<typename T>
template<typename CompType>
void CustomComponentsVector<T, true>::checkComponentType(
	const std::string& compName) const
{
	std::type_index t(typeid(CompType));
	if (t != componentType.at(compName)) {
		throw BadCustomComponentTypeException(
			"Expected type " + std::string(componentType.at(compName).name()) + " for " +
			compName + ", but was " + std::string(t.name()) + ".");
	}
}

} // namespace vcl::internal
