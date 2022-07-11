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

#include "element_requirements.h"

#include "../mesh.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

/**
 * @brief Checks at compile time if MeshType has the Face Element.
 * @return true if MeshType has Faces.
 */
template<typename MeshType>
bool constexpr hasFaces()
{
	return vcl::mesh::hasFaces<MeshType>();
}

/**
 * @brief Checks at compile time if MeshType has the HalfEdge Element.
 * @return true if MeshType has HalfEdges.
 */
template<typename MeshType>
bool constexpr hasHalfEdges()
{
	return vcl::mesh::hasHalfEdges<MeshType>();
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void constexpr requireFaces()
{
	static_assert(hasFaces<MeshType>(), "Mesh does not have a Face Container.");
}

template<typename MeshType>
void constexpr requireHalfEdges()
{
	static_assert(hasHalfEdges<MeshType>(), "Mesh does not have an HalfEdge Container.");
}

} // namespace vcl
