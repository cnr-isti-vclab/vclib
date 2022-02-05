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

class Vertex;

class Vertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,                  // 4b
			vcl::vert::Coordinate3d,              // 24b
			vcl::vert::Normal3d,                  // 24b
			vcl::vert::Color,                     // 4b
			vcl::vert::Scalard,                   // 8b
			vcl::vert::OptionalTexCoordf<Vertex>, // 0b
			vcl::vert::OptionalMark<Vertex>,      // 0b
			vcl::vert::CustomComponents<Vertex>>  // 0b
{
};

} // namespace vcl::pointcloud

namespace vcl {

class PointCloud :
		public vcl::Mesh<
			mesh::VertexContainer<pointcloud::Vertex>,
			mesh::BoundingBox3d,
			mesh::Mark,
			mesh::TextureFileNames,
			mesh::TransformMatrixd>
{
};

} // namespace vcl

#endif // VCL_POINT_CLOUD_H
