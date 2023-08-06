/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#ifndef VCL_ALGORITHMS_FILTER_H
#define VCL_ALGORITHMS_FILTER_H

#include <vclib/mesh/requirements.h>
#include <vclib/views.h>

namespace vcl {

template<MeshConcept InMeshType, MeshConcept OutMeshType = InMeshType>
OutMeshType perVertexMeshFilter(
	const InMeshType& m,
	Range auto&& vertexFilterRng,
	bool saveBirthIndicesInCustomComponent = true);

template<FaceMeshConcept InMeshType, FaceMeshConcept OutMeshType = InMeshType>
OutMeshType perFaceMeshFilter(
	const InMeshType& m,
	Range auto&& faceFilterRng,
	bool saveBirthIndicesInCustomComponent = true);

} // namespace vcl

#include "filter.cpp"

#endif // VCL_ALGORITHMS_FILTER_H
