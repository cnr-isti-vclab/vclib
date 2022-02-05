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
		"You should include BitFlags (or a derived) as HalfEdge component in your Mesh "
		"definition.");
	static_assert(
		vcl::hedge::hasHalfEdgeReferences<T>(),
		"You should include a HalfEdgeReferences as HalfEdge component in your Mesh definition.");

	template <typename U>
	friend class HalfEdgeContainer;

	using HalfEdgeContainerType = HalfEdgeContainer<T>;
	using Base = ElementContainer<T>;

public:
	using HalfEdge                   = T;
	using HalfEdgeType               = T;
	using HalfEdgeIterator           = typename Base::ElementIterator;
	using ConstHalfEdgeIterator      = typename Base::ConstElementIterator;
	using HalfEdgeRangeIterator      = typename Base::ElementRangeIterator;
	using ConstHalfEdgeRangeIterator = typename Base::ConstElementRangeIterator;

	HalfEdgeContainer();

	const HalfEdgeType& halfEdge(uint i) const;
	HalfEdgeType&       halfEdge(uint i);

	uint halfEdgeNumber() const;
	uint halfEdgeContainerSize() const;
	uint deletedHalfEdgeNumber() const;

	void deleteHalfEdge(uint i);
	void deleteHalfEdge(const HalfEdgeType* e);
};

} // namespace vcl::mesh

#include "half_edge_container.cpp"

#endif // VCL_MESH_CONTAINER_HALF_EDGE_CONTAINER_H
