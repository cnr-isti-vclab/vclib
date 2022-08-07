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

#include "vertex_half_edge_reference.h"

#include <cstddef>

namespace vcl::comp {

template<typename HalfEdge>
VertexHalfEdgeReference<HalfEdge>::VertexHalfEdgeReference()
{
}

template<typename HalfEdge>
const HalfEdge *VertexHalfEdgeReference<HalfEdge>::halfEdge() const
{
	return he;
}

template<typename HalfEdge>
HalfEdge *&VertexHalfEdgeReference<HalfEdge>::halfEdge()
{
	return he;
}

template<typename HalfEdge>
uint VertexHalfEdgeReference<HalfEdge>::adjFacesNumber() const
{
	uint cnt = 0;
	for (const Face* f : adjFaces())
		++cnt;
	return cnt;
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::Face*&
VertexHalfEdgeReference<HalfEdge>::adjFace(uint i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const typename VertexHalfEdgeReference<HalfEdge>::Face*
VertexHalfEdgeReference<HalfEdge>::adjFace(uint i) const
{
	uint                 j  = 0;
	ConstAdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::Face*&
VertexHalfEdgeReference<HalfEdge>::adjFaceMod(int i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it =
		AdjacentFaceIterator(he, nullptr); // this iterator does not have an end
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
const typename VertexHalfEdgeReference<HalfEdge>::Face*
VertexHalfEdgeReference<HalfEdge>::adjFaceMod(int i) const
{
	uint                 j  = 0;
	ConstAdjacentFaceIterator it =
		ConstAdjacentFaceIterator(he, nullptr); // this iterator does not have an end
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
void VertexHalfEdgeReference<HalfEdge>::setAdjFace(Face* f, uint i)
{
	adjFace(i) = f;
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::setAdjFaces(const std::vector<Face*> &list)
{
	assert(list.size() == adjFacesNumber());
	uint i = 0;
	for (Face*& f : adjFaces()) {
		f = list[i++];
	}
}

template<typename HalfEdge>
bool VertexHalfEdgeReference<HalfEdge>::containsAdjFace(const Face* f) const
{
	return findAdjFace(f) != adjFaceEnd();
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::AdjacentFaceIterator
VertexHalfEdgeReference<HalfEdge>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HalfEdge>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HalfEdge>
int VertexHalfEdgeReference<HalfEdge>::indexOfAdjFace(const Face* f) const
{
	uint i = 0;
	for (const Face* ff : adjFaces()) {
		if (ff == f)
			return i;
		i++;
	}
	return -1;
}

template<typename HalfEdge>
uint VertexHalfEdgeReference<HalfEdge>::adjVerticesNumber() const
{
	uint cnt = 0;
	for (const Vertex* f : adjVertices())
		++cnt;
	return cnt;
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::Vertex*&
VertexHalfEdgeReference<HalfEdge>::adjVertex(uint i)
{
	uint                   j  = 0;
	AdjacentVertexIterator it = adjVertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
const typename VertexHalfEdgeReference<HalfEdge>::Vertex*
VertexHalfEdgeReference<HalfEdge>::adjVertex(uint i) const
{
	uint                        j  = 0;
	ConstAdjacentVertexIterator it = adjVertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::Vertex*&
VertexHalfEdgeReference<HalfEdge>::adjVertexMod(int i)
{
	uint                   j  = 0;
	AdjacentVertexIterator it =
		AdjacentVertexIterator(he, nullptr); // this iterator does not have an end
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
const typename VertexHalfEdgeReference<HalfEdge>::Vertex*
VertexHalfEdgeReference<HalfEdge>::adjVertexMod(int i) const
{
	uint                 j  = 0;
	ConstAdjacentVertexIterator it =
		ConstAdjacentVertexIterator(he, nullptr); // this iterator does not have an end
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
void VertexHalfEdgeReference<HalfEdge>::setAdjVertex(Vertex* v, uint i)
{
	adjVertex(i) = v;
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::setAdjVertices(const std::vector<Vertex*> &list)
{
	assert(list.size() == adjVerticesNumber());
	uint i = 0;
	for (Vertex*& v : adjVertices()) {
		v = list[i++];
	}
}

template<typename HalfEdge>
bool VertexHalfEdgeReference<HalfEdge>::containsAdjVertex(const Vertex* v) const
{
	return findAdjVertex(v) != adjVertexEnd();
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::AdjacentVertexIterator
VertexHalfEdgeReference<HalfEdge>::findAdjVertex(const Vertex* v)
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HalfEdge>::findAdjVertex(const Vertex* v) const
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename HalfEdge>
int VertexHalfEdgeReference<HalfEdge>::indexOfAdjVertex(const Vertex* v) const
{
	uint i = 0;
	for (const Vertex* vv : adjVertices()) {
		if (vv == v)
			return i;
		i++;
	}
	return -1;
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::AdjacentFaceIterator
VertexHalfEdgeReference<HalfEdge>::adjFaceBegin()
{
	return AdjacentFaceIterator(he);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::AdjacentFaceIterator
VertexHalfEdgeReference<HalfEdge>::adjFaceEnd()
{
	return AdjacentFaceIterator(nullptr);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HalfEdge>::adjFaceBegin() const
{
	return ConstAdjacentFaceIterator(he);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HalfEdge>::adjFaceEnd() const
{
	return ConstAdjacentFaceIterator(nullptr);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::AdjacentFaceRangeIterator
VertexHalfEdgeReference<HalfEdge>::adjFaces()
{
	return AdjacentFaceRangeIterator(
		*this, &VertexHalfEdgeReference::adjFaceBegin, &VertexHalfEdgeReference::adjFaceEnd);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::ConstAdjacentFaceRangeIterator
VertexHalfEdgeReference<HalfEdge>::adjFaces() const
{
	return ConstAdjacentFaceRangeIterator(
		*this, &VertexHalfEdgeReference::adjFaceBegin, &VertexHalfEdgeReference::adjFaceEnd);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::AdjacentVertexIterator
VertexHalfEdgeReference<HalfEdge>::adjVertexBegin()
{
	return AdjacentVertexIterator(he);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::AdjacentVertexIterator
VertexHalfEdgeReference<HalfEdge>::adjVertexEnd()
{
	return AdjacentVertexIterator(nullptr);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HalfEdge>::adjVertexBegin() const
{
	return ConstAdjacentVertexIterator(he);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HalfEdge>::adjVertexEnd() const
{
	return ConstAdjacentVertexIterator(nullptr);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::AdjacentVertexRangeIterator
VertexHalfEdgeReference<HalfEdge>::adjVertices()
{
	return AdjacentVertexRangeIterator(
		*this, &VertexHalfEdgeReference::adjVertexBegin, &VertexHalfEdgeReference::adjVertexEnd);
}

template<typename HalfEdge>
typename VertexHalfEdgeReference<HalfEdge>::ConstAdjacentVertexRangeIterator
VertexHalfEdgeReference<HalfEdge>::adjVertices() const
{
	return ConstAdjacentVertexRangeIterator(
		*this, &VertexHalfEdgeReference::adjVertexBegin, &VertexHalfEdgeReference::adjVertexEnd);
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::updateHalfEdgeReferences(
	const HalfEdge* oldBase,
	const HalfEdge* newBase)
{
	if (he != nullptr) {
		size_t diff = he - oldBase;
		he = (HalfEdge*)newBase + diff;
	}
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::updateHalfEdgeReferencesAfterCompact(
	const HalfEdge*         base,
	const std::vector<int>& newIndices)
{
	if (he != nullptr) {
		size_t diff = he - base;
		if (newIndices[diff] < 0)
			he = nullptr;
		else
			he = (HalfEdge*)base + newIndices[diff];
	}
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::updateVertexReferences(const Vertex *, const Vertex *)
{
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::updateVertexReferencesAfterCompact(
	const Vertex*,
	const std::vector<int>&)
{
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::updateFaceReferences(const Face*, const Face*)
{
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::updateFaceReferencesAfterCompact(
	const Face*,
	const std::vector<int>&)
{
}

template<typename HalfEdge>
template<typename Element>
void VertexHalfEdgeReference<HalfEdge>::importFrom(const Element &e)
{
}

template<typename HalfEdge>
template<typename OtherVertex, typename OtherHEType>
void VertexHalfEdgeReference<HalfEdge>::importHalfEdgeReferencesFrom(
	const OtherVertex& e,
	HalfEdge*          base,
	const OtherHEType* ebase)
{
	if constexpr (HasVertexHalfEdgeReference<OtherVertex>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.halfEdge() != nullptr) {
				he = base + (e.halfEdge() - ebase);
			}
		}
	}
}

} // namespace vcl::comp
