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

#ifndef VCL_ALGORITHMS_STAT_H
#define VCL_ALGORITHMS_STAT_H

#include <utility>

namespace vcl {

template<typename MeshType>
std::pair<typename MeshType::VertexType::ScalarType, typename MeshType::VertexType::ScalarType>
perVertexScalarMinMax(const MeshType& m);

template<typename MeshType>
std::pair<typename MeshType::FaceType::ScalarType, typename MeshType::FaceType::ScalarType>
perFaceScalarMinMax(const MeshType& m);

template<typename MeshType>
typename MeshType::VertexType::ScalarType perVertexScalarAverage(const MeshType& m);

template<typename MeshType>
typename MeshType::FaceType::ScalarType perFaceScalarAverage(const MeshType& m);

template<typename MeshType>
typename MeshType::VertexType::CoordType barycenter(const MeshType& m);

template<typename MeshType>
typename MeshType::VertexType::CoordType scalarWeightedBarycenter(const MeshType& m);

} // namespace vcl

#include "stat.cpp"

#endif // VCL_ALGORITHMS_STAT_H
