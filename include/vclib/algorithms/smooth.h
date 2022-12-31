/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_ALGORITHMS_SMOOTH_H
#define VCL_ALGORITHMS_SMOOTH_H

#include <vclib/mesh/requirements.h>

namespace vcl {

template<PointConcept PointType>
class KDTree;

template<FaceMeshConcept MeshType>
void laplacianSmoothing(
	MeshType& m,
	uint      step,
	bool      smoothSelected  = false,
	bool      cotangentWeight = false /*, vcl::CallBackPos *cb = 0*/);

template<FaceMeshConcept MeshType>
void taubinSmoothing(
	MeshType& m,
	uint      step,
	float     lambda,
	float     mu,
	bool      smoothSelected  = false/*, vcl::CallBackPos *cb = 0*/);

template<MeshConcept MeshType>
void smoothPerVertexNormalsPointCloud(MeshType& m, uint neighborNum, uint iterNum);

template<MeshConcept MeshType, PointConcept PointType>
void smoothPerVertexNormalsPointCloud(
	MeshType&                m,
	const KDTree<PointType>& tree,
	uint                     neighborNum,
	uint                     iterNum);

} // namespace vcl

#include "smooth.cpp"

#endif // VCL_ALGORITHMS_SMOOTH_H
