/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_VECTOR_H
#define VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_VECTOR_H

#include <unordered_map>

#include <typeindex>

#include "../persistent/custom_components.h"

#include "generic_component_vector.h"

namespace vcl::internal {

// to shorten triggerer for Vertex class
template<typename T>
using IfHasCustomComp = std::enable_if_t<comp::HasCustomComponents<T>>;

template<typename, typename = void>
class CustomComponentsVector
{
public:
	void clear() {}
	void reserve(uint) {}
	void resize(uint) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class CustomComponentsVector<
	T,
	IfHasCustomComp<T>>
{
public:
	void clear();

	void reserve(uint size);

	void resize(uint size);

	void compact(const std::vector<int>& newIndices);

	template<typename CompType>
	void addNewComponent(const std::string& name, uint size);

	void deleteComponent(const std::string& name);

	void assertComponentExists(const std::string& compName) const;

	bool componentExists(const std::string& compName) const;

	std::vector<std::string> allComponentNames() const;

	template<typename CompType>
	bool isComponentOfType(const std::string& compName) const;

	template<typename CompType>
	std::vector<std::string> allComponentNamesOfType() const;

	template<typename CompType>
	const std::vector<std::any>& componentVector(const std::string& compName) const;

	template<typename CompType>
	std::vector<std::any>& componentVector(const std::string& compName);

private:
	// the actual map containing, for each name of a custom component, the vector
	// of values (a value for each element(vertex/face...) of the mesh)
	std::unordered_map<std::string, std::vector<std::any>> map;
	// map that stores if some custom components need to be initialized
	// this happens when we resize the container of an element: we need to resize also the vectors
	// of the custom components, but we cannot initialize them with the right type (because we don't
	// know it in resize phase). The initialization will be made when the type is known (get of the
	// custom component vector).
	mutable std::unordered_map<std::string, bool> needToInitialize;
	// types used for each custom component
	std::unordered_map<std::string, std::type_index> componentType;

	template<typename CompType>
	void checkComponentType(const std::string& compName) const;
};

} // namespace vcl::internal

#include "custom_components_vector.cpp"

#endif // VCL_MESH_COMPONENTS_CUSTOM_COMPONENTS_VECTOR_H
