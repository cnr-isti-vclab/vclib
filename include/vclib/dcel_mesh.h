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

#ifndef VCL_DCEL_MESH_H
#define VCL_DCEL_MESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl::dcel {

class HalfEdge;
class Vertex;
class Face;

class HalfEdge :
		public vcl::HalfEdge<
			vcl::hedge::BitFlags,
			vcl::hedge::HalfEdgeReferences<HalfEdge, Vertex, Face>,
			vcl::hedge::OptionalScalard<HalfEdge>,
			vcl::hedge::OptionalColor<HalfEdge>,
			vcl::hedge::OptionalMark<HalfEdge>,
			vcl::hedge::CustomComponents<HalfEdge>>
{
};

class Vertex :
		public vcl::Vertex<
			vcl::vert::BitFlags,
			vcl::vert::Coordinate3d,
			vcl::vert::Normal3d,
			vcl::vert::Color,
			vcl::vert::Scalard,
			vcl::vert::HalfEdgeReference<HalfEdge>,
			vcl::vert::OptionalTexCoordf<Vertex>,
			vcl::vert::OptionalMark<Vertex>,
			vcl::vert::CustomComponents<Vertex>>
{
};

class Face :
		public vcl::Face<
			vcl::face::BitFlags,
			vcl::face::HalfEdgeReference<HalfEdge>,
			vcl::face::Normal3d,
			vcl::face::OptionalScalard<Face>,
			vcl::face::OptionalColor<Face>,
			vcl::face::OptionalMark<Face>,
			vcl::face::CustomComponents<Face>>
{
};

} // namespace vcl::dcel

namespace vcl {

class DcelMesh :
		public vcl::Mesh<
			mesh::VertexContainer<dcel::Vertex>,
			mesh::FaceContainer<dcel::Face>,
			mesh::HalfEdgeContainer<dcel::HalfEdge>,
			mesh::BoundingBox3d,
			mesh::Mark,
			mesh::TexFileNames,
			mesh::TransformMatrixd>
{
};

} // namespace vcl

#endif // VCL_DCEL_MESH_H
