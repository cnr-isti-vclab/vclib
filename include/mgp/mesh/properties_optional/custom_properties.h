/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_CUSTOM_PROPERTIES_H
#define MGP_MESH_PROPERTIES_CUSTOM_PROPERTIES_H

#include "optional_info.h"

#include <any>
#include <string>
#include <vector>

namespace mgp::prop {

/**
 * @brief The CustomProperties class is a container of custom and additional properties associated
 * to an Element (e.g. Vertex, Face).
 *
 */
template<typename T>
class CustomProperties : public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	bool hasCustomProperty(const std::string& attrName) const
	{
		return B::contPtr->propertyExists(attrName);
	}

	template<typename AttrType>
	const AttrType& customProperty(const std::string& attrName) const
	{
		return std::any_cast<const AttrType&>(
			B::contPtr->template propertyVector<AttrType>(attrName)[((T*) this)->id()]);
	}

	template<typename AttrType>
	AttrType& customProperty(const std::string& attrName)
	{
		return std::any_cast<AttrType&>(
			B::contPtr->template propertyVector<AttrType>(attrName)[((T*) this)->id()]);
	}
};

/**
 * Detector to check if a class has (inherits) CustomProperties
 */

template<typename T>
using hasCustomPropertiesT = std::is_base_of<CustomProperties<T>, T>;

template<typename U, typename T>
using ReturnIfHasCustomProperties =
	typename std::enable_if<hasCustomPropertiesT<U>::value, T>::type;

template<typename T>
bool constexpr hasCustomProperties()
{
	return hasCustomPropertiesT<T>::value;
}

} // namespace mgp::prop

#endif // MGP_MESH_PROPERTIES_CUSTOM_PROPERTIES_H
