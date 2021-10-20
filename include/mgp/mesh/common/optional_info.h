/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMMON_OPTIONAL_INFO_H
#define MGP_MESH_COMMON_OPTIONAL_INFO_H

#include "info.h"

namespace mgp {

namespace mesh {
template<typename T>
class OptionalContainer;
}

namespace common {

template<typename T>
class OptionalInfo : virtual public Info
{
public:
	OptionalInfo() : Info(), containerPointer(nullptr) {}

protected:
	void setContainerPointer(mesh::OptionalContainer<T>* cp) { containerPointer = cp; }
	mesh::OptionalContainer<T>* containerPointer;
};

} // namespace common
} // namespace mgp

#endif // MGP_MESH_COMMON_OPTIONAL_INFO_H
