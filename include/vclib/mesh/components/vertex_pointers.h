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

#ifndef VCL_MESH_COMPONENTS_VERTEX_POINTERS_H
#define VCL_MESH_COMPONENTS_VERTEX_POINTERS_H

#include <vclib/concepts/mesh/components/vertex_pointers.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The VertexPointers class
 *
 * @ingroup components
 */
template<typename Vertex, int N, typename ElementType = void, bool OPT = false>
class VertexPointers : public PointersContainerComponent<VERTEX_PTRS, Vertex, N, ElementType, OPT, false>
{
	using Base = PointersContainerComponent<VERTEX_PTRS, Vertex, N, ElementType, OPT, false>;

public:
	using VertexType = Vertex;

	static const int VERTEX_NUMBER = Base::SIZE;

	/* Iterator Types declaration */

	using VertexIterator      = typename Base::Iterator;
	using ConstVertexIterator = typename Base::ConstIterator;

	/* Constructor and isEnabled */

	void init();

	bool isEnabled();

	/* Member functions */

	uint vertexNumber() const;

	Vertex*&      vertex(uint i);
	const Vertex* vertex(uint i) const;
	Vertex*&      vertexMod(int i);
	const Vertex* vertexMod(int i) const;

	void setVertex(Vertex* v, uint i);
	void setVertices(const std::vector<Vertex*>& list);

	bool containsVertex(const Vertex* v) const;

	VertexIterator findVertex(const Vertex* v);
	ConstVertexIterator findVertex(const Vertex* v) const;

	uint indexOfVertex(const Vertex* v) const;
	uint indexOfEdge(const Vertex* v1, const Vertex* v2) const;

	/* Member functions specific for vector of pointers */

	void resizeVertices(uint n) requires (N < 0);
	void pushVertex(Vertex* v) requires (N < 0);
	void insertVertex(uint i, Vertex* v) requires (N < 0);
	void eraseVertex(uint i) requires (N < 0);
	void clearVertices() requires (N < 0);

	/* Iterator Member functions */

	VertexIterator           vertexBegin();
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexBegin() const;
	ConstVertexIterator      vertexEnd() const;
	auto                     vertices();
	auto                     vertices() const;

protected:
	// Component interface function
	template<typename Element>
	void importFrom(const Element& e);

	// PointersComponent interface functions
	template<typename Element, typename ElVType>
	void importPointersFrom(const Element& e, Vertex* base, const ElVType* ebase);

	void updatePointers(const Vertex* oldBase, const Vertex* newBase);

	void updatePointers(const Vertex* base, const std::vector<uint>& newIndices);

private:
	template<typename Element, typename ElVType>
	void importPtrsFrom(const Element& e, Vertex* base, const ElVType* ebase);
};

} // namespace vcl::comp

#include "vertex_pointers.cpp"

#endif // VCL_MESH_COMPONENTS_VERTEX_POINTERS_H
