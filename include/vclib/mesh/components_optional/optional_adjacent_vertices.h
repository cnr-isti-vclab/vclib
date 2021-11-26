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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H

#include <array>
#include <assert.h>
#include <vector>

#include <vclib/misc/vcl_types.h>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalVertexReferencesVector;
}

namespace vcl::comp {

class OptionalAdjacentVerticesTriggerer
{
};

template<typename Vertex, int N, typename T>
class OptionalAdjacentVertices :
		public OptionalAdjacentVerticesTriggerer,
		public virtual OptionalInfo<T>
{
	template<typename, typename>
	friend class OptionalVertexReferencesVector;

private:
	using B = OptionalInfo<T>;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// the AdjVertsContainer type will be array or vector, depending on N value
	using AdjVertsContainer = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>,
		typename std::vector<Vertex*>>::type;

	static const int ADJ_VERTEX_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using AdjacentVertexIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>::iterator,
		typename std::vector<Vertex*>::iterator>::type;

	using ConstAdjacentVertexIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>::const_iterator,
		typename std::vector<Vertex*>::const_iterator>::type;

	using AdjacentVertexRangeIterator = RangeIterator<OptionalAdjacentVertices, AdjacentVertexIterator>;
	using ConstAdjacentVertexRangeIterator =
		ConstRangeIterator<OptionalAdjacentVertices, ConstAdjacentVertexIterator>;

	/** Constructor **/

	OptionalAdjacentVertices();

	/** Member functions **/

	uint adjVerticesNumber() const;

	Vertex*&      adjVertex(uint i);
	const Vertex* adjVertex(uint i) const;
	Vertex*&      adjVertexMod(int i);
	const Vertex* adjVertexMod(int i) const;

	void setAdjVertex(Vertex* f, uint i);
	void setAdjVertices(const std::vector<Vertex*>& list);

	bool containsAdjVertex(const Vertex* v) const;

	AdjacentVertexIterator findAdjVertex(const Vertex* v);
	ConstAdjacentVertexIterator findAdjVertex(const Vertex* v) const;

	int indexOfAdjVertex(const Vertex* v) const;

	/** Member functions specific for vector **/

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeAdjVertices(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushAdjVertex(Vertex* f);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) insertAdjVertex(uint i, Vertex* f);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseAdjVertex(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearAdjVertices();

	/** Iterator Member functions **/

	AdjacentVertexIterator           adjVertexBegin();
	AdjacentVertexIterator           adjVertexEnd();
	ConstAdjacentVertexIterator      adjVertexBegin() const;
	ConstAdjacentVertexIterator      adjVertexEnd() const;
	AdjacentVertexRangeIterator      adjVertices();
	ConstAdjacentVertexRangeIterator adjVertices() const;

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

private:
	uint thisId() const { return B::index((T*)this); }
};

/**
 * Detector to check if a class has (inherits) OptionalVertexReferences
 */

template<typename T>
using hasOptionalAdjacentVerticesT = std::is_base_of<OptionalAdjacentVerticesTriggerer, T>;

template<typename T>
bool constexpr hasOptionalAdjacentVertices()
{
	return hasOptionalAdjacentVerticesT<T>::value;
}

} // namespace vcl::comp

#include "optional_adjacent_vertices.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
