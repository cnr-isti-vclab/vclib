/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENT_VECTOR_CUSTOM_COMPONENTS_VECTOR_H
#define MGP_MESH_COMPONENT_VECTOR_CUSTOM_COMPONENTS_VECTOR_H

#include <unordered_map>

#include <mgp/mesh/components_optional/custom_components.h>

#include "optional_generic_vector.h"

namespace mgp::internal {

// to shorten triggerer for Vertex class
template<class T>
using IfHasCustomComp = std::enable_if_t<components::hasCustomComponents<T>()>;

template<typename, typename = void>
class CustomComponentsVector
{
public:
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
	void compact(const std::vector<int>&) {}
};

template<typename T>
class CustomComponentsVector<
	T,
	IfHasCustomComp<T>>
{
public:
	void reserve(unsigned int size);

	void resize(unsigned int size);

	void compact(const std::vector<int>& newIndices);

	template<typename AttrType>
	void addNewComponent(const std::string& name, unsigned int size);

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

} // namespace mgp::internal

#include "custom_components_vector.cpp"

#endif // MGP_MESH_COMPONENT_VECTOR_CUSTOM_COMPONENTS_VECTOR_H
