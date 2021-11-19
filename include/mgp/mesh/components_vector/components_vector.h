/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VECTOR_H
#define MGP_MESH_COMPONENTS_VECTOR_H

#include "custom_components_vector.h"
#include "optional_adjacent_faces_vector.h"
#include "optional_adjacent_vertices_vector.h"
#include "optional_color_vector.h"
#include "optional_mark_vector.h"
#include "optional_mutable_bit_flags_vector.h"
#include "optional_normal_vector.h"
#include "optional_scalar_vector.h"
#include "optional_tex_coord_vector.h"
#include "optional_wedge_tex_coords_vector.h"

namespace mgp::internal {

template<typename T>
class ComponentsVector :
		public OptionalAdjacentFacesVector<T>,
		public OptionalAdjacentVerticesVector<T>,
		public OptionalColorVector<T>,
		public OptionalMarkVector<T>,
		public OptionalMutableBitFlagsVector<T>,
		public OptionalNormalVector<T>,
		public OptionalScalarVector<T>,
		public OptionalTexCoordVector<T>,
		public OptionalWedgeTexCoordsVector<T>,
		public CustomComponentsVector<T>
{
public:
	void clear()
	{
		OptionalAdjacentFacesVector<T>::clear();
		OptionalAdjacentVerticesVector<T>::clear();
		OptionalColorVector<T>::clear();
		OptionalMarkVector<T>::clear();
		OptionalMutableBitFlagsVector<T>::clear();
		OptionalNormalVector<T>::clear();
		OptionalScalarVector<T>::clear();
		OptionalTexCoordVector<T>::clear();
		OptionalWedgeTexCoordsVector<T>::clear();
		CustomComponentsVector<T>::clear();
	}
	void resize(unsigned int size)
	{
		OptionalAdjacentFacesVector<T>::resize(size);
		OptionalAdjacentVerticesVector<T>::resize(size);
		OptionalColorVector<T>::resize(size);
		OptionalMarkVector<T>::resize(size);
		OptionalMutableBitFlagsVector<T>::resize(size);
		OptionalNormalVector<T>::resize(size);
		OptionalScalarVector<T>::resize(size);
		OptionalTexCoordVector<T>::resize(size);
		OptionalWedgeTexCoordsVector<T>::resize(size);
		CustomComponentsVector<T>::resize(size);
	}
	void reserve(unsigned int size)
	{
		OptionalAdjacentFacesVector<T>::resize(size);
		OptionalAdjacentVerticesVector<T>::reserve(size);
		OptionalColorVector<T>::reserve(size);
		OptionalMarkVector<T>::reserve(size);
		OptionalMutableBitFlagsVector<T>::reserve(size);
		OptionalNormalVector<T>::reserve(size);
		OptionalScalarVector<T>::reserve(size);
		OptionalTexCoordVector<T>::reserve(size);
		OptionalWedgeTexCoordsVector<T>::reserve(size);
		CustomComponentsVector<T>::reserve(size);
	}
	void compact(const std::vector<int>& newIndices)
	{
		OptionalAdjacentFacesVector<T>::compact(newIndices);
		OptionalAdjacentVerticesVector<T>::compact(newIndices);
		OptionalColorVector<T>::compact(newIndices);
		OptionalMarkVector<T>::compact(newIndices);
		OptionalMutableBitFlagsVector<T>::compact(newIndices);
		OptionalNormalVector<T>::compact(newIndices);
		OptionalScalarVector<T>::compact(newIndices);
		OptionalTexCoordVector<T>::compact(newIndices);
		OptionalWedgeTexCoordsVector<T>::compact(newIndices);
		CustomComponentsVector<T>::compact(newIndices);
	}
};

} // namespace mgp::internal

#endif // MGP_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
