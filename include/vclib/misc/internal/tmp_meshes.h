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

#ifndef VCL_MISC_TMP_MESHES_H
#define VCL_MISC_TMP_MESHES_H

#include <vclib/mesh/mesh.h>

namespace vcl::internal {

class TMPSimpleTriMesh;
class TMPSimplePolyMesh;

namespace tmpMesh {

class TriVertex : public vcl::Vertex<TMPSimpleTriMesh, vcl::vert::BitFlags, vcl::vert::Coordinate3d>
{
};

class PolyVertex :
		public vcl::Vertex<TMPSimplePolyMesh, vcl::vert::BitFlags, vcl::vert::Coordinate3d>
{
};

class TriFace :
		public vcl::Face<
			TMPSimpleTriMesh,
			vcl::face::TriangleBitFlags,
			vcl::face::TriangleVertexPtrs<TriVertex>>
{
};

class PolyFace :
		public vcl::Face<
			TMPSimplePolyMesh,
			vcl::face::PolygonBitFlags,
			vcl::face::PolygonVertexPtrs<PolyVertex>>
{
};

} // namespace tmpMesh

class TMPSimpleTriMesh :
		public vcl::Mesh<
			mesh::VertexContainer<tmpMesh::TriVertex>,
			mesh::FaceContainer<tmpMesh::TriFace>>
{
};

class TMPSimplePolyMesh :
		public vcl::Mesh<
			mesh::VertexContainer<tmpMesh::PolyVertex>,
			mesh::FaceContainer<tmpMesh::PolyFace>>
{
};

} // namespace vcl::internal

#endif // VCL_MISC_TMP_MESHES_H
