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

#ifndef VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H
#define VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H

#include <vclib/iterators/container_iterator.h>
#include <vclib/iterators/container_range_iterator.h>
#include <vclib/mesh/elements/half_edge.h>

#include "../components/vertical/vectors/custom_component_vector_handle.h"
#include "element_container.h"

namespace vcl::mesh {

template<typename T>
class HalfEdgeContainer : protected ElementContainer<T>, public HalfEdgeContainerTriggerer
{
	// Sanity checks for the Edge -- all components must be consistent each other
	static_assert(
		vcl::hedge::hasBitFlags<T>(),
		"You should include BitFlags (or a derived) as Edge component in your Mesh definition.");
	static_assert(
		vcl::hedge::hasVertexReferences<T>(),
		"You should include a VertexReferences as Edge component in your Mesh definition.");
	static_assert(T::VERTEX_NUMBER == 2, "Edges must have 2 vertex references.");

public:
	HalfEdgeContainer();
};

} // namespace vcl::mesh

#include "half_edge_container.cpp"

#endif // VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H
