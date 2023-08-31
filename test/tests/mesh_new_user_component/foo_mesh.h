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

#ifndef FOO_MESH_H
#define FOO_MESH_H

#include <vclib/mesh/mesh.h>
#include <vclib/mesh/requirements.h>

#include "foo_component.h"

/*
 * This file defines a FooMesh class that uses the FooComponent in the Vertex
 * Element.
 */

class FooMesh;

namespace foomesh {

class Vertex;

class Face;

class Vertex :
		public vcl::Vertex<
			FooMesh,
			vcl::vert::BitFlags,
			vcl::vert::Coordinate3d,
			vcl::vert::Normal3d,
			vcl::vert::Color,
			FooComponent>
{
};

class Face :
		public vcl::Face<
			FooMesh,
			vcl::face::TriangleBitFlags,
			vcl::face::TriangleVertexPtrs<Vertex>,
			vcl::face::Normal3d>
{
};

} // namespace foomesh

class FooMesh :
		public vcl::Mesh<
			vcl::mesh::VertexContainer<foomesh::Vertex>,
			vcl::mesh::FaceContainer<foomesh::Face>,
			vcl::mesh::BoundingBox3d>
{
public:
	using ScalarType = double;
};


#endif // FOO_MESH_H
