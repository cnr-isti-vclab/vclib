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
#include <vector>

#include "optional_info.h"

#include "../components/detection/adjacent_vertices_detection.h"
#include "../iterators/range_iterator.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalVertexReferencesVector;
}

namespace vcl::comp {

template<typename Vertex, typename T>
class OptionalAdjacentVertices :
		public OptionalAdjacentVerticesTriggerer,
		public virtual OptionalInfo<T>
{
	template<typename, typename>
	friend class OptionalVertexReferencesVector;

private:
	using B = OptionalInfo<T>;

public:
	// the AdjVertsContainer type will be array or vector, depending on N value
	using AdjVertsContainer = std::vector<Vertex*>;

	/* Iterator Types declaration */

	// if using array, will be the array iterator, the vector iterator otherwise
	using AdjacentVertexIterator = typename std::vector<Vertex*>::iterator;
	using ConstAdjacentVertexIterator = typename std::vector<Vertex*>::const_iterator;

	using AdjacentVertexRangeIterator = RangeIterator<OptionalAdjacentVertices, AdjacentVertexIterator>;
	using ConstAdjacentVertexRangeIterator =
		ConstRangeIterator<OptionalAdjacentVertices, ConstAdjacentVertexIterator>;

	/* Constructor */

	OptionalAdjacentVertices();

	/* Member functions */

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

	/* Member functions specific for vector */

	void resizeAdjVertices(uint n);
	void pushAdjVertex(Vertex* f);
	void insertAdjVertex(uint i, Vertex* f);
	void eraseAdjVertex(uint i);
	void clearAdjVertices();

	/* Iterator Member functions */

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

} // namespace vcl::comp

#include "optional_adjacent_vertices.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
