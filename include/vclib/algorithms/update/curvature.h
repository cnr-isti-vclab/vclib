/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_ALGORITHMS_UPDATE_CURVATURE_H
#define VCL_ALGORITHMS_UPDATE_CURVATURE_H

#include <vclib/mesh/requirements.h>

namespace vcl {

typedef enum {
	VCL_PRINCIPAL_CURVATURE_TAUBIN95
} VCLibPrincipalCurvatureAlgorithm;

template <HasPerVertexPrincipalCurvature MeshType>
void updatePrincipalCurvatureTaubin95(MeshType& m);

template<HasPerVertexPrincipalCurvature MeshType>
void updatePrincipalCurvature(
	MeshType&                        m,
	VCLibPrincipalCurvatureAlgorithm alg = VCL_PRINCIPAL_CURVATURE_TAUBIN95);

} // namespace vcl

#include "curvature.cpp"

#endif // VCL_ALGORITHMS_UPDATE_CURVATURE_H
