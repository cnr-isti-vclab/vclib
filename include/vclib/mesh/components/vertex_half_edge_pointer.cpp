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

#include "vertex_half_edge_pointer.h"

#include <cstddef>

namespace vcl::comp {

template<typename HE, typename El, bool O>
VertexHalfEdgePointer<HE, El, O>::VertexHalfEdgePointer()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

template<typename HE, typename El, bool O>
void VertexHalfEdgePointer<HE, El, O>::init()
{
	he() = nullptr;
}

template<typename HE, typename El, bool O>
bool VertexHalfEdgePointer<HE, El, O>::isEnabled()
{
	return Base::isEnabled(this);
}

template<typename HE, typename El, bool O>
const HE* VertexHalfEdgePointer<HE, El, O>::halfEdge() const
{
	return he();
}

template<typename HE, typename El, bool O>
HE*& VertexHalfEdgePointer<HE, El, O>::halfEdge()
{
	return he();
}

template<typename HE, typename El, bool O>
uint VertexHalfEdgePointer<HE, El, O>::adjFacesNumber() const
{
	uint cnt = 0;
	for (const Face* f : adjFaces())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::Face*&
VertexHalfEdgePointer<HE, El, O>::adjFace(uint i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
const typename VertexHalfEdgePointer<HE, El, O>::Face*
VertexHalfEdgePointer<HE, El, O>::adjFace(uint i) const
{
	uint                 j  = 0;
	ConstAdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::Face*&
VertexHalfEdgePointer<HE, El, O>::adjFaceMod(int i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it =
		AdjacentFaceIterator(he(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
const typename VertexHalfEdgePointer<HE, El, O>::Face*
VertexHalfEdgePointer<HE, El, O>::adjFaceMod(int i) const
{
	uint                 j  = 0;
	ConstAdjacentFaceIterator it =
		ConstAdjacentFaceIterator(he(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
void VertexHalfEdgePointer<HE, El, O>::setAdjFace(Face* f, uint i)
{
	adjFace(i) = f;
}

template<typename HE, typename El, bool O>
void VertexHalfEdgePointer<HE, El, O>::setAdjFaces(const std::vector<Face*> &list)
{
	assert(list.size() == adjFacesNumber());
	uint i = 0;
	for (Face*& f : adjFaces()) {
		f = list[i++];
	}
}

template<typename HE, typename El, bool O>
bool VertexHalfEdgePointer<HE, El, O>::containsAdjFace(const Face* f) const
{
	return findAdjFace(f) != adjFaceEnd();
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::AdjacentFaceIterator
VertexHalfEdgePointer<HE, El, O>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::ConstAdjacentFaceIterator
VertexHalfEdgePointer<HE, El, O>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool O>
int VertexHalfEdgePointer<HE, El, O>::indexOfAdjFace(const Face* f) const
{
	uint i = 0;
	for (const Face* ff : adjFaces()) {
		if (ff == f)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool O>
uint VertexHalfEdgePointer<HE, El, O>::adjVerticesNumber() const
{
	uint cnt = 0;
	for (const Vertex* f : adjVertices())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::Vertex*&
VertexHalfEdgePointer<HE, El, O>::adjVertex(uint i)
{
	uint                   j  = 0;
	AdjacentVertexIterator it = adjVertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
const typename VertexHalfEdgePointer<HE, El, O>::Vertex*
VertexHalfEdgePointer<HE, El, O>::adjVertex(uint i) const
{
	uint                        j  = 0;
	ConstAdjacentVertexIterator it = adjVertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::Vertex*&
VertexHalfEdgePointer<HE, El, O>::adjVertexMod(int i)
{
	uint                   j  = 0;
	AdjacentVertexIterator it =
		AdjacentVertexIterator(he(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
const typename VertexHalfEdgePointer<HE, El, O>::Vertex*
VertexHalfEdgePointer<HE, El, O>::adjVertexMod(int i) const
{
	uint                 j  = 0;
	ConstAdjacentVertexIterator it =
		ConstAdjacentVertexIterator(he(), nullptr); // this iterator does not have an end
	while (j < (uint)std::abs(i)) {
		if (i > 0)
			++it;
		else
			--it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool O>
void VertexHalfEdgePointer<HE, El, O>::setAdjVertex(Vertex* v, uint i)
{
	adjVertex(i) = v;
}

template<typename HE, typename El, bool O>
void VertexHalfEdgePointer<HE, El, O>::setAdjVertices(const std::vector<Vertex*> &list)
{
	assert(list.size() == adjVerticesNumber());
	uint i = 0;
	for (Vertex*& v : adjVertices()) {
		v = list[i++];
	}
}

template<typename HE, typename El, bool O>
bool VertexHalfEdgePointer<HE, El, O>::containsAdjVertex(const Vertex* v) const
{
	return findAdjVertex(v) != adjVertexEnd();
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::AdjacentVertexIterator
VertexHalfEdgePointer<HE, El, O>::findAdjVertex(const Vertex* v)
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::ConstAdjacentVertexIterator
VertexHalfEdgePointer<HE, El, O>::findAdjVertex(const Vertex* v) const
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename HE, typename El, bool O>
int VertexHalfEdgePointer<HE, El, O>::indexOfAdjVertex(const Vertex* v) const
{
	uint i = 0;
	for (const Vertex* vv : adjVertices()) {
		if (vv == v)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::AdjacentFaceIterator
VertexHalfEdgePointer<HE, El, O>::adjFaceBegin()
{
	return AdjacentFaceIterator(he());
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::AdjacentFaceIterator
VertexHalfEdgePointer<HE, El, O>::adjFaceEnd()
{
	return AdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::ConstAdjacentFaceIterator
VertexHalfEdgePointer<HE, El, O>::adjFaceBegin() const
{
	return ConstAdjacentFaceIterator(he());
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::ConstAdjacentFaceIterator
VertexHalfEdgePointer<HE, El, O>::adjFaceEnd() const
{
	return ConstAdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool O>
auto VertexHalfEdgePointer<HE, El, O>::adjFaces()
{
	return View(adjFaceBegin(), adjFaceEnd());
}

template<typename HE, typename El, bool O>
auto VertexHalfEdgePointer<HE, El, O>::adjFaces() const
{
	return View(adjFaceBegin(), adjFaceEnd());
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::AdjacentVertexIterator
VertexHalfEdgePointer<HE, El, O>::adjVertexBegin()
{
	return AdjacentVertexIterator(he());
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::AdjacentVertexIterator
VertexHalfEdgePointer<HE, El, O>::adjVertexEnd()
{
	return AdjacentVertexIterator(nullptr);
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::ConstAdjacentVertexIterator
VertexHalfEdgePointer<HE, El, O>::adjVertexBegin() const
{
	return ConstAdjacentVertexIterator(he());
}

template<typename HE, typename El, bool O>
typename VertexHalfEdgePointer<HE, El, O>::ConstAdjacentVertexIterator
VertexHalfEdgePointer<HE, El, O>::adjVertexEnd() const
{
	return ConstAdjacentVertexIterator(nullptr);
}

template<typename HE, typename El, bool O>
auto VertexHalfEdgePointer<HE, El, O>::adjVertices()
{
	return View(adjVertexBegin(), adjVertexEnd());
}

template<typename HE, typename El, bool O>
auto VertexHalfEdgePointer<HE, El, O>::adjVertices() const
{
	return View(adjVertexBegin(), adjVertexEnd());
}

template<typename HE, typename El, bool O>
template<typename Element>
void VertexHalfEdgePointer<HE, El, O>::importFrom(const Element&)
{
}

template<typename HE, typename El, bool O>
template<typename OtherVertex, typename OtherHEType>
void VertexHalfEdgePointer<HE, El, O>::importPointersFrom(
	const OtherVertex& e,
	HE*          base,
	const OtherHEType* ebase)
{
	if constexpr (HasVertexHalfEdgePointer<OtherVertex>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.halfEdge() != nullptr) {
				he() = base + (e.halfEdge() - ebase);
			}
		}
	}
}

template<typename HE, typename El, bool O>
void VertexHalfEdgePointer<HE, El, O>::updatePointers(
	const HE* oldBase,
	const HE* newBase)
{
	if (he() != nullptr) {
		size_t diff = he() - oldBase;
		he() = (HE*)newBase + diff;
	}
}

template<typename HE, typename El, bool O>
void VertexHalfEdgePointer<HE, El, O>::updatePointersAfterCompact(
	const HE*         base,
	const std::vector<int>& newIndices)
{
	if (he() != nullptr) {
		size_t diff = he() - base;
		if (newIndices[diff] < 0)
			he() = nullptr;
		else
			he() = (HE*)base + newIndices[diff];
	}
}

template<typename HE, typename El, bool O>
HE*& VertexHalfEdgePointer<HE, El, O>::he()
{
	return Base::data(this);
}

template<typename HE, typename El, bool O>
const HE* VertexHalfEdgePointer<HE, El, O>::he() const
{
	return Base::data(this);
}

} // namespace vcl::comp
