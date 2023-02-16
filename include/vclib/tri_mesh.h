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

#ifndef VCL_TRI_MESH_H
#define VCL_TRI_MESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl {

template<typename ScalarType>
class TriMeshT;

}

namespace vcl::trimesh {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Face;

template<typename Scalar>
class Vertex :
		public vcl::Vertex<
			TriMeshT<Scalar>,
			vcl::vert::BitFlags,                                            // 4b
			vcl::vert::Coordinate3<Scalar>,                                 // 12 or 24b
			vcl::vert::Normal3<Scalar>,                                     // 12 or 24b
			vcl::vert::Color,                                               // 4b
			vcl::vert::Scalar<Scalar>,                                      // 4 or 8b
			vcl::vert::OptionalAdjacentFaces<Face<Scalar>, Vertex<Scalar>>, // 0b
			vcl::vert::OptionalAdjacentVertices<Vertex<Scalar>>,            // 0b
			vcl::vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar>>,
			vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,            // 0b
			vcl::vert::OptionalMark<Vertex<Scalar>>,                        // 0b
			vcl::vert::CustomComponents<Vertex<Scalar>>>                    // 0b
{
};

template<typename Scalar>
class Face :
		public vcl::Face<
			TriMeshT<Scalar>,
			vcl::face::TriangleBitFlags,                                     // 4b
			vcl::face::TriangleVertexRefs<Vertex<Scalar>>,                   // 24b
			vcl::face::Normal3<Scalar>,                                      // 12 or 24b
			vcl::face::OptionalScalar<Scalar, Face<Scalar>>,                 // 0b
			vcl::face::OptionalColor<Face<Scalar>>,                          // 0b
			vcl::face::OptionalAdjacentTriangles<Face<Scalar>>,              // 0b
			vcl::face::OptionalTriangleWedgeTexCoords<Scalar, Face<Scalar>>, // 0b
			vcl::face::OptionalMark<Face<Scalar>>,                           // 0b
			vcl::face::CustomComponents<Face<Scalar>>>                       // 0b
{
};

} // namespace vcl::trimesh

namespace vcl {

template<typename ScalarType = double>
class TriMeshT :
		public vcl::Mesh<
			mesh::VertexContainer<trimesh::Vertex<ScalarType>>,
			mesh::FaceContainer<trimesh::Face<ScalarType>>,
			mesh::BoundingBox3<ScalarType>,
			mesh::Color,
			mesh::Mark,
			mesh::Name,
			mesh::TexturePaths,
			mesh::TransformMatrix<ScalarType>,
			mesh::CustomComponents>
{
};

using TriMeshf = TriMeshT<float>;
using TriMesh  = TriMeshT<double>;

// makes sure that the TriMesh satisfies Mesh concepts
static_assert(MeshConcept<TriMesh>, "The TriMesh is not a valid Mesh.");
static_assert(FaceMeshConcept<TriMesh>, "The TriMesh is not a valid Mesh with Faces.");
static_assert(TriangleMeshConcept<TriMesh>, "The TriMesh is not a static Triangle Mesh.");
static_assert(!QuadMeshConcept<TriMesh>, "The TriMesh is a static Quad Mesh.");
static_assert(!EdgeMeshConcept<TriMesh>, "The TriMesh is an Edge Mesh.");
static_assert(!PolygonMeshConcept<TriMesh>, "The TriMesh is a valid Polygon Mesh.");
static_assert(!DcelMeshConcept<TriMesh>, "The TriMesh is a Dcel Mesh.");

} // namespace vcl

#endif // VCL_TRI_MESH_H
