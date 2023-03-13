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

#ifndef VCL_ALGORITHM_UPDATE_SCALAR_H
#define VCL_ALGORITHM_UPDATE_SCALAR_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
void setPerVertexScalar(MeshType& m, typename MeshType::VertexType::ScalarType s);

template<FaceMeshConcept MeshType>
void setPerFaceScalar(MeshType& m, typename MeshType::FaceType::ScalarType s);

template<MeshConcept MeshType>
void clampPerVertexScalar(
	MeshType&                                 m,
	typename MeshType::VertexType::ScalarType minS,
	typename MeshType::VertexType::ScalarType maxS);

template<FaceMeshConcept MeshType>
void clampPerFaceScalar(
	MeshType&                               m,
	typename MeshType::FaceType::ScalarType minS,
	typename MeshType::FaceType::ScalarType maxS);

template<MeshConcept MeshType>
void normalizePerVertexScalar(
	MeshType&                                 m,
	typename MeshType::VertexType::ScalarType minS = 0,
	typename MeshType::VertexType::ScalarType maxS = 1);

template<FaceMeshConcept MeshType>
void normalizePerFaceScalar(
	MeshType&                               m,
	typename MeshType::FaceType::ScalarType minS = 0,
	typename MeshType::FaceType::ScalarType maxS = 1);

template<FaceMeshConcept MeshType>
void setPerVertexScalarFromVertexValence(MeshType& m);

template<FaceMeshConcept MeshType>
void setPerFaceScalarFromFaceArea(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureGaussian(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureMean(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureMinValue(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureMaxValue(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureShapeIndex(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexScalarFromPrincipalCurvatureCurvedness(MeshType& m);

} // namespace vcl

#include "scalar.cpp"

#endif // VCL_ALGORITHM_UPDATE_SCALAR_H