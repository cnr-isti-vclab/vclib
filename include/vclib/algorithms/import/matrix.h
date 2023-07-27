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

#ifndef VCL_ALGORITHMS_IMPORT_MATRIX_H
#define VCL_ALGORITHMS_IMPORT_MATRIX_H

#include <vclib/concepts/space/matrix.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType, MatrixConcept VMatrix, MatrixConcept VNMatrix>
MeshType pointCloudMeshFromMatrices(
	const VMatrix&  vertices,
	const VNMatrix& vertexNormals = VNMatrix());

template<
	FaceMeshConcept MeshType,
	MatrixConcept   VMatrix,
	MatrixConcept   FMatrix,
	MatrixConcept   VNMatrix>
MeshType meshFromMatrices(
	const VMatrix&  vertices,
	const FMatrix&  faces = FMatrix(),
	const VNMatrix& vertexNormals = VNMatrix());

} // namespace vcl

#include "matrix.cpp"

#endif // VCL_ALGORITHMS_IMPORT_MATRIX_H
