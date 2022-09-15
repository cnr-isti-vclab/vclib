/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

namespace tmpSimplePolyMesh {
class Vertex;
class Face;

class Vertex : public vcl::Vertex<vcl::vert::BitFlags, vcl::vert::Coordinate3d>
{
};

class Face : public vcl::Face<vcl::face::PolygonBitFlags, vcl::face::PolygonVertexRefs<Vertex>>
{
};

} // namespace tmpSimplePolyMesh

class TMPSimplePolyMesh :
		public vcl::Mesh<
			mesh::VertexContainer<tmpSimplePolyMesh::Vertex>,
			mesh::FaceContainer<tmpSimplePolyMesh::Face>>
{
};

} // namespace vcl::internal

#endif // VCL_MISC_TMP_MESHES_H
