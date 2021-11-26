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

#ifndef VCL_POLYMESH_H
#define VCL_POLYMESH_H

#include "mesh/mesh.h"
#include "mesh/requirements.h"

namespace vcl::polymesh {

class Vertex;
class Face;

class Vertex : public vcl::Vertex<vcl::vert::BitFlags, vcl::vert::Coordinate3d>
{
};

class Face :
		public vcl::Face<
			vcl::face::PolygonBitFlags, // 4b
			vcl::face::PolygonVertexRefs<Vertex>,
			vcl::face::PolygonWedgeTexCoordsf,
			vcl::face::OptionalAdjacentPolygons<Face>>
{
};

} // namespace vcl::polymesh

namespace vcl {

class PolyMesh : public vcl::Mesh<polymesh::Vertex, polymesh::Face>
{
public:
	using Vertex = polymesh::Vertex;
	using Face   = polymesh::Face;
};

} // namespace vcl

#endif // VCL_POLYMESH_H
