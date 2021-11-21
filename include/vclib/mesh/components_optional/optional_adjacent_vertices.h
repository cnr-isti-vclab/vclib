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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalVertexReferencesVector;
}

namespace vcl::comp {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

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

	unsigned int adjVerticesNumber() const;

	Vertex*&      adjVertex(unsigned int i);
	const Vertex* adjVertex(unsigned int i) const;
	Vertex*&      adjVertexMod(int i);
	const Vertex* adjVertexMod(int i) const;

	void setAdjVertex(Vertex* f, unsigned int i);
	void setAdjVertices(const std::vector<Vertex*>& list);

	bool containsAdjVertex(const Vertex* v) const;

	AdjacentVertexIterator findAdjVertex(const Vertex* v);
	ConstAdjacentVertexIterator findAdjVertex(const Vertex* v) const;

	int indexOfAdjVertex(const Vertex* v) const;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeAdjVertices(unsigned int n);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushAdjVertex(Vertex* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertAdjVertex(unsigned int i, Vertex* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseAdjVertex(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearAdjVertices();

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
	unsigned int thisId() const { return B::index((T*)this); }
};

/**
 * Detector to check if a class has (inherits) OptionalVertexReferences
 */

template<typename T>
using hasOptionalAdjacentVerticesT = std::is_base_of<OptionalAdjacentVerticesTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentVertices =
	typename std::enable_if<hasOptionalAdjacentVerticesT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalAdjacentVertices()
{
	return hasOptionalAdjacentVerticesT<T>::value;
}

} // namespace vcl::comp

#include "optional_adjacent_vertices.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
