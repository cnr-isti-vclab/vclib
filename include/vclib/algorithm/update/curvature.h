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

#ifndef VCL_ALGORITHM_UPDATE_CURVATURE_H
#define VCL_ALGORITHM_UPDATE_CURVATURE_H

#include <vclib/mesh/requirements.h>
#include <vclib/misc/logger.h>

namespace vcl {

typedef enum {
	VCL_PRINCIPAL_CURVATURE_TAUBIN95,
	VCL_PRINCIPAL_CURVATURE_PCA
} VCLibPrincipalCurvatureAlgorithm;

template <FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvatureTaubin95(MeshType& m, LogType& log = nullLogger);

template<
	FaceMeshConcept MeshType,
	LoggerConcept LogType = NullLogger>
void updatePrincipalCurvaturePCA(
	MeshType&  m,
	typename MeshType::VertexType::ScalarType radius,
	bool       montecarloSampling = true,
	LogType&   log                = nullLogger);

template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvature(MeshType& m, LogType& log = nullLogger);

template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
void updatePrincipalCurvature(
	MeshType&                        m,
	VCLibPrincipalCurvatureAlgorithm alg = VCL_PRINCIPAL_CURVATURE_TAUBIN95,
	LogType&                         log = nullLogger);

} // namespace vcl

#include "curvature.cpp"

#endif // VCL_ALGORITHM_UPDATE_CURVATURE_H
