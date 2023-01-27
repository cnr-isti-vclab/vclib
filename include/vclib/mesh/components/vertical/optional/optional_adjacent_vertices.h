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

#ifndef VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H

#include <array>
#include <vector>

#include <vclib/iterators/range_iterator.h>

#include "../vertical_component.h"

#include "../../concepts/adjacent_vertices.h"

namespace vcl::mesh {
template<typename, typename>
class OptionalVertexReferencesVector;
}

namespace vcl::comp {

template<typename Vertex, typename T>
class OptionalAdjacentVertices : public virtual VerticalComponent<T>
{
	template<typename, typename>
	friend class OptionalVertexReferencesVector;

private:
	using B = VerticalComponent<T>;
	uint thisId() const { return B::index((T*)this); }

public:
	using AdjacentVertexType = Vertex;

	// possibility to access to the AdjacentVertices class, whether is optional or not.
	using AdjacentVertices = OptionalAdjacentVertices;

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

	bool isAdjVerticesEnabled() const;

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

	// dummy member to discriminate between non-optional and optional component
	void __optionalAdjVertices() const {};

	// dummy member to discriminate between this component and the one simulated by the
	// VertexHalfEdgeReference component
	void __compAdjVertices() const {};

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	template <typename Element>
	void importFrom(const Element& e);

	template<typename Element, typename ElVType>
	void
	importVertexReferencesFrom(const Element& e, Vertex* base, const ElVType* ebase);

private:
	template<typename Element, typename ElVType>
	void
	importReferencesFrom(const Element& e, Vertex* base, const ElVType* ebase);
};

} // namespace vcl::comp

#include "optional_adjacent_vertices.cpp"

#endif // VCL_MESH_COMPONENTS_OPTIONAL_ADJACENT_VERTICES_H
