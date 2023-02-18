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

#ifndef VCL_ALGORITHMS_GENERATE_H
#define VCL_ALGORITHMS_GENERATE_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
std::vector<bool> boolVectorFromVertexSelection(const MeshType& m);

template<FaceMeshConcept MeshType>
std::vector<bool> boolVectorFromFaceSelection(const MeshType& m);

template<MeshConcept InMeshType, MeshConcept OutMeshType = InMeshType>
OutMeshType generateMeshFromVertexBoolVector(
	const InMeshType& m,
	std::vector<bool>& vec,
	bool saveBirthVertexInCustomComponent = true);

} // namespace vcl

#include "generate.cpp"

#endif // VCL_ALGORITHMS_GENERATE_H
