/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
#define MGP_MESH_COMPONENTS_CUSTOM_COMPONENTS_H

#include "optional_info.h"

#include <any>
#include <string>
#include <vector>

namespace mgp::components {

/**
 * @brief The CustomComponents class is a container of custom and additionalcomponents associated
 * to an Element (e.g. Vertex, Face).
 *
 */
template<typename T>
class CustomComponents : public virtual OptionalInfo<T>
{
private:
	using B = OptionalInfo<T>;

public:
	bool hasCustomComponent(const std::string& attrName) const
	{
		return B::contPtr->componentExists(attrName);
	}

	template<typename AttrType>
	const AttrType& customComponent(const std::string& attrName) const
	{
		return std::any_cast<const AttrType&>(
			B::contPtr->template componentVector<AttrType>(attrName)[((T*) this)->id()]);
	}

	template<typename AttrType>
	AttrType& customComponent(const std::string& attrName)
	{
		return std::any_cast<AttrType&>(
			B::contPtr->template componentVector<AttrType>(attrName)[((T*) this)->id()]);
	}
};

/**
 * Detector to check if a class has (inherits) CustomComponents
 */

template<typename T>
using hasCustomComponentsT = std::is_base_of<CustomComponents<T>, T>;

template<typename U, typename T>
using ReturnIfHasCustomComponents =
	typename std::enable_if<hasCustomComponentsT<U>::value, T>::type;

template<typename T>
bool constexpr hasCustomComponents()
{
	return hasCustomComponentsT<T>::value;
}

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_CUSTOM_COMPONENTS_H
