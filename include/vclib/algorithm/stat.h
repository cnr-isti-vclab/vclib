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

#ifndef VCL_ALGORITHM_STAT_H
#define VCL_ALGORITHM_STAT_H

#include <vector>

#include <vclib/math/matrix.h>

#include "stat/bounding_box.h"
#include "stat/scalar.h"
#include "stat/selection.h"

namespace vcl {

template<MeshConcept MeshType>
typename MeshType::VertexType::CoordType barycenter(const MeshType& m);

template<MeshConcept MeshType>
typename MeshType::VertexType::CoordType scalarWeightedBarycenter(const MeshType& m);

template<FaceMeshConcept MeshType>
typename MeshType::VertexType::CoordType shellBarycenter(const MeshType& m);

template<FaceMeshConcept MeshType>
double volume(const MeshType& m);

template<FaceMeshConcept MeshType>
double surfaceArea(const MeshType& m);

template<FaceMeshConcept MeshType>
double borderLength(const MeshType& m);

template<PointConcept PointType>
Matrix33<double> covarianceMatrixOfPointCloud(const std::vector<PointType>& pointVec);

template<MeshConcept MeshType>
Matrix33<double> covarianceMatrixOfPointCloud(const MeshType& m);

template<PointConcept PointType>
Matrix33<double> weightedCovarianceMatrixOfPointCloud(
	const std::vector<PointType>& pointVec,
	const std::vector<typename PointType::ScalarType>& weigths);

template<FaceMeshConcept MeshType>
Matrix33<double> covarianceMatrixOfMesh(const MeshType& m);

template<MeshConcept MeshType, typename ScalarType>
std::vector<ScalarType> vertexRadiusFromWeights(
	const MeshType&                m,
	const std::vector<ScalarType>& weights,
	double                         diskRadius,
	double                         radiusVariance,
	bool                           invert = false);

} // namespace vcl

#include "stat.cpp"

#endif // VCL_ALGORITHM_STAT_H
