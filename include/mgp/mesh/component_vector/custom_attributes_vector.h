/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENT_VECTOR_CUSTOM_ATTRIBUTES_VECTOR_H
#define MGP_MESH_COMPONENT_VECTOR_CUSTOM_ATTRIBUTES_VECTOR_H

#include <unordered_map>

#include <mgp/mesh/common/custom_attributes.h>

#include "optional_generic_vector.h"

namespace mgp::mesh {

template<typename, typename = void>
class CustomAttributesVector
{
public:
	void reserve(unsigned int) {}
	void resize(unsigned int) {}
};

template<typename T>
class CustomAttributesVector<
	T,
	std::enable_if_t<common::hasCustomAttributes<T>::value>>
{
public:
	void reserve(unsigned int size)
	{
		for (auto& p : map){
			p.second.reserve(size);
		}
	}

	void resize(unsigned int size)
	{
		for (auto& p : map){
			p.second.resize(size);
		}
		needToInitialize = true;
	}

	template<typename AttrType>
	void addNewAttribute(const std::string& name, unsigned int size)
	{
		std::vector<std::any>& v = map[name];
		v.resize(size, AttrType());
	}

	void assertAttributeExists(const std::string& attrName) const
	{
		assert(map.find(attrName) != map.end());
	};

	bool attributeExists(const std::string& attrName) const
	{
		return (map.find(attrName) != map.end());
	};

	template<typename AttrType>
	const std::vector<std::any>& attrVector(const std::string& attrName) const
	{
		std::vector<std::any>& v = const_cast<std::vector<std::any>&>(map.at(attrName));
		if (needToInitialize){
			for (std::any& a : v){
				if (!a.has_value())
					a = AttrType();
			}
			needToInitialize = false;
		}
		return v;
	}

	template<typename AttrType>
	std::vector<std::any>& attrVector(const std::string& attrName)
	{
		std::vector<std::any>& v = map.at(attrName);
		if (needToInitialize){
			for (std::any& a : v){
				if (!a.has_value())
					a = AttrType();
			}
			needToInitialize = false;
		}
		return v;
	}

private:
	std::unordered_map<std::string, std::vector<std::any>> map;
	mutable bool needToInitialize = false;
};

} // namespace mgp::mesh

#endif // MGP_MESH_COMPONENT_VECTOR_CUSTOM_ATTRIBUTES_VECTOR_H
