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

#include "vertex_half_edge_reference.h"

#include <cstddef>

namespace vcl::comp {

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::init()
{
	he() = nullptr;
}

template<typename HE, typename El, bool h, bool o>
const HE* VertexHalfEdgeReference<HE, El, h, o>::halfEdge() const
{
	return he();
}

template<typename HE, typename El, bool h, bool o>
HE*& VertexHalfEdgeReference<HE, El, h, o>::halfEdge()
{
	return he();
}

template<typename HE, typename El, bool h, bool o>
uint VertexHalfEdgeReference<HE, El, h, o>::adjFacesNumber() const
{
	uint cnt = 0;
	for (const Face* f : adjFaces())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::Face*&
VertexHalfEdgeReference<HE, El, h, o>::adjFace(uint i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
const typename VertexHalfEdgeReference<HE, El, h, o>::Face*
VertexHalfEdgeReference<HE, El, h, o>::adjFace(uint i) const
{
	uint                 j  = 0;
	ConstAdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::Face*&
VertexHalfEdgeReference<HE, El, h, o>::adjFaceMod(int i)
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

template<typename HE, typename El, bool h, bool o>
const typename VertexHalfEdgeReference<HE, El, h, o>::Face*
VertexHalfEdgeReference<HE, El, h, o>::adjFaceMod(int i) const
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

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::setAdjFace(Face* f, uint i)
{
	adjFace(i) = f;
}

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::setAdjFaces(const std::vector<Face*> &list)
{
	assert(list.size() == adjFacesNumber());
	uint i = 0;
	for (Face*& f : adjFaces()) {
		f = list[i++];
	}
}

template<typename HE, typename El, bool h, bool o>
bool VertexHalfEdgeReference<HE, El, h, o>::containsAdjFace(const Face* f) const
{
	return findAdjFace(f) != adjFaceEnd();
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::AdjacentFaceIterator
VertexHalfEdgeReference<HE, El, h, o>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HE, El, h, o>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool h, bool o>
int VertexHalfEdgeReference<HE, El, h, o>::indexOfAdjFace(const Face* f) const
{
	uint i = 0;
	for (const Face* ff : adjFaces()) {
		if (ff == f)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool h, bool o>
uint VertexHalfEdgeReference<HE, El, h, o>::adjVerticesNumber() const
{
	uint cnt = 0;
	for (const Vertex* f : adjVertices())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::Vertex*&
VertexHalfEdgeReference<HE, El, h, o>::adjVertex(uint i)
{
	uint                   j  = 0;
	AdjacentVertexIterator it = adjVertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
const typename VertexHalfEdgeReference<HE, El, h, o>::Vertex*
VertexHalfEdgeReference<HE, El, h, o>::adjVertex(uint i) const
{
	uint                        j  = 0;
	ConstAdjacentVertexIterator it = adjVertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::Vertex*&
VertexHalfEdgeReference<HE, El, h, o>::adjVertexMod(int i)
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

template<typename HE, typename El, bool h, bool o>
const typename VertexHalfEdgeReference<HE, El, h, o>::Vertex*
VertexHalfEdgeReference<HE, El, h, o>::adjVertexMod(int i) const
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

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::setAdjVertex(Vertex* v, uint i)
{
	adjVertex(i) = v;
}

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::setAdjVertices(const std::vector<Vertex*> &list)
{
	assert(list.size() == adjVerticesNumber());
	uint i = 0;
	for (Vertex*& v : adjVertices()) {
		v = list[i++];
	}
}

template<typename HE, typename El, bool h, bool o>
bool VertexHalfEdgeReference<HE, El, h, o>::containsAdjVertex(const Vertex* v) const
{
	return findAdjVertex(v) != adjVertexEnd();
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::AdjacentVertexIterator
VertexHalfEdgeReference<HE, El, h, o>::findAdjVertex(const Vertex* v)
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HE, El, h, o>::findAdjVertex(const Vertex* v) const
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename HE, typename El, bool h, bool o>
int VertexHalfEdgeReference<HE, El, h, o>::indexOfAdjVertex(const Vertex* v) const
{
	uint i = 0;
	for (const Vertex* vv : adjVertices()) {
		if (vv == v)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::AdjacentFaceIterator
VertexHalfEdgeReference<HE, El, h, o>::adjFaceBegin()
{
	return AdjacentFaceIterator(he());
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::AdjacentFaceIterator
VertexHalfEdgeReference<HE, El, h, o>::adjFaceEnd()
{
	return AdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HE, El, h, o>::adjFaceBegin() const
{
	return ConstAdjacentFaceIterator(he());
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HE, El, h, o>::adjFaceEnd() const
{
	return ConstAdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::AdjacentFaceRangeIterator
VertexHalfEdgeReference<HE, El, h, o>::adjFaces()
{
	return AdjacentFaceRangeIterator(
		*this, &VertexHalfEdgeReference::adjFaceBegin, &VertexHalfEdgeReference::adjFaceEnd);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::ConstAdjacentFaceRangeIterator
VertexHalfEdgeReference<HE, El, h, o>::adjFaces() const
{
	return ConstAdjacentFaceRangeIterator(
		*this, &VertexHalfEdgeReference::adjFaceBegin, &VertexHalfEdgeReference::adjFaceEnd);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::AdjacentVertexIterator
VertexHalfEdgeReference<HE, El, h, o>::adjVertexBegin()
{
	return AdjacentVertexIterator(he());
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::AdjacentVertexIterator
VertexHalfEdgeReference<HE, El, h, o>::adjVertexEnd()
{
	return AdjacentVertexIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HE, El, h, o>::adjVertexBegin() const
{
	return ConstAdjacentVertexIterator(he());
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HE, El, h, o>::adjVertexEnd() const
{
	return ConstAdjacentVertexIterator(nullptr);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::AdjacentVertexRangeIterator
VertexHalfEdgeReference<HE, El, h, o>::adjVertices()
{
	return AdjacentVertexRangeIterator(
		*this, &VertexHalfEdgeReference::adjVertexBegin, &VertexHalfEdgeReference::adjVertexEnd);
}

template<typename HE, typename El, bool h, bool o>
typename VertexHalfEdgeReference<HE, El, h, o>::ConstAdjacentVertexRangeIterator
VertexHalfEdgeReference<HE, El, h, o>::adjVertices() const
{
	return ConstAdjacentVertexRangeIterator(
		*this, &VertexHalfEdgeReference::adjVertexBegin, &VertexHalfEdgeReference::adjVertexEnd);
}

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::updateHalfEdgeReferences(
	const HE* oldBase,
	const HE* newBase)
{
	if (he() != nullptr) {
		size_t diff = he() - oldBase;
		he() = (HE*)newBase + diff;
	}
}

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::updateHalfEdgeReferencesAfterCompact(
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

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::updateVertexReferences(const Vertex*, const Vertex*)
{
}

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::updateVertexReferencesAfterCompact(
	const Vertex*,
	const std::vector<int>&)
{
}

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::updateFaceReferences(const Face*, const Face*)
{
}

template<typename HE, typename El, bool h, bool o>
void VertexHalfEdgeReference<HE, El, h, o>::updateFaceReferencesAfterCompact(
	const Face*,
	const std::vector<int>&)
{
}

template<typename HE, typename El, bool h, bool o>
template<typename Element>
void VertexHalfEdgeReference<HE, El, h, o>::importFrom(const Element&)
{
}

template<typename HE, typename El, bool h, bool o>
template<typename OtherVertex, typename OtherHEType>
void VertexHalfEdgeReference<HE, El, h, o>::importHalfEdgeReferencesFrom(
	const OtherVertex& e,
	HE*          base,
	const OtherHEType* ebase)
{
	if constexpr (HasVertexHalfEdgeReference<OtherVertex>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.halfEdge() != nullptr) {
				he() = base + (e.halfEdge() - ebase);
			}
		}
	}
}

template<typename HE, typename El, bool h, bool o>
HE*& VertexHalfEdgeReference<HE, El, h, o>::he()
{
	return data.template get<El>(this);
}

template<typename HE, typename El, bool h, bool o>
const HE* VertexHalfEdgeReference<HE, El, h, o>::he() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
