/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_INFO_H
#define MGP_MESH_COMPONENTS_OPTIONAL_INFO_H

#include <type_traits>

namespace mgp::mesh {

template<typename T>
class ElementContainer;

}

namespace mgp::internal {

template <typename T>
class ComponentsVector;

}

namespace mgp::comp {

template<typename T>
class OptionalInfo
{
public:
	OptionalInfo() {}

protected:
	void setContainerPointer(mgp::mesh::ElementContainer<T>* cp) { contPtr = cp; }

	mgp::internal::ComponentsVector<T>& optCont()
	{
		assert(contPtr != nullptr);
		return contPtr->optionalVec;
	}

	const mgp::internal::ComponentsVector<T>& optCont() const
	{
		assert(contPtr != nullptr);
		return contPtr->optionalVec;
	}

	unsigned int index(const T* e) const
	{
		assert(contPtr != nullptr);
		assert(!contPtr->vec.empty() && e >= contPtr->vec.data() && e <= &contPtr->vec.back());
		return e - contPtr->vec.data();
	}

	mgp::mesh::ElementContainer<T>* contPtr = nullptr;
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

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_OPTIONAL_INFO_H
