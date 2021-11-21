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

#include "vertex_references.h"

namespace vcl::comp {

template<typename Vertex, int N>
VertexReferences<Vertex, N>::VertexReferences() : Base()
{
}

template<typename Vertex, int N>
unsigned int VertexReferences<Vertex, N>::vertexNumber() const
{
	return Base::size();
}

template<typename Vertex, int N>
Vertex*& VertexReferences<Vertex, N>::vertex(unsigned int i)
{
	return Base::at(i);
}

template<typename Vertex, int N>
const Vertex* VertexReferences<Vertex, N>::vertex(unsigned int i) const
{
	return Base::at(i);
}

template<typename Vertex, int N>
Vertex*& VertexReferences<Vertex, N>::vertexMod(int i)
{
	return Base::atMod(i);
}

template<typename Vertex, int N>
const Vertex* VertexReferences<Vertex, N>::vertexMod(int i) const
{
	return Base::atMod(i);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::setVertex(Vertex* v, unsigned int i)
{
	Base::set(v, i);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::setVertices(const std::vector<Vertex*>& list)
{
	Base::set(list);
}

template<typename Vertex, int N>
bool VertexReferences<Vertex, N>::containsVertex(const Vertex* v) const
{
	return Base::contains(v);
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::VertexIterator
VertexReferences<Vertex, N>::findVertex(const Vertex* v)
{
	return Base::find(v);
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexIterator
VertexReferences<Vertex, N>::findVertex(const Vertex* v) const
{
	return Base::find(v);
}

template<typename Vertex, int N>
int VertexReferences<Vertex, N>::indexOfVertex(const Vertex* v) const
{
	return Base::indexOf(v);
}

template<typename Vertex, int N>
int VertexReferences<Vertex, N>::indexOfEdge(const Vertex* v1, const Vertex* v2)
{
	int vid = indexOfVertex(v1);
	if (vid < 0) {
		return -1;
	}
	else if (vertexMod(vid+1) == v2) {
		return vid;
	}
	else if (vertexMod(vid-1) == v2) {
		unsigned int n = vertexNumber();
		return ((vid-1) % n + n) % n;
	}
	else {
		return -1;
	}
}

template<typename Vertex, int N>
template<int U>
internal::ReturnIfIsVector<U, void> VertexReferences<Vertex, N>::resizeVertices(unsigned int n)
{
	Base::resize(n);
}

template<typename Vertex, int N>
template<int U>
internal::ReturnIfIsVector<U, void> VertexReferences<Vertex, N>::pushVertex(Vertex* v)
{
	Base::pushBack(v);
}

template<typename Vertex, int N>
template<int U>
internal::ReturnIfIsVector<U, void>
VertexReferences<Vertex, N>::insertVertex(unsigned int i, Vertex* v)
{
	Base::insert(i, v);
}

template<typename Vertex, int N>
template<int U>
internal::ReturnIfIsVector<U, void> VertexReferences<Vertex, N>::eraseVertex(unsigned int i)
{
	Base::erase(i);
}

template<typename Vertex, int N>
template<int U>
internal::ReturnIfIsVector<U, void> VertexReferences<Vertex, N>::clearVertices()
{
	Base::clear();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::VertexIterator VertexReferences<Vertex, N>::vertexBegin()
{
	return Base::begin();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::VertexIterator VertexReferences<Vertex, N>::vertexEnd()
{
	return Base::end();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexIterator
VertexReferences<Vertex, N>::vertexBegin() const
{
	return Base::begin();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexIterator
VertexReferences<Vertex, N>::vertexEnd() const
{
	return Base::end();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::VertexRangeIterator VertexReferences<Vertex, N>::vertices()
{
	return Base::rangeIterator();
}

template<typename Vertex, int N>
typename VertexReferences<Vertex, N>::ConstVertexRangeIterator
VertexReferences<Vertex, N>::vertices() const
{
	return Base::rangeIterator();
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	Base::updateElementReferences(oldBase, newBase);
}

template<typename Vertex, int N>
void VertexReferences<Vertex, N>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices);
}

} // namespace vcl::comp
