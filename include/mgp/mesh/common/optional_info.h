/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_INFO_H
#define MGP_MESH_COMMON_OPTIONAL_INFO_H

#include <mgp/misc/type_traits.h>

#include "info.h"

namespace mgp {

namespace mesh {
template<typename T>
class OptionalComponentsVector;
}

namespace common {

template<typename T>
class OptionalInfo : virtual public Info
{
public:
	OptionalInfo() : Info(), containerPointer(nullptr) {}

	void __optional_info__() const {} // dummy member used just for detection of an OpionalInfo object

protected:
	void setContainerPointer(mesh::OptionalComponentsVector<T>* cp) { containerPointer = cp; }
	mesh::OptionalComponentsVector<T>* containerPointer;
};

/**
 * Detector to check if a class has the method v(int)
 */

template<typename T>
using hasOptionalInfo_t = decltype(std::declval<const T&>().__optional_info__());

template<typename T>
using hasOptionalInfo = typename detector<hasOptionalInfo_t, void, T>::type;

} // namespace common
} // namespace mgp

#endif // MGP_MESH_COMMON_OPTIONAL_INFO_H
