/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
#define MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H

#include "optional_color_vector.h"

namespace mgp {
namespace mesh {

template<typename T>
class OptionalComponentsVector : public OptionalColorVector<T>
{
};

} // namespace mesh
} // namespace mgp

#endif // MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
