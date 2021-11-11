/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "custom_components_vector.h"
#include <mgp/misc/compactness.h>

namespace mgp::internal {

template<typename T>
void CustomComponentsVector<T, IfHasCustomProp<T> >::clear()
{
	map.clear();
	needToInitialize.clear();
}

template<typename T>
void CustomComponentsVector<T, IfHasCustomProp<T>>::reserve(unsigned int size)
{
	for (auto& p : map) {
		p.second.reserve(size);
	}
}

template<typename T>
void CustomComponentsVector<T, IfHasCustomProp<T>>::resize(unsigned int size)
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
		mgp::compactVector(p.second, newIndices);
	}
}

template<typename T>
template<typename AttrType>
void CustomComponentsVector<T, IfHasCustomProp<T>>::addNewComponent(
	const std::string& name,
	unsigned int       size)
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

} // namespace mgp::internal
