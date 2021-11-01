/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_INFO_H
#define MGP_MESH_COMPONENTS_OPTIONAL_INFO_H

#include <type_traits>

namespace mgp::internal {

template<typename T>
class ComponentsVector;

}

namespace mgp::components {

template<typename T>
class OptionalInfo
{
public:
	OptionalInfo() : contPtr(nullptr) {}

protected:
	void setContainerPointer(mgp::internal::ComponentsVector<T>* cp) { contPtr = cp; }

	mgp::internal::ComponentsVector<T>* contPtr;
};

/**
 * Detector to check if a class has (inherits) OptionalInfo
 */

template<typename T>
using hasOptionalInfoT = std::is_base_of<OptionalInfo<T>, T>;

template<typename T>
bool constexpr hasOptionalInfo()
{
	return hasOptionalInfoT<T>::value;
}

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_OPTIONAL_INFO_H
