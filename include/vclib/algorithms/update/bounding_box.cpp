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

#include "bounding_box.h"

namespace vcl {

/**
 * @brief updateBoundingBox updates the bounding box of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - BoundingBox
 *
 * @param m
 */
template<MeshConcept MeshType>
void updateBoundingBox(MeshType& m)
	requires vcl::HasBoundingBox<MeshType>
{
	using VertexType = typename MeshType::VertexType;

	m.boundingBox().setNull();
	for (const VertexType& v : m.vertices()) {
		m.boundingBox().add(v.coord());
	}
}

} // namespace vcl
