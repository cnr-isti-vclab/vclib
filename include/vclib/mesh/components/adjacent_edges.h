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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_ADJACENT_EDGES_H

#include "internal/element_references.h"

namespace vcl::comp {

class AdjacentEdgesTriggerer
{
};

/**
 * @brief The AdjacentEdges class is a container of Edge references. It could be used by any
 * Element to save adjacencies information (also the Edge element itself).
 *
 * It is a random access container having static or dynamic size, depending on the value of N (a
 * negative number means dynamic).
 *
 * @note If this component is part of a Face Element, the number of Adjacent Edges is tied to the
 * Vertex Number of the Face, therefore all the members that allows to modify the number of
 * Adjacent Edges in case of dynamic size won't be available on Face Elements.
 */
template<typename Edge, int N>
class AdjacentEdges : protected internal::ElementReferences<Edge, N>, public AdjacentEdgesTriggerer
{
	using Base = internal::ElementReferences<Edge, N>;

public:
	/// Static size of the container. If the container is dynamic, this value will be negative and
	/// you should use the adjEdgesNumber() member function.
	static const int ADJ_EDGE_NUMBER = Base::CONTAINER_SIZE;

	/* Iterator Types declaration */

	using AdjacentEdgeIterator           = typename Base::GCIterator;
	using ConstAdjacentEdgeIterator      = typename Base::ConstGCIterator;
	using AdjacentEdgeRangeIterator      = typename Base::GCRangeIterator;
	using ConstAdjacentEdgeRangeIterator = typename Base::ConstGCRangeIterator;

	/* Constructor */

	AdjacentEdges();

	/* Member functions */

	uint adjEdgesNumber() const;

	Edge*&      adjEdge(uint i);
	const Edge* adjEdge(uint i) const;
	Edge*&      adjEdgeMod(int i);
	const Edge* adjEdgeMod(int i) const;

	void setAdjEdge(Edge* f, uint i);
	void setAdjEdges(const std::vector<Edge*>& list);

	bool containsAdjEdge(const Edge* e) const;

	AdjacentEdgeIterator      findAdjEdge(const Edge* e);
	ConstAdjacentEdgeIterator findAdjEdge(const Edge* e) const;

	int indexOfAdjEdge(const Edge* e) const;

	/* Member functions specific for vector */

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void)
	resizeAdjEdges(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void)
	pushAdjEdge(Edge* e);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void)
	insertAdjEdge(uint i, Edge* e);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void)
	eraseAdjEdge(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void)
	clearAdjEdges();

	/* Iterator Member functions */

	AdjacentEdgeIterator           adjEdgeBegin();
	AdjacentEdgeIterator           adjEdgeEnd();
	ConstAdjacentEdgeIterator      adjEdgeBegin() const;
	ConstAdjacentEdgeIterator      adjEdgeEnd() const;
	AdjacentEdgeRangeIterator      adjEdges();
	ConstAdjacentEdgeRangeIterator adjEdges() const;

protected:
	void updateEdgeReferences(const Edge* oldBase, const Edge* newBase);

	void updateEdgeReferencesAfterCompact(const Edge* base, const std::vector<int>& newIndices);
};

/**
 * Detector to check if a class has (inherits) AdjacenctEdges
 */

template<typename T>
using hasAdjacentEdgesT = std::is_base_of<AdjacentEdgesTriggerer, T>;

template<typename T>
bool constexpr hasAdjacentEdges()
{
	return hasAdjacentEdgesT<T>::value;
}

template<typename T>
bool constexpr sanityCheckAdjacentEdges()
{
	if constexpr (hasAdjacentEdges<T>()) {
		return T::VERTEX_NUMBER == T::ADJ_EDGE_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#include "adjacent_edges.cpp"

#endif // VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
