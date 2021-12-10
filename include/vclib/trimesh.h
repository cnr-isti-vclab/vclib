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

#ifndef VCL_TRIMESH_H
#define VCL_TRIMESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl::trimesh {

class Vertex;
class Face;

class Vertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,                            // 4b
			vcl::vert::Coordinate3d,                        // 24b
			vcl::vert::Normal3d,                            // 24b
			vcl::vert::Color,                               // 4b
			vcl::vert::Scalard,                             // 8b
			vcl::vert::OptionalTexCoordf<Vertex>,           // 0b
			vcl::vert::OptionalAdjacentFaces<Face, Vertex>, // 0b
			vcl::vert::OptionalAdjacentVertices<Vertex>,    // 0b
			vcl::vert::CustomComponents<Vertex>>            // 0b
{
};

class Face :
		public vcl::Face<
			vcl::face::TriangleBitFlags,                      // 4b
			vcl::face::TriangleVertexRefs<Vertex>,            // 24b
			vcl::face::Normal3d,                              // 24b
			vcl::face::OptionalScalard<Face>,                 // 0b
			vcl::face::OptionalColor<Face>,                   // 0b
			vcl::face::OptionalAdjacentTriangles<Face>,       // 0b
			vcl::face::OptionalTriangleWedgeTexCoordsf<Face>, // 0b
			vcl::face::CustomComponents<Face>>                // 0b
{
};

} // namespace vcl::trimesh

namespace vcl {

class TriMesh :
		public vcl::Mesh<
			trimesh::Vertex,
			trimesh::Face,
			mesh::BoundingBox3d,
			mesh::Mark,
			mesh::TextureFileNames,
			mesh::TransformMatrixd>
{
public:
	using Vertex = trimesh::Vertex;
	using Face   = trimesh::Face;
};

} // namespace vcl

#endif // VCL_TRIMESH_H
