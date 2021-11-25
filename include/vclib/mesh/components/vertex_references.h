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

#ifndef VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include "internal/element_references.h"

namespace vcl::comp {

class VertexReferencesTriggerer
{
};

template<typename Vertex, int N>
class VertexReferences : protected internal::ElementReferences<Vertex, N>, public VertexReferencesTriggerer
{
	using Base = internal::ElementReferences<Vertex, N>;

public:
	using VertexType = Vertex;

	static const int VERTEX_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using VertexIterator           = typename Base::GCIterator;
	using ConstVertexIterator      = typename Base::ConstGCIterator;
	using VertexRangeIterator      = typename Base::GCRangeIterator;
	using ConstVertexRangeIterator = typename Base::ConstGCRangeIterator;

	/** Constructor **/

	VertexReferences();

	/** Member functions **/

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
	int indexOfEdge(const Vertex* v1, const Vertex* v2);

	/** Member functions specific for vector **/

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) resizeVertices(uint n);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) pushVertex(Vertex* v);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) insertVertex(uint i, Vertex* v);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) eraseVertex(uint i);

	template<int M = N>
	VCL_ENABLE_IF(M < 0, void) clearVertices();

	/** Iterator Member functions **/

	VertexIterator           vertexBegin();
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexBegin() const;
	ConstVertexIterator      vertexEnd() const;
	VertexRangeIterator      vertices();
	ConstVertexRangeIterator vertices() const;

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);
};

template<typename Vertex>
class TriVertexReferences : public VertexReferences<Vertex, 3>
{
private:
	using B = VertexReferences<Vertex, 3>;

public:
	Vertex*&      v0() { return B::container[0]; }
	Vertex*&      v1() { return B::container[1]; }
	Vertex*&      v2() { return B::container[2]; }
	const Vertex* v0() const { return B::container[0]; }
	const Vertex* v1() const { return B::container[1]; }
	const Vertex* v2() const { return B::container[2]; }

	void setV0(Vertex* v) { B::container[0] = v; }
	void setV1(Vertex* v) { B::container[1] = v; }
	void setV2(Vertex* v) { B::container[2] = v; }
};

template<typename T>
using hasVertexReferencesT = std::is_base_of<VertexReferencesTriggerer, T>;

template<typename T>
bool constexpr hasVertexReferences()
{
	return hasVertexReferencesT<T>::value;
}

} // namespace vcl::comp

#include "vertex_references.cpp"

#endif // VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
