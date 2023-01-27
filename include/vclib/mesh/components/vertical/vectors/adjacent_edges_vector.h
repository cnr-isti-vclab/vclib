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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_EDGES_VECTOR_H
#define VCL_MESH_COMPONENTS_ADJACENT_EDGES_VECTOR_H

#include "../../concepts/adjacent_edges.h"

#include "generic_component_vector.h"

namespace vcl::internal {

// the AdjEdgesContainer type will be array or vector, depending on N value
template<typename E, int N>
using AdjEdgesContainer = typename std::conditional<
	(N >= 0),
	typename std::array<E*, N >= 0 ? N : 0>,
	typename std::vector<E*>>::type;

template<typename>
class AdjacentEdgesVector
{
public:
	void clear() {}
	void resize(uint) {}
	void reserve(uint) {}
	void compact(const std::vector<int>&) {}
	bool isAdjacentEdgesEnabled() const { return false; }
};

template<comp::HasOptionalAdjacentEdges T>
class AdjacentEdgesVector<T> :
		private GenericComponentVector<
			AdjEdgesContainer<typename T::AdjacentEdgeType, T::ADJ_EDGE_NUMBER>>
{
private:
	// the base class is a GenericComponentVector of Array/Vectors of Edge pointers
	// each Array/Vector of the GenericComponentVector is the list of the Edges adjacent to T,
	// which is the current elemeent
	using Container = AdjEdgesContainer<typename T::AdjacentEdgeType, T::ADJ_EDGE_NUMBER>;
	using Base      = GenericComponentVector<Container>;

public:
	using Base::clear;
	using Base::compact;
	using Base::reserve;
	using Base::resize;

	bool isAdjacentEdgesEnabled() const { return Base::isEnabled(); };
	void enableAdjacentEdges(uint size) { Base::enable(size); }
	void disableAdjacentEdges() { Base::disable(); }

	Container&       adjEdges(uint i) { return Base::at(i); }
	const Container& adjEdges(uint i) const { return Base::at(i); }
};

} // namespace vcl::internal

#endif // VCL_MESH_COMPONENTS_ADJACENT_EDGES_VECTOR_H
