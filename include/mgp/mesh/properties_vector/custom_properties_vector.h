/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_VECTOR_CUSTOM_PROPERTIES_VECTOR_H
#define MGP_MESH_PROPERTIES_VECTOR_CUSTOM_PROPERTIES_VECTOR_H

#include <unordered_map>

#include <mgp/mesh/properties_optional/custom_properties.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

// to shorten triggerer for Vertex class
template<class T>
using IfHasCustomProp = std::enable_if_t<prop::hasCustomProperties<T>()>;

template<typename, typename = void>
class CustomPropertiesVector
{
public:
	void clear() {}
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class CustomPropertiesVector<
	T,
	IfHasCustomProp<T>>
{
public:
	void clear();

	void reserve(unsigned int size);

	void resize(unsigned int size);

	void compact(const std::vector<int>& newIndices);

	template<typename AttrType>
	void addNewProperty(const std::string& name, unsigned int size);

	void assertPropertyExists(const std::string& attrName) const;;

	bool propertyExists(const std::string& attrName) const;;

	template<typename AttrType>
	const std::vector<std::any>& propertyVector(const std::string& attrName) const;

	template<typename AttrType>
	std::vector<std::any>& propertyVector(const std::string& attrName);

private:
	std::unordered_map<std::string, std::vector<std::any>> map;
	mutable std::unordered_map<std::string, bool> needToInitialize;
};

} // namespace mgp::internal

#include "custom_properties_vector.cpp"

#endif // MGP_MESH_PROPERTIES_VECTOR_CUSTOM_PROPERTIES_VECTOR_H
