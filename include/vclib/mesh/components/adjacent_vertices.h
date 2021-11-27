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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include "internal/element_references.h"

namespace vcl::comp {

class AdjacentVerticesTriggerer
{
};

/**
 * @brief The AdjacentVertices class is a container of Vertex references. It is a component that
 * makes sense to use mostly on Vertex Elements. For Faces and Edges, see the VertexReferences
 * component (which is similar, but has different member function names).
 *
 * It is a random access container having static or dynamic size, depending on the value of N (a
 * negative number means dynamic).
 *
 * The member functions of this class will be available in the instance of any Element that will
 * contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentVertices component, you'll be
 * able to access to this component member functions from `v`:
 *
 * @code{.cpp}
 * v.adjVerticesNumber();
 * @endcode
 */
template<typename Vertex, int N>
class AdjacentVertices : protected internal::ElementReferences<Vertex, N>, public AdjacentVerticesTriggerer
{
	using Base = internal::ElementReferences<Vertex, N>;

public:
	static const int ADJ_VERTEX_NUMBER = Base::CONTAINER_SIZE;

	/* Iterator Types declaration */

	using AdjacentVertexIterator           = typename Base::GCIterator;
	using ConstAdjacentVertexIterator      = typename Base::ConstGCIterator;
	using AdjacentVertexRangeIterator      = typename Base::GCRangeIterator;
	using ConstAdjacentVertexRangeIterator = typename Base::ConstGCRangeIterator;

	/* Constructor */

	AdjacentVertices();

	/* Member functions */

	uint adjVerticesNumber() const;

	Vertex*&      adjVertex(uint i);
	const Vertex* adjVertex(uint i) const;
	Vertex*&      adjVertexMod(int i);
	const Vertex* adjVertexMod(int i) const;

	void setAdjVertex(Vertex* v, uint i);
	void setAdjVertices(const std::vector<Vertex*>& list);

	bool containsAdjVertex(const Vertex* v) const;

	AdjacentVertexIterator findAdjVertex(const Vertex* v);
	ConstAdjacentVertexIterator findAdjVertex(const Vertex* v) const;

	int indexOfAdjVertex(const Vertex* v) const;

	/* Member functions specific for vector */

	/**
	 * @brief Resize the container of the adjacent vertices to the given size.
	 * @note This function is available only if the container of the Adjacent Vertices is has dynamic
	 * size.
	 * @param n
	 */
	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeAdjVertices(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushAdjVertex(Vertex* v);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) insertAdjVertex(uint i, Vertex* v);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseAdjVertex(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearAdjVertices();

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
};

template<typename T>
using hasAdjacentVerticesT = std::is_base_of<AdjacentVerticesTriggerer, T>;

template<typename T>
bool constexpr hasAdjacentVertices()
{
	return hasAdjacentVerticesT<T>::value;
}

} // namespace vcl::comp

#include "adjacent_vertices.cpp"

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
