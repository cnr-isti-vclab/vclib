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

#ifndef VCL_ALGORITHMS_SCALAR_H
#define VCL_ALGORITHMS_SCALAR_H

namespace vcl {

template<typename MeshType>
void setPerVertexScalar(MeshType& m, typename MeshType::VertexType::ScalarType s);

template<typename MeshType>
void setPerFaceScalar(MeshType& m, typename MeshType::FaceType::ScalarType s);

template<typename MeshType>
void clampPerVertexScalar(
	MeshType&                                 m,
	typename MeshType::VertexType::ScalarType minS,
	typename MeshType::VertexType::ScalarType maxS);

template<typename MeshType>
void clampPerFaceScalar(
	MeshType&                               m,
	typename MeshType::FaceType::ScalarType minS,
	typename MeshType::FaceType::ScalarType maxS);

template<typename MeshType>
void setPerVertexScalarFromVertexValence(MeshType& m);

} // namespace vcl

#include "scalar.cpp"

#endif // VCL_ALGORITHMS_SCALAR_H
