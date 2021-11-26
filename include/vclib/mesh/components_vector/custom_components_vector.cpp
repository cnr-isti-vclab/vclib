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

#include "custom_components_vector.h"
#include <vclib/misc/compactness.h>

namespace vcl::internal {

template<typename T>
void CustomComponentsVector<T, IfHasCustomProp<T> >::clear()
{
	map.clear();
	needToInitialize.clear();
}

template<typename T>
void CustomComponentsVector<T, IfHasCustomProp<T>>::reserve(uint size)
{
	for (auto& p : map) {
		p.second.reserve(size);
	}
}

template<typename T>
void CustomComponentsVector<T, IfHasCustomProp<T>>::resize(uint size)
{
	for (auto& p : map) {
		p.second.resize(size);
		needToInitialize.at(p.first) = true;
	}
}

template<typename T>
void CustomComponentsVector<T, IfHasCustomProp<T> >::compact(const std::vector<int>& newIndices)
{
	for (auto& p : map) {
		vcl::compactVector(p.second, newIndices);
	}
}

template<typename T>
template<typename AttrType>
void CustomComponentsVector<T, IfHasCustomProp<T>>::addNewComponent(
	const std::string& name,
	uint       size)
{
	std::vector<std::any>& v = map[name];
	v.resize(size, AttrType());
	needToInitialize[name] = false;
}

template<typename T>
void CustomComponentsVector<T, IfHasCustomProp<T>>::assertComponentExists(
	const std::string& attrName) const
{
	(void) (attrName);
	assert(map.find(attrName) != map.end());
}

template<typename T>
bool CustomComponentsVector<T, IfHasCustomProp<T>>::componentExists(
	const std::string& attrName) const
{
	return (map.find(attrName) != map.end());
}

template<typename T>
template<typename AttrType>
const std::vector<std::any>&
CustomComponentsVector<T, IfHasCustomProp<T>>::componentVector(const std::string& attrName) const
{
	std::vector<std::any>& v = const_cast<std::vector<std::any>&>(map.at(attrName));
	if (needToInitialize.at(attrName)) {
		for (std::any& a : v) {
			if (!a.has_value())
				a = AttrType();
		}
		needToInitialize.at(attrName) = false;
	}
	return v;
}

template<typename T>
template<typename AttrType>
std::vector<std::any>&
CustomComponentsVector<T, IfHasCustomProp<T>>::componentVector(const std::string& attrName)
{
	std::vector<std::any>& v = map.at(attrName);
	if (needToInitialize.at(attrName)) {
		for (std::any& a : v) {
			if (!a.has_value())
				a = AttrType();
		}
		needToInitialize.at(attrName) = false;
	}
	return v;
}

} // namespace vcl::internal
