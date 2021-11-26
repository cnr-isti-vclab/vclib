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

#ifndef VCL_MESH_COMPONENTS_VECTOR_CUSTOM_COMPONENTS_VECTOR_H
#define VCL_MESH_COMPONENTS_VECTOR_CUSTOM_COMPONENTS_VECTOR_H

#include <unordered_map>

#include <vclib/mesh/components_optional/custom_components.h>

#include "optional_generic_vector.h"

namespace vcl::internal {

// to shorten triggerer for Vertex class
template<typename T>
using IfHasCustomProp = std::enable_if_t<comp::hasCustomComponents<T>()>;

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
	IfHasCustomProp<T>>
{
public:
	void clear();

	void reserve(uint size);

	void resize(uint size);

	void compact(const std::vector<int>& newIndices);

	template<typename AttrType>
	void addNewComponent(const std::string& name, uint size);

	void assertComponentExists(const std::string& attrName) const;;

	bool componentExists(const std::string& attrName) const;;

	template<typename AttrType>
	const std::vector<std::any>& componentVector(const std::string& attrName) const;

	template<typename AttrType>
	std::vector<std::any>& componentVector(const std::string& attrName);

private:
	std::unordered_map<std::string, std::vector<std::any>> map;
	mutable std::unordered_map<std::string, bool> needToInitialize;
};

} // namespace vcl::internal

#include "custom_components_vector.cpp"

#endif // VCL_MESH_COMPONENTS_VECTOR_CUSTOM_COMPONENTS_VECTOR_H
