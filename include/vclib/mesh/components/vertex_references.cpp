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

#include "vertex_references.h"

namespace vcl::comp {

template<typename Vertex, int N, typename El, bool o>
VertexReferences<Vertex, N, El, o>::VertexReferences()
{
	if constexpr (!IS_VERTICAL) {
		init();
	}
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::init()
{
	Base::init(this);
}

template<typename Vertex, int N, typename El, bool o>
bool VertexReferences<Vertex, N, El, o>::isEnabled()
{
	return Base::isEnabled(this);
}

template<typename Vertex, int N, typename El, bool o>
uint VertexReferences<Vertex, N, El, o>::vertexNumber() const
{
	return Base::container(this).size();
}

template<typename Vertex, int N, typename El, bool o>
Vertex*& VertexReferences<Vertex, N, El, o>::vertex(uint i)
{
	return Base::container(this).at(i);
}

template<typename Vertex, int N, typename El, bool o>
const Vertex* VertexReferences<Vertex, N, El, o>::vertex(uint i) const
{
	return Base::container(this).at(i);
}

template<typename Vertex, int N, typename El, bool o>
Vertex*& VertexReferences<Vertex, N, El, o>::vertexMod(int i)
{
	return Base::container(this).atMod(i);
}

template<typename Vertex, int N, typename El, bool o>
const Vertex* VertexReferences<Vertex, N, El, o>::vertexMod(int i) const
{
	return Base::container(this).atMod(i);
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::setVertex(Vertex* v, uint i)
{
	Base::container(this).set(v, i);
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::setVertices(const std::vector<Vertex*>& list)
{
	Base::container(this).set(list);
}

template<typename Vertex, int N, typename El, bool o>
bool VertexReferences<Vertex, N, El, o>::containsVertex(const Vertex* v) const
{
	return Base::container(this).contains(v);
}

template<typename Vertex, int N, typename El, bool o>
typename VertexReferences<Vertex, N, El, o>::VertexIterator
VertexReferences<Vertex, N, El, o>::findVertex(const Vertex* v)
{
	return Base::container(this).find(v);
}

template<typename Vertex, int N, typename El, bool o>
typename VertexReferences<Vertex, N, El, o>::ConstVertexIterator
VertexReferences<Vertex, N, El, o>::findVertex(const Vertex* v) const
{
	return Base::container(this).find(v);
}

template<typename Vertex, int N, typename El, bool o>
int VertexReferences<Vertex, N, El, o>::indexOfVertex(const Vertex* v) const
{
	return Base::container(this).indexOf(v);
}

template<typename Vertex, int N, typename El, bool o>
int VertexReferences<Vertex, N, El, o>::indexOfEdge(const Vertex* v1, const Vertex* v2) const
{
	int vid = indexOfVertex(v1);
	if (vid < 0) {
		return -1;
	}
	else if (vertexMod(vid+1) == v2) {
		return vid;
	}
	else if (vertexMod(vid-1) == v2) {
		uint n = vertexNumber();
		return ((vid-1) % n + n) % n;
	}
	else {
		return -1;
	}
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::resizeVertices(uint n) requires (N < 0)
{
	Base::container(this).resize(n);
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::pushVertex(Vertex* v) requires (N < 0)
{
	Base::container(this).pushBack(v);
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::insertVertex(uint i, Vertex* v) requires (N < 0)
{
	Base::container(this).insert(i, v);
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::eraseVertex(uint i) requires (N < 0)
{
	Base::container(this).erase(i);
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::clearVertices() requires (N < 0)
{
	Base::container(this).clear();
}

template<typename Vertex, int N, typename El, bool o>
typename VertexReferences<Vertex, N, El, o>::VertexIterator VertexReferences<Vertex, N, El, o>::vertexBegin()
{
	return Base::container(this).begin();
}

template<typename Vertex, int N, typename El, bool o>
typename VertexReferences<Vertex, N, El, o>::VertexIterator VertexReferences<Vertex, N, El, o>::vertexEnd()
{
	return Base::container(this).end();
}

template<typename Vertex, int N, typename El, bool o>
typename VertexReferences<Vertex, N, El, o>::ConstVertexIterator
VertexReferences<Vertex, N, El, o>::vertexBegin() const
{
	return Base::container(this).begin();
}

template<typename Vertex, int N, typename El, bool o>
typename VertexReferences<Vertex, N, El, o>::ConstVertexIterator
VertexReferences<Vertex, N, El, o>::vertexEnd() const
{
	return Base::container(this).end();
}

template<typename Vertex, int N, typename El, bool o>
typename VertexReferences<Vertex, N, El, o>::VertexRangeIterator VertexReferences<Vertex, N, El, o>::vertices()
{
	return Base::container(this).rangeIterator();
}

template<typename Vertex, int N, typename El, bool o>
typename VertexReferences<Vertex, N, El, o>::ConstVertexRangeIterator
VertexReferences<Vertex, N, El, o>::vertices() const
{
	return Base::container(this).rangeIterator();
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	Base::updateElementReferences(oldBase, newBase, this);
}

template<typename Vertex, int N, typename El, bool o>
void VertexReferences<Vertex, N, El, o>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices, this);
}

template<typename Vertex, int N, typename El, bool o>
template<typename Element>
void VertexReferences<Vertex, N, El, o>::importFrom(const Element&)
{
}

template<typename Vertex, int N, typename El, bool o>
template<typename Element, typename ElVType>
void VertexReferences<Vertex, N, El, o>::importVertexReferencesFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if constexpr (HasVertexReferences<Element>) {
		if constexpr(N > 0) {
			// same size non-polygonal faces
			if constexpr (N == Element::VERTEX_NUMBER) {
				importReferencesFrom(e, base, ebase);
			}
			// from polygonal to fixed size, but the polygon size == the fixed face size
			else if constexpr (Element::VERTEX_NUMBER < 0){
				if (e.vertexNumber() == N) {
					importReferencesFrom(e, base, ebase);
				}
			}
			else {
				// do not import in this case: cannot import from a face of different size
			}
		}
		else {
			// from fixed to polygonal size: need to resize first, then import
			resizeVertices(e.vertexNumber());
			importReferencesFrom(e, base, ebase);
		}
	}
}

template<typename Vertex, int N, typename El, bool o>
template<typename Element, typename ElVType>
void VertexReferences<Vertex, N, El, o>::importReferencesFrom(
	const Element& e,
	Vertex* base,
	const ElVType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.vertexNumber(); ++i){
			if (e.vertex(i) != nullptr){
				vertex(i) = base + (e.vertex(i) - ebase);
			}
		}
	}
}

} // namespace vcl::comp
