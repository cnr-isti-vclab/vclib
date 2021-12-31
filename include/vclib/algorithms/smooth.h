/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include <vclib/misc/types.h>

namespace vcl {

template<typename Scalar>
class KDTree;

template<typename MeshType>
void laplacianSmoothing(
	MeshType& m,
	uint      step,
	bool      smoothSelected  = false,
	bool      cotangentWeight = false /*, vcl::CallBackPos *cb = 0*/);

template<typename MeshType>
void taubinSmoothing(
	MeshType& m,
	uint      step,
	float     lambda,
	float     mu,
	bool      smoothSelected  = false/*, vcl::CallBackPos *cb = 0*/);

template<typename MeshType>
void smoothPerVertexNormalsPointCloud(MeshType& m, uint neighborNum, uint iterNum);

template<typename MeshType, typename Scalar>
void smoothPerVertexNormalsPointCloud(
	MeshType&             m,
	const KDTree<Scalar>& tree,
	uint                  neighborNum,
	uint                  iterNum);

} // namespace vcl

#include "smooth.cpp"

#endif // VCL_ALGORITHMS_SMOOTH_H
