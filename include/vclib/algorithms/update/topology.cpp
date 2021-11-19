/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#include "topology.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief updatePerVertexAdjacentFaces
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - AdjacentFaces
 *   - Faces
 *
 * @param m
 */
template<typename MeshType>
void updatePerVertexAdjacentFaces(MeshType& m)
{
	vcl::requirePerVertexAdjacentFaces(m);
	vcl::requireFaces<MeshType>();

	using VertexType = typename MeshType::Vertex;
	using FaceType   = typename MeshType::Face;

	for (VertexType& v : m.vertices()) {
		v.clearAdjFaces();
	}

	for (FaceType& f : m.faces()) {
		for (VertexType* v : f.vertices()){
			v->pushAdjFace(&f);
		}
	}
}

}

