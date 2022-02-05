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

#ifndef VCL_POLY_MESH_H
#define VCL_POLY_MESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl::polymesh {

class Vertex;
class Face;

class Vertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,
			vcl::vert::Coordinate3d,
			vcl::vert::Normal3f,
			vcl::vert::Color,
			vcl::vert::Scalard,
			vcl::vert::OptionalTexCoordf<Vertex>,
			vcl::vert::OptionalAdjacentFaces<Face, Vertex>,
			vcl::vert::OptionalAdjacentVertices<Vertex>,
			vcl::vert::OptionalPrincipalCurvatured<Vertex>,
			vcl::vert::CustomComponents<Vertex>>
{
};

class Face :
		public vcl::Face<
			vcl::face::PolygonBitFlags, // 4b
			vcl::face::PolygonVertexRefs<Vertex>,
			vcl::face::Normal3f,
			vcl::face::OptionalColor<Face>,
			vcl::face::OptionalScalard<Face>,
			vcl::face::OptionalAdjacentPolygons<Face>,
			vcl::face::OptionalPolygonWedgeTexCoordsf<Face>,
			vcl::face::CustomComponents<Face>>
{
};

} // namespace vcl::polymesh

namespace vcl {

class PolyMesh :
		public vcl::Mesh<
			mesh::VertexContainer<polymesh::Vertex>,
			mesh::FaceContainer<polymesh::Face>,
			mesh::BoundingBox3d,
			mesh::Mark,
			mesh::TextureFileNames,
			mesh::TransformMatrixd>
{
};

} // namespace vcl

#endif // VCL_POLY_MESH_H
