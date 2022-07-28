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

#ifndef VCL_POINT_CLOUD_H
#define VCL_POINT_CLOUD_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl::pointcloud {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Vertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,                                 // 4b
			vcl::vert::Coordinate3<Scalar>,                      // 24b
			vcl::vert::Normal3<Scalar>,                          // 24b
			vcl::vert::Color,                                    // 4b
			vcl::vert::Scalar<Scalar>,                           // 8b
			vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>, // 0b
			vcl::vert::OptionalMark<Vertex<Scalar>>,             // 0b
			vcl::vert::CustomComponents<Vertex<Scalar>>>         // 0b
{
};

} // namespace vcl::pointcloud

namespace vcl {

template<typename ScalarType = double>
class PointCloudT :
		public vcl::Mesh<
			mesh::VertexContainer<pointcloud::Vertex<ScalarType>>,
			mesh::BoundingBox3<ScalarType>,
			mesh::Mark,
			mesh::TexturePaths,
			mesh::TransformMatrix<ScalarType>>
{
};

using PointCloudf = PointCloudT<float>;
using PointCloud  = PointCloudT<double>;

} // namespace vcl

#endif // VCL_POINT_CLOUD_H
