/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_VECTOR_H
#define VCL_MESH_COMPONENTS_VECTOR_H

#include "custom_components_vector.h"
#include "optional_adjacent_faces_vector.h"
#include "optional_adjacent_vertices_vector.h"
#include "optional_color_vector.h"
#include "optional_mark_vector.h"
#include "optional_mutable_bit_flags_vector.h"
#include "optional_normal_vector.h"
#include "optional_principal_curvature_vector.h"
#include "optional_scalar_vector.h"
#include "optional_tex_coord_vector.h"
#include "optional_wedge_colors_vector.h"
#include "optional_wedge_tex_coords_vector.h"

namespace vcl::internal {

template<typename T>
class ComponentsVector :
		public OptionalAdjacentFacesVector<T>,
		public OptionalAdjacentVerticesVector<T>,
		public OptionalColorVector<T>,
		public OptionalMarkVector<T>,
		public OptionalMutableBitFlagsVector<T>,
		public OptionalNormalVector<T>,
		public OptionalPrincipalCurvatureVector<T>,
		public OptionalScalarVector<T>,
		public OptionalTexCoordVector<T>,
		public OptionalWedgeColorsVector<T>,
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
		OptionalPrincipalCurvatureVector<T>::clear();
		OptionalScalarVector<T>::clear();
		OptionalTexCoordVector<T>::clear();
		OptionalWedgeColorsVector<T>::clear();
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
		OptionalPrincipalCurvatureVector<T>::resize(size);
		OptionalScalarVector<T>::resize(size);
		OptionalTexCoordVector<T>::resize(size);
		OptionalWedgeColorsVector<T>::resize(size);
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
		OptionalPrincipalCurvatureVector<T>::reserve(size);
		OptionalScalarVector<T>::reserve(size);
		OptionalTexCoordVector<T>::reserve(size);
		OptionalWedgeColorsVector<T>::reserve(size);
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
		OptionalPrincipalCurvatureVector<T>::compact(newIndices);
		OptionalScalarVector<T>::compact(newIndices);
		OptionalTexCoordVector<T>::compact(newIndices);
		OptionalWedgeColorsVector<T>::compact(newIndices);
		OptionalWedgeTexCoordsVector<T>::compact(newIndices);
		CustomComponentsVector<T>::compact(newIndices);
	}
};

} // namespace vcl::internal

#endif // VCL_MESH_CONTAINER_OPTIONAL_COMPONENTS_VECTOR_H
