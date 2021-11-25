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

#ifndef VCL_MESH_ELEMENTS_EDGE_H
#define VCL_MESH_ELEMENTS_EDGE_H

namespace vcl::mesh {

// EdgeContainer class declaration
template<typename>
class EdgeContainer;

} // namespace vcl::mesh

namespace vcl {

// Dummy class used to detect a Edge regardless of its template arguments
class EdgeTriggerer
{
};

template<typename... Args>
class Edge : public EdgeTriggerer, public Args...
{
	template<typename>
	friend class mesh::EdgeContainer;

	// Vertex references component of the Face
	using VRefs      = typename Edge::VertexReferences;
	using VertexType = typename VRefs::VertexType;
};

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_EDGE_H
