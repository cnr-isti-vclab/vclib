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

#ifndef VCL_MESH_POINT_CLOUD_H
#define VCL_MESH_POINT_CLOUD_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl {

template<typename ScalarType>
class PointCloudT;

}

namespace vcl::pointcloud {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Vertex :
		public vcl::Vertex<
			PointCloudT<Scalar>,
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
			mesh::Name,
			mesh::TexturePaths,
			mesh::TransformMatrix<ScalarType>,
			mesh::CustomComponents>
{
};

using PointCloudf = PointCloudT<float>;
using PointCloud  = PointCloudT<double>;

// makes sure that the PointCloud satisfies Mesh concepts
static_assert(MeshConcept<PointCloud>, "The PointCloud is not a valid Mesh.");
static_assert(!FaceMeshConcept<PointCloud>, "The PointCloud is a valid Mesh with Faces.");
static_assert(!TriangleMeshConcept<PointCloud>, "The PointCloud is a static Triangle Mesh.");
static_assert(!QuadMeshConcept<PointCloud>, "The PointCloud is a static Quad Mesh.");
static_assert(!EdgeMeshConcept<PointCloud>, "The PointCloud is an Edge Mesh.");
static_assert(!PolygonMeshConcept<PointCloud>, "The PointCloud is a PolygonMesh.");
static_assert(!DcelMeshConcept<PointCloud>, "The PointCloud is a DcelMesh.");

} // namespace vcl

#endif // VCL_MESH_POINT_CLOUD_H
