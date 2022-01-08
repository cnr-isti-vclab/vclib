/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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

#ifndef VCL_MESH_VERTICAL_COMPONENTS_VECTOR_H
#define VCL_MESH_VERTICAL_COMPONENTS_VECTOR_H

#include "custom_components_vector.h"
#include "adjacent_edges_vector.h"
#include "adjacent_faces_vector.h"
#include "adjacent_vertices_vector.h"
#include "color_vector.h"
#include "mark_vector.h"
#include "normal_vector.h"
#include "principal_curvature_vector.h"
#include "scalar_vector.h"
#include "tex_coord_vector.h"
#include "wedge_colors_vector.h"
#include "wedge_tex_coords_vector.h"

namespace vcl::internal {

template<typename T>
class VerticalComponentsVector :
		public AdjacentEdgesVector<T>,
		public AdjacentFacesVector<T>,
		public AdjacentVerticesVector<T>,
		public ColorVector<T>,
		public MarkVector<T>,
		public NormalVector<T>,
		public PrincipalCurvatureVector<T>,
		public ScalarVector<T>,
		public TexCoordVector<T>,
		public WedgeColorsVector<T>,
		public WedgeTexCoordsVector<T>,
		public CustomComponentsVector<T>
{
public:
	void clear()
	{
		AdjacentEdgesVector<T>::clear();
		AdjacentFacesVector<T>::clear();
		AdjacentVerticesVector<T>::clear();
		ColorVector<T>::clear();
		MarkVector<T>::clear();
		NormalVector<T>::clear();
		PrincipalCurvatureVector<T>::clear();
		ScalarVector<T>::clear();
		TexCoordVector<T>::clear();
		WedgeColorsVector<T>::clear();
		WedgeTexCoordsVector<T>::clear();
		CustomComponentsVector<T>::clear();
	}
	void resize(uint size)
	{
		AdjacentEdgesVector<T>::resize(size);
		AdjacentFacesVector<T>::resize(size);
		AdjacentVerticesVector<T>::resize(size);
		ColorVector<T>::resize(size);
		MarkVector<T>::resize(size);
		NormalVector<T>::resize(size);
		PrincipalCurvatureVector<T>::resize(size);
		ScalarVector<T>::resize(size);
		TexCoordVector<T>::resize(size);
		WedgeColorsVector<T>::resize(size);
		WedgeTexCoordsVector<T>::resize(size);
		CustomComponentsVector<T>::resize(size);
	}
	void reserve(uint size)
	{
		AdjacentEdgesVector<T>::reserve(size);
		AdjacentFacesVector<T>::reserve(size);
		AdjacentVerticesVector<T>::reserve(size);
		ColorVector<T>::reserve(size);
		MarkVector<T>::reserve(size);
		NormalVector<T>::reserve(size);
		PrincipalCurvatureVector<T>::reserve(size);
		ScalarVector<T>::reserve(size);
		TexCoordVector<T>::reserve(size);
		WedgeColorsVector<T>::reserve(size);
		WedgeTexCoordsVector<T>::reserve(size);
		CustomComponentsVector<T>::reserve(size);
	}
	void compact(const std::vector<int>& newIndices)
	{
		AdjacentEdgesVector<T>::compact(newIndices);
		AdjacentFacesVector<T>::compact(newIndices);
		AdjacentVerticesVector<T>::compact(newIndices);
		ColorVector<T>::compact(newIndices);
		MarkVector<T>::compact(newIndices);
		NormalVector<T>::compact(newIndices);
		PrincipalCurvatureVector<T>::compact(newIndices);
		ScalarVector<T>::compact(newIndices);
		TexCoordVector<T>::compact(newIndices);
		WedgeColorsVector<T>::compact(newIndices);
		WedgeTexCoordsVector<T>::compact(newIndices);
		CustomComponentsVector<T>::compact(newIndices);
	}
};

} // namespace vcl::internal

#endif // VCL_MESH_VERTICAL_COMPONENTS_VECTOR_H
