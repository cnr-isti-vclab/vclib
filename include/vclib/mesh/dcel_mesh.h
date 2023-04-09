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

#ifndef VCL_MESH_DCEL_MESH_H
#define VCL_MESH_DCEL_MESH_H

#include "mesh/mesh.h"
#include "requirements.h"

namespace vcl {

template<typename ScalarType>
class DcelMeshT;

}

namespace vcl::dcel {

template<typename Scalar>
class HalfEdge;

template<typename Scalar>
class Vertex;

template<typename Scalar>
class Face;

template<typename Scalar>
class HalfEdge :
		public vcl::HalfEdge<
			DcelMeshT<Scalar>,
			vcl::hedge::BitFlags,
			vcl::hedge::HalfEdgeReferences<HalfEdge<Scalar>, Vertex<Scalar>, Face<Scalar>>,
			vcl::hedge::OptionalScalar<Scalar, HalfEdge<Scalar>>,
			vcl::hedge::OptionalColor<HalfEdge<Scalar>>,
			vcl::hedge::OptionalMark<HalfEdge<Scalar>>,
			vcl::hedge::OptionalTexCoord<Scalar, HalfEdge<Scalar>>,
			vcl::hedge::CustomComponents<HalfEdge<Scalar>>>
{
};

template<typename Scalar>
class Vertex :
		public vcl::Vertex<
			DcelMeshT<Scalar>,
			vcl::vert::BitFlags,
			vcl::vert::Coordinate3<Scalar>,
			vcl::vert::Normal3<Scalar>,
			vcl::vert::Color,
			vcl::vert::Scalar<Scalar>,
			vcl::vert::HalfEdgeReference<HalfEdge<Scalar>>,
			vcl::vert::OptionalTexCoord<Scalar, Vertex<Scalar>>,
			vcl::vert::OptionalMark<Vertex<Scalar>>,
			vcl::vert::CustomComponents<Vertex<Scalar>>>
{
};

template<typename Scalar>
class Face :
		public vcl::Face<
			DcelMeshT<Scalar>,
			vcl::face::BitFlags,
			vcl::face::HalfEdgeReference<HalfEdge<Scalar>>,
			vcl::face::Normal3<Scalar>,
			vcl::face::OptionalScalar<Scalar, Face<Scalar>>,
			vcl::face::OptionalColor<Face<Scalar>>,
			vcl::face::OptionalMark<Face<Scalar>>,
			vcl::face::CustomComponents<Face<Scalar>>>
{
};

} // namespace vcl::dcel

namespace vcl {

template<typename ScalarType = double>
class DcelMeshT :
		public vcl::Mesh<
			mesh::VertexContainer<dcel::Vertex<ScalarType>>,
			mesh::FaceContainer<dcel::Face<ScalarType>>,
			mesh::HalfEdgeContainer<dcel::HalfEdge<ScalarType>>,
			mesh::BoundingBox3<ScalarType>,
			mesh::Mark,
			mesh::Name,
			mesh::TexturePaths,
			mesh::TransformMatrix<ScalarType>,
			mesh::CustomComponents>
{
};

using DcelMeshf = DcelMeshT<float>;
using DcelMesh  = DcelMeshT<double>;

} // namespace vcl

#endif // VCL_MESH_DCEL_MESH_H
