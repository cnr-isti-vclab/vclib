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

#ifndef VCL_ALGORITHMS_STAT_QUALITY_H
#define VCL_ALGORITHMS_STAT_QUALITY_H

#include <vclib/math/histogram.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

template<MeshConcept MeshType>
std::pair<typename MeshType::VertexType::QualityType, typename MeshType::VertexType::QualityType>
vertexQualityMinMax(const MeshType& m);

template<FaceMeshConcept MeshType>
std::pair<typename MeshType::FaceType::QualityType, typename MeshType::FaceType::QualityType>
faceQualityMinMax(const MeshType& m);

template<MeshConcept MeshType>
typename MeshType::VertexType::QualityType vertexQualityAverage(const MeshType& m);

template<FaceMeshConcept MeshType>
typename MeshType::FaceType::QualityType faceQualityAverage(const MeshType& m);

template<MeshConcept MeshType>
std::vector<typename MeshType::VertexType::QualityType> vertexRadiusFromQuality(
	const MeshType& m,
	double          diskRadius,
	double          radiusVariance,
	bool            invert = false);

template<MeshConcept MeshType, typename HScalar = double>
Histogram<HScalar> vertexQualityHistogram(
	const MeshType& m,
	bool selectionOnly = false,
	uint histSize = 10000);

template<FaceMeshConcept MeshType, typename HScalar = double>
Histogram<HScalar> faceQualityHistogram(
	const MeshType& m,
	bool selectionOnly = false,
	uint histSize = 10000);

} // namespace vcl

#include "quality.cpp"

#endif // VCL_ALGORITHMS_STAT_QUALITY_H