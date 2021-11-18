/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_ELEMENT_CONTAINER_H
#define MGP_MESH_CONTAINER_ELEMENT_CONTAINER_H

#include <vector>
#include "../components_optional/optional_info.h"
#include "../components_vector/components_vector.h"

namespace mgp::mesh {

template <typename T>
class ElementContainer
{
public:
	ElementContainer() {};

protected:
	std::vector<T> vec;
	internal::ComponentsVector<T> optionalVec;
};

}

#endif // MGP_MESH_CONTAINER_ELEMENT_CONTAINER_H
