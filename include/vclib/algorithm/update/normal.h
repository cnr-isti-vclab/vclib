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

#ifndef VCL_ALGORITHM_UPDATE_NORMAL_H
#define VCL_ALGORITHM_UPDATE_NORMAL_H

#include <vclib/math/matrix.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<FaceMeshConcept MeshType>
void normalizePerFaceNormals(MeshType& m);

template<FaceMeshConcept MeshType>
void updatePerFaceNormals(MeshType& m, bool normalize = true);

template<MeshConcept MeshType>
void clearPerVertexNormals(MeshType& m);

template<FaceMeshConcept MeshType>
void clearPerReferencedVertexNormals(MeshType& m);

template<MeshConcept MeshType>
void normalizePerVertexNormals(MeshType& m);

template<FaceMeshConcept MeshType>
void updatePerVertexNormals(MeshType& m, bool normalize = true);

template<FaceMeshConcept MeshType>
void updatePerVertexNormalsFromFaceNormals(MeshType& m, bool normalize = true);

template<FaceMeshConcept MeshType>
void updatePerVertexNormalsAngleWeighted(MeshType& m, bool normalize = true);

template<FaceMeshConcept MeshType>
void updatePerVertexNormalsNelsonMaxWeighted(MeshType& m, bool normalize = true);

template<FaceMeshConcept MeshType, typename MScalar>
void multiplyPerFaceNormalsByMatrix(
	MeshType&                     mesh,
	const vcl::Matrix44<MScalar>& mat,
	bool                          removeScalingFromMatrix = true);

template<MeshConcept MeshType, typename MScalar>
void multiplyPerVertexNormalsByMatrix(
	MeshType&                     mesh,
	const vcl::Matrix44<MScalar>& mat,
	bool                          removeScalingFromMatrix = true);

} // namespace vcl

#include "normal.cpp"

#endif // VCL_ALGORITHM_UPDATE_NORMAL_H
