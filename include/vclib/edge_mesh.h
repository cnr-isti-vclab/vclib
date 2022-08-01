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

#ifndef VCL_EDGE_MESH_H
#define VCL_EDGE_MESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl::edgemesh {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Edge;

template<typename Scalar>
class Vertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,                                            // 4b
			vcl::vert::Coordinate3<Scalar>,                                 // 24b
			vcl::vert::Normal3<Scalar>,                                     // 24b
			vcl::vert::Color,                                               // 4b
			vcl::vert::Scalar<Scalar>,                                      // 8b
			vcl::vert::OptionalAdjacentEdges<Edge<Scalar>, Vertex<Scalar>>, // 0b
			vcl::vert::OptionalAdjacentVertices<Vertex<Scalar>>,            // 0b
			vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,            // 0b
			vcl::vert::OptionalMark<Vertex<Scalar>>,                        // 0b
			vcl::vert::CustomComponents<Vertex<Scalar>>>                    // 0b
{
};

template<typename Scalar>
class Edge :
		public vcl::Edge<
			vcl::edge::BitFlags,                             // 4b
			vcl::edge::VertexReferences<Vertex<Scalar>>,     // 24b
			vcl::edge::OptionalScalar<Scalar, Edge<Scalar>>, // 0b
			vcl::edge::OptionalColor<Edge<Scalar>>,          // 0b
			vcl::edge::OptionalAdjacentEdges<Edge<Scalar>>,  // 0b
			vcl::edge::OptionalMark<Edge<Scalar>>,           // 0b
			vcl::edge::CustomComponents<Edge<Scalar>>>       // 0b
{
};

} // namespace vcl::trimesh

namespace vcl {

template<typename ScalarType = double>
class EdgeMeshT :
		public vcl::Mesh<
			mesh::VertexContainer<edgemesh::Vertex<ScalarType>>,
			mesh::EdgeContainer<edgemesh::Edge<ScalarType>>,
			mesh::BoundingBox3<ScalarType>,
			mesh::Mark,
			mesh::TexturePaths,
			mesh::TransformMatrix<ScalarType>>
{
};

using EdgeMeshf = EdgeMeshT<float>;
using EdgeMesh  = EdgeMeshT<double>;

// makes sure that the EdgeMesh satisfies Mesh concepts
static_assert(MeshConcept<EdgeMesh>, "The EdgeMesh is not a valid Mesh.");
static_assert(!FaceMeshConcept<EdgeMesh>, "The EdgeMesh is a valid Mesh with Faces.");
static_assert(!TriangleMeshConcept<EdgeMesh>, "The EdgeMesh is a static Triangle Mesh.");
static_assert(!QuadMeshConcept<EdgeMesh>, "The EdgeMesh is a static Quad Mesh.");
static_assert(EdgeMeshConcept<EdgeMesh>, "The EdgeMesh is not a valid Edge Mesh.");
static_assert(!PolygonMeshConcept<EdgeMesh>, "The EdgeMesh is a PolygonMesh.");
static_assert(!DcelMeshConcept<EdgeMesh>, "The EdgeMesh is a DcelMesh.");

} // namespace vcl

#endif // VCL_EDGE_MESH_H
