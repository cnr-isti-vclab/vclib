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

#include "element_requirements.h"

#include "../mesh.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

template<typename MeshType>
bool constexpr hasVertices()
{
	return vcl::mesh::hasVertices<MeshType>();
}

template<typename MeshType>
bool constexpr hasFaces()
{
	return vcl::mesh::hasFaces<MeshType>();
}

/*********************
 * require functions *
 *********************/

template<typename MeshType>
void constexpr requireVertices()
{
	static_assert(hasVertices<MeshType>(), "Mesh does not have a Vertex Container.");
}

template<typename MeshType>
void constexpr requireFaces()
{
	static_assert(hasFaces<MeshType>(), "Mesh does not have a Face Container.");
}

}
