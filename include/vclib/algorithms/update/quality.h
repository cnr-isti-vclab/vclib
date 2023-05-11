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

#ifndef VCL_ALGORITHMS_UPDATE_QUALITY_H
#define VCL_ALGORITHMS_UPDATE_QUALITY_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
void setPerVertexQuality(MeshType& m, typename MeshType::VertexType::QualityType s);

template<FaceMeshConcept MeshType>
void setPerFaceQuality(MeshType& m, typename MeshType::FaceType::QualityType s);

template<MeshConcept MeshType>
void clampPerVertexQuality(
	MeshType&                                 m,
	typename MeshType::VertexType::QualityType minS,
	typename MeshType::VertexType::QualityType maxS);

template<FaceMeshConcept MeshType>
void clampPerFaceQuality(
	MeshType&                               m,
	typename MeshType::FaceType::QualityType minS,
	typename MeshType::FaceType::QualityType maxS);

template<MeshConcept MeshType>
void normalizePerVertexQuality(
	MeshType&                                 m,
	typename MeshType::VertexType::QualityType minS = 0,
	typename MeshType::VertexType::QualityType maxS = 1);

template<FaceMeshConcept MeshType>
void normalizePerFaceQuality(
	MeshType&                               m,
	typename MeshType::FaceType::QualityType minS = 0,
	typename MeshType::FaceType::QualityType maxS = 1);

template<FaceMeshConcept MeshType>
void setPerVertexQualityFromVertexValence(MeshType& m);

template<FaceMeshConcept MeshType>
void setPerFaceQualityFromFaceArea(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureGaussian(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureMean(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureMinValue(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureMaxValue(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureShapeIndex(MeshType& m);

template<MeshConcept MeshType>
void setPerVertexQualityFromPrincipalCurvatureCurvedness(MeshType& m);

} // namespace vcl

#include "quality.cpp"

#endif // VCL_ALGORITHMS_UPDATE_QUALITY_H
