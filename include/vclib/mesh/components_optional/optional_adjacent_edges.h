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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_EDGES_H

#include <array>
#include <vector>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalAdjacentEdgesVector;
}

namespace vcl::comp {

class OptionalAdjacentEdgesTriggerer
{
};

template<typename Edge, int N, typename T>
class OptionalAdjacentEdges : public OptionalAdjacentEdgesTriggerer, public virtual OptionalInfo<T>
{
	template<typename, typename>
	friend class OptionalAdjacentEdgesVector;

private:
	using B = OptionalInfo<T>;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// the AdjEdgesContainer type will be array or vector, depending on N value
	using AdjEdgesContainer = typename std::conditional<
		(N >= 0),
		typename std::array<Edge*, ARRAY_SIZE>,
		typename std::vector<Edge*>>::type;

	static const int ADJ_EDGE_NUMBER = N;

	/* Iterator Types declaration */

	// if using array, will be the array iterator, the vector iterator otherwise
	using AdjacentEdgeIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Edge*, ARRAY_SIZE>::iterator,
		typename std::vector<Edge*>::iterator>::type;

	using ConstAdjacentEdgeIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Edge*, ARRAY_SIZE>::const_iterator,
		typename std::vector<Edge*>::const_iterator>::type;

	using AdjacentEdgeRangeIterator = RangeIterator<OptionalAdjacentEdges, AdjacentEdgeIterator>;
	using ConstAdjacentEdgeRangeIterator =
		ConstRangeIterator<OptionalAdjacentEdges, ConstAdjacentEdgeIterator>;

	/* Constructor */

	OptionalAdjacentEdges();

	/* Member functions */

	uint adjEdgesNumber() const;

	Edge*&      adjEdge(uint i);
	const Edge* adjEdge(uint i) const;
	Edge*&      adjEdgeMod(int i);
	const Edge* adjEdgeMod(int i) const;

	void setAdjEdge(Edge* e, uint i);
	void setAdjEdges(const std::vector<Edge*>& list);

	bool containsAdjEdge(const Edge* e) const;

	AdjacentEdgeIterator findAdjEdge(const Edge* e);
	ConstAdjacentEdgeIterator findAdjEdge(const Edge* e) const;

	int indexOfAdjEdge(const Edge* e) const;

	/* Member functions specific for vector */

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeAdjEdges(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushAdjEdge(Edge* e);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) insertAdjEdge(uint i, Edge* e);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseAdjEdge(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearAdjEdges();

	/* Iterator Member functions */

	AdjacentEdgeIterator           adjEdgeBegin();
	AdjacentEdgeIterator           adjEdgeEnd();
	ConstAdjacentEdgeIterator      adjEdgeBegin() const;
	ConstAdjacentEdgeIterator      adjEdgeEnd() const;
	AdjacentEdgeRangeIterator      adjEdges();
	ConstAdjacentEdgeRangeIterator adjEdges() const;

protected:
	bool adjEdgesEnabled() const;

	void updateEdgeReferences(const Edge* oldBase, const Edge* newBase);

	void updateEdgeReferencesAfterCompact(const Edge* base, const std::vector<int>& newIndices);

private:
	uint thisId() const { return B::index((T*)this); }
};

/**
 * Detector to check if a class has (inherits) OptionalAdjacentEdges
 */

template<typename T>
using hasOptionalAdjacentEdgesT = std::is_base_of<OptionalAdjacentEdgesTriggerer, T>;

template<typename T>
bool constexpr hasOptionalAdjacentEdges()
{
	return hasOptionalAdjacentEdgesT<T>::value;
}

template <typename T>
bool constexpr sanityCheckOptionalAdjacentEdges()
{
	if constexpr (hasOptionalAdjacentEdges<T>()) {
		return T::VERTEX_NUMBER == T::ADJ_EDGE_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#include "optional_adjacent_edges.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_EDGES_H
