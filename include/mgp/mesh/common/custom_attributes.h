/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_CUSTOM_ATTRIBUTES_H
#define MGP_MESH_COMMON_CUSTOM_ATTRIBUTES_H

#include "optional_info.h"

#include <any>
#include <string>
#include <vector>

namespace mgp::common {

template<typename T>
class CustomAttributes : public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;
public:
	bool hasCustomAttribute(const std::string& attrName) const
	{
		return B::containerPointer->attributeExists(attrName);
	}

	template<typename AttrType>
	const AttrType& customAttribute(const std::string& attrName) const
	{
		return std::any_cast<const AttrType&>(B::containerPointer->attrVector(attrName)[B::id()]);
	}

	template<typename AttrType>
	AttrType& customAttribute(const std::string& attrName)
	{
		return std::any_cast<AttrType&>(B::containerPointer->attrVector(attrName)[B::id()]);
	}
};

/**
 * Detector to check if a class has (inherits) CustomAttributes
 */

template<typename T>
using hasCustomAttributes_t = decltype(std::declval<T&>().hasCustomAttribute(std::declval<const std::string&>()));

template <typename T>
using hasCustomAttributes = typename detector<hasCustomAttributes_t, void, T>::type;

}

#endif // MGP_MESH_COMMON_OPTIONAL_CUSTOM_ATTRIBUTES_H
