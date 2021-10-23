/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
#define MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H

#include "optional_color_vector.h"
#include "optional_mutable_bit_flags_vector.h"
#include "optional_normal_vector.h"
#include "optional_scalar_vector.h"

namespace mgp::mesh {

template<typename T>
class OptionalComponentsVector :
		public OptionalMutableBitFlagsVector<T>,
		public OptionalColorVector<T>,
		public OptionalNormalVector<T>,
		public OptionalScalarVector<T>
{
public:
	void resize(unsigned int size)
	{
		OptionalMutableBitFlagsVector<T>::resize(size);
		OptionalColorVector<T>::resize(size);
		OptionalNormalVector<T>::resize(size);
		OptionalScalarVector<T>::resize(size);
	}
	void reserve(unsigned int size)
	{
		OptionalMutableBitFlagsVector<T>::reserve(size);
		OptionalColorVector<T>::reserve(size);
		OptionalNormalVector<T>::reserve(size);
		OptionalScalarVector<T>::reserve(size);
	}
};

} // namespace mgp::mesh

#endif // MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
