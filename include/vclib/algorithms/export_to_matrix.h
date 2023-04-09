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

#ifndef VCL_ALGORITHMS_EXPORT_TO_MATRIX_H
#define VCL_ALGORITHMS_EXPORT_TO_MATRIX_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<typename Matrix, MeshConcept MeshType>
Matrix vertexMatrix(const MeshType& m);

template<typename Matrix, FaceMeshConcept MeshType>
Matrix faceMatrix(const MeshType& m);

template<typename Matrix, EdgeMeshConcept MeshType>
Matrix edgeMatrix(const MeshType& m);

template<typename Matrix, MeshConcept MeshType>
Matrix vertexNormalsMatrix(const MeshType& m) requires HasPerVertexNormal<MeshType>;

} // namespace vcl

#include "export_to_matrix.cpp"

#endif // VCL_ALGORITHMS_EXPORT_TO_MATRIX_H
