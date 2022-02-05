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

#include "face_half_edge_reference.h"

namespace vcl::comp {

template<typename HalfEdge>
FaceHalfEdgeReference<HalfEdge>::FaceHalfEdgeReference()
{
}

template<typename HalfEdge>
const HalfEdge *FaceHalfEdgeReference<HalfEdge>::outerHalfEdge() const
{
	return ohe;
}

template<typename HalfEdge>
HalfEdge*& FaceHalfEdgeReference<HalfEdge>::outerHalfEdge()
{
	return ohe;
}

template<typename HalfEdge>
uint FaceHalfEdgeReference<HalfEdge>::numberHoles() const
{
	return ihe.size();
}

template<typename HalfEdge>
const HalfEdge* FaceHalfEdgeReference<HalfEdge>::innerHalfEdge(uint i) const
{
	return ihe[i];
}

template<typename HalfEdge>
HalfEdge*& FaceHalfEdgeReference<HalfEdge>::innerHalfEdge(uint i)
{
	return ihe[i];
}

template<typename HalfEdge>
uint FaceHalfEdgeReference<HalfEdge>::vertexNumber() const
{
	uint cnt = 0;
	for (const Vertex* v: vertices())
		++cnt;
	return cnt;
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::Vertex*&
FaceHalfEdgeReference<HalfEdge>::vertex(uint i)
{
	uint j = 0;
	VertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const typename FaceHalfEdgeReference<HalfEdge>::Vertex*
FaceHalfEdgeReference<HalfEdge>::vertex(uint i) const
{
	uint j = 0;
	ConstVertexIterator it = vertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::Vertex*&
FaceHalfEdgeReference<HalfEdge>::vertexMod(int i)
{
	uint j = 0;
	VertexIterator it = VertexIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const typename FaceHalfEdgeReference<HalfEdge>::Vertex*
FaceHalfEdgeReference<HalfEdge>::vertexMod(int i) const
{
	uint j = 0;
	ConstVertexIterator it =
		ConstVertexIterator(ohe, nullptr); // this iterator does not have an end
	while (j < std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::setVertex(Vertex *v, uint i)
{
	vertex(i) = v;
}

template<typename HalfEdge>
void FaceHalfEdgeReference<HalfEdge>::setVertices(const std::vector<Vertex *> &list)
{
	assert(list.size() == vertexNumber());
	uint i = 0;
	for (Vertex*& v : vertices()) {
		v = list[i++];
	}
}

template<typename HalfEdge>
bool FaceHalfEdgeReference<HalfEdge>::containsVertex(const Vertex *v) const
{
	return std::find(vertexBegin(), vertexEnd(), v) != vertexEnd();
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexIterator
FaceHalfEdgeReference<HalfEdge>::findVertex(const Vertex* v)
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexIterator
FaceHalfEdgeReference<HalfEdge>::findVertex(const Vertex* v) const
{
	return std::find(vertexBegin(), vertexEnd(), v);
}

template<typename HalfEdge>
int FaceHalfEdgeReference<HalfEdge>::indexOfVertex(const Vertex *v) const
{
	uint i = 0;
	for (const Vertex* vv : vertices()) {
		if (vv == v) return i;
		i++;
	}
	return -1;
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexBegin()
{
	return VertexIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexBegin() const
{
	return ConstVertexIterator(ohe);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexEnd()
{
	return VertexIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexIterator
FaceHalfEdgeReference<HalfEdge>::vertexEnd() const
{
	return ConstVertexIterator(nullptr);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::VertexRangeIterator
FaceHalfEdgeReference<HalfEdge>::vertices()
{
	return VertexRangeIterator(
		*this, &FaceHalfEdgeReference::vertexBegin, &FaceHalfEdgeReference::vertexEnd);
}

template<typename HalfEdge>
typename FaceHalfEdgeReference<HalfEdge>::ConstVertexRangeIterator
FaceHalfEdgeReference<HalfEdge>::vertices() const
{
	return ConstVertexRangeIterator(
		*this, &FaceHalfEdgeReference::vertexBegin, &FaceHalfEdgeReference::vertexEnd);
}

} // namespace vcl::comp
