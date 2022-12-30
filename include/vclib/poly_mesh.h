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

#ifndef VCL_POLY_MESH_H
#define VCL_POLY_MESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl::polymesh {

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Face;

template<typename Scalar>
class Vertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,
			vcl::vert::Coordinate3<Scalar>,
			vcl::vert::Normal3<Scalar>,
			vcl::vert::Color,
			vcl::vert::Scalar<Scalar>,
			vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
			vcl::vert::OptionalAdjacentFaces<Face<Scalar>, Vertex<Scalar>>,
			vcl::vert::OptionalAdjacentVertices<Vertex<Scalar>>,
			vcl::vert::OptionalPrincipalCurvature<Scalar, Vertex<Scalar>>,
			vcl::vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Face :
		public vcl::Face<
			vcl::face::PolygonBitFlags, // 4b
			vcl::face::PolygonVertexRefs<Vertex<Scalar>>,
			vcl::face::Normal3<Scalar>,
			vcl::face::OptionalColor<Face<Scalar>>,
			vcl::face::OptionalScalar<Scalar, Face<Scalar>>,
			vcl::face::OptionalAdjacentPolygons<Face<Scalar>>,
			vcl::face::OptionalPolygonWedgeTexCoords<Scalar, Face<Scalar>>,
			vcl::face::CustomComponents<Face<Scalar>>>
{
};

} // namespace vcl::polymesh

namespace vcl {

template<typename ScalarType = double>
class PolyMeshT :
		public vcl::Mesh<
			mesh::VertexContainer<polymesh::Vertex<ScalarType>>,
			mesh::FaceContainer<polymesh::Face<ScalarType>>,
			mesh::BoundingBox3<ScalarType>,
			mesh::Color,
			mesh::Mark,
			mesh::Name,
			mesh::TexturePaths,
			mesh::TransformMatrix<ScalarType>>
{
};

using PolyMeshf = PolyMeshT<float>;
using PolyMesh  = PolyMeshT<double>;

// makes sure that the PolyMesh satisfies Mesh concepts
static_assert(MeshConcept<PolyMesh>, "The PolyMesh is not a valid Mesh.");
static_assert(FaceMeshConcept<PolyMesh>, "The PolyMesh is not a valid Mesh with Faces.");
static_assert(!TriangleMeshConcept<PolyMesh>, "The PolyMesh is a static Triangle Mesh.");
static_assert(!QuadMeshConcept<PolyMesh>, "The PolyMesh is a static Quad Mesh.");
static_assert(!EdgeMeshConcept<PolyMesh>, "The PolyMesh is an Edge Mesh.");
static_assert(PolygonMeshConcept<PolyMesh>, "The PolyMesh is not a valid Polygon Mesh.");
static_assert(!DcelMeshConcept<PolyMesh>, "The PolyMesh is a Dcel Mesh.");

} // namespace vcl

#endif // VCL_POLY_MESH_H
