/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include "concepts/vertex_references.h"
#include "internal/element_references.h"

namespace vcl::comp {

template<
	typename Vertex,
	int N,
	typename ElementType = void,
	bool optional        = false>
class VertexReferences : protected internal::ElementReferences<Vertex, N, ElementType>
{
	using ThisType = VertexReferences<Vertex, N, ElementType, optional>;

	using Base = internal::ElementReferences<Vertex, N, ElementType>;

public:
	using DataValueType = typename Base::DataValueType; // data that the component stores internally (or vertically)
	using VertexReferencesComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	using VertexType = Vertex;

	static const int VERTEX_NUMBER = Base::CONTAINER_SIZE;

	/* Iterator Types declaration */

	using VertexIterator           = typename Base::Iterator;
	using ConstVertexIterator      = typename Base::ConstIterator;
	using VertexRangeIterator      = typename Base::RACRangeIterator;
	using ConstVertexRangeIterator = typename Base::RACConstRangeIterator;

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

	int indexOfVertex(const Vertex* v) const;
	int indexOfEdge(const Vertex* v1, const Vertex* v2) const;

	/* Member functions specific for vector of references */

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
	VertexRangeIterator      vertices();
	ConstVertexRangeIterator vertices() const;

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	template<typename Element>
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

#include "vertex_references.cpp"

#endif // VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
