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

#ifndef VCL_ALGORITHMS_EXPORT_MATRIX_H
#define VCL_ALGORITHMS_EXPORT_MATRIX_H

#include <vclib/concepts/space/matrix.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexMatrix(const MeshType& mesh);

template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceMatrix(const MeshType& mesh);

template<typename Vect, FaceMeshConcept MeshType>
Vect faceSizesVector(const MeshType& mesh);

template<MatrixConcept Matrix, EdgeMeshConcept MeshType>
Matrix edgeMatrix(const MeshType& mesh);

template<uint EL_TYPE, typename Vect, MeshConcept MeshType>
Vect elementSelectionVector(const MeshType& mesh);

template<typename Vect, MeshConcept MeshType>
Vect vertexSelectionVector(const MeshType& mesh);

template<typename Vect, FaceMeshConcept MeshType>
Vect faceSelectionVector(const MeshType& mesh);

template<uint EL_TYPE, MatrixConcept Matrix, MeshConcept MeshType>
Matrix elementNormalsMatrix(const MeshType& mesh);

template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexNormalsMatrix(const MeshType& mesh);

template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceNormalsMatrix(const MeshType& mesh);

template<uint EL_TYPE, MatrixConcept Matrix, MeshConcept MeshType>
Matrix elementColorsMatrix(const MeshType& mesh);

template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexColorsMatrix(const MeshType& mesh);

template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceColorsMatrix(const MeshType& mesh);

template<uint EL_TYPE, typename Vect, MeshConcept MeshType>
Vect elementQualityVector(const MeshType& mesh);

template<typename Vect, MeshConcept MeshType>
Vect vertexQualityVector(const MeshType& mesh);

template<typename Vect, FaceMeshConcept MeshType>
Vect faceQualityVector(const MeshType& mesh);

} // namespace vcl

#include "matrix.cpp"

#endif // VCL_ALGORITHMS_EXPORT_MATRIX_H
