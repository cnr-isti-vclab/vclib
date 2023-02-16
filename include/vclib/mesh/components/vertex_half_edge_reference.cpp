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

#include "vertex_half_edge_reference.h"

#include <cstddef>

namespace vcl::comp {

template<typename HE, typename El, bool o>
VertexHalfEdgeReference<HE, El, o>::VertexHalfEdgeReference()
{
	if constexpr (!IS_VERTICAL) {
		init();
	}
}

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::init()
{
	he() = nullptr;
}

template<typename HE, typename El, bool o>
bool VertexHalfEdgeReference<HE, El, o>::isEnabled()
{
	return data.template isComponentEnabled<El>(this);
}

template<typename HE, typename El, bool o>
const HE* VertexHalfEdgeReference<HE, El, o>::halfEdge() const
{
	return he();
}

template<typename HE, typename El, bool o>
HE*& VertexHalfEdgeReference<HE, El, o>::halfEdge()
{
	return he();
}

template<typename HE, typename El, bool o>
uint VertexHalfEdgeReference<HE, El, o>::adjFacesNumber() const
{
	uint cnt = 0;
	for (const Face* f : adjFaces())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::Face*&
VertexHalfEdgeReference<HE, El, o>::adjFace(uint i)
{
	uint                 j  = 0;
	AdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool o>
const typename VertexHalfEdgeReference<HE, El, o>::Face*
VertexHalfEdgeReference<HE, El, o>::adjFace(uint i) const
{
	uint                 j  = 0;
	ConstAdjacentFaceIterator it = adjFaceBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::Face*&
VertexHalfEdgeReference<HE, El, o>::adjFaceMod(int i)
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

template<typename HE, typename El, bool o>
const typename VertexHalfEdgeReference<HE, El, o>::Face*
VertexHalfEdgeReference<HE, El, o>::adjFaceMod(int i) const
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

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::setAdjFace(Face* f, uint i)
{
	adjFace(i) = f;
}

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::setAdjFaces(const std::vector<Face*> &list)
{
	assert(list.size() == adjFacesNumber());
	uint i = 0;
	for (Face*& f : adjFaces()) {
		f = list[i++];
	}
}

template<typename HE, typename El, bool o>
bool VertexHalfEdgeReference<HE, El, o>::containsAdjFace(const Face* f) const
{
	return findAdjFace(f) != adjFaceEnd();
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::AdjacentFaceIterator
VertexHalfEdgeReference<HE, El, o>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HE, El, o>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename HE, typename El, bool o>
int VertexHalfEdgeReference<HE, El, o>::indexOfAdjFace(const Face* f) const
{
	uint i = 0;
	for (const Face* ff : adjFaces()) {
		if (ff == f)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool o>
uint VertexHalfEdgeReference<HE, El, o>::adjVerticesNumber() const
{
	uint cnt = 0;
	for (const Vertex* f : adjVertices())
		++cnt;
	return cnt;
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::Vertex*&
VertexHalfEdgeReference<HE, El, o>::adjVertex(uint i)
{
	uint                   j  = 0;
	AdjacentVertexIterator it = adjVertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool o>
const typename VertexHalfEdgeReference<HE, El, o>::Vertex*
VertexHalfEdgeReference<HE, El, o>::adjVertex(uint i) const
{
	uint                        j  = 0;
	ConstAdjacentVertexIterator it = adjVertexBegin();
	while (j < i) {
		++it;
		++j;
	}
	return *it;
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::Vertex*&
VertexHalfEdgeReference<HE, El, o>::adjVertexMod(int i)
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

template<typename HE, typename El, bool o>
const typename VertexHalfEdgeReference<HE, El, o>::Vertex*
VertexHalfEdgeReference<HE, El, o>::adjVertexMod(int i) const
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

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::setAdjVertex(Vertex* v, uint i)
{
	adjVertex(i) = v;
}

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::setAdjVertices(const std::vector<Vertex*> &list)
{
	assert(list.size() == adjVerticesNumber());
	uint i = 0;
	for (Vertex*& v : adjVertices()) {
		v = list[i++];
	}
}

template<typename HE, typename El, bool o>
bool VertexHalfEdgeReference<HE, El, o>::containsAdjVertex(const Vertex* v) const
{
	return findAdjVertex(v) != adjVertexEnd();
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::AdjacentVertexIterator
VertexHalfEdgeReference<HE, El, o>::findAdjVertex(const Vertex* v)
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HE, El, o>::findAdjVertex(const Vertex* v) const
{
	return std::find(adjVertexBegin(), adjVertexEnd(), v);
}

template<typename HE, typename El, bool o>
int VertexHalfEdgeReference<HE, El, o>::indexOfAdjVertex(const Vertex* v) const
{
	uint i = 0;
	for (const Vertex* vv : adjVertices()) {
		if (vv == v)
			return i;
		i++;
	}
	return -1;
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::AdjacentFaceIterator
VertexHalfEdgeReference<HE, El, o>::adjFaceBegin()
{
	return AdjacentFaceIterator(he());
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::AdjacentFaceIterator
VertexHalfEdgeReference<HE, El, o>::adjFaceEnd()
{
	return AdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HE, El, o>::adjFaceBegin() const
{
	return ConstAdjacentFaceIterator(he());
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::ConstAdjacentFaceIterator
VertexHalfEdgeReference<HE, El, o>::adjFaceEnd() const
{
	return ConstAdjacentFaceIterator(nullptr);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::AdjacentFaceRangeIterator
VertexHalfEdgeReference<HE, El, o>::adjFaces()
{
	return AdjacentFaceRangeIterator(
		*this, &VertexHalfEdgeReference::adjFaceBegin, &VertexHalfEdgeReference::adjFaceEnd);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::ConstAdjacentFaceRangeIterator
VertexHalfEdgeReference<HE, El, o>::adjFaces() const
{
	return ConstAdjacentFaceRangeIterator(
		*this, &VertexHalfEdgeReference::adjFaceBegin, &VertexHalfEdgeReference::adjFaceEnd);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::AdjacentVertexIterator
VertexHalfEdgeReference<HE, El, o>::adjVertexBegin()
{
	return AdjacentVertexIterator(he());
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::AdjacentVertexIterator
VertexHalfEdgeReference<HE, El, o>::adjVertexEnd()
{
	return AdjacentVertexIterator(nullptr);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HE, El, o>::adjVertexBegin() const
{
	return ConstAdjacentVertexIterator(he());
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::ConstAdjacentVertexIterator
VertexHalfEdgeReference<HE, El, o>::adjVertexEnd() const
{
	return ConstAdjacentVertexIterator(nullptr);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::AdjacentVertexRangeIterator
VertexHalfEdgeReference<HE, El, o>::adjVertices()
{
	return AdjacentVertexRangeIterator(
		*this, &VertexHalfEdgeReference::adjVertexBegin, &VertexHalfEdgeReference::adjVertexEnd);
}

template<typename HE, typename El, bool o>
typename VertexHalfEdgeReference<HE, El, o>::ConstAdjacentVertexRangeIterator
VertexHalfEdgeReference<HE, El, o>::adjVertices() const
{
	return ConstAdjacentVertexRangeIterator(
		*this, &VertexHalfEdgeReference::adjVertexBegin, &VertexHalfEdgeReference::adjVertexEnd);
}

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::updateHalfEdgeReferences(
	const HE* oldBase,
	const HE* newBase)
{
	if (he() != nullptr) {
		size_t diff = he() - oldBase;
		he() = (HE*)newBase + diff;
	}
}

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::updateHalfEdgeReferencesAfterCompact(
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

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::updateVertexReferences(const Vertex*, const Vertex*)
{
}

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::updateVertexReferencesAfterCompact(
	const Vertex*,
	const std::vector<int>&)
{
}

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::updateFaceReferences(const Face*, const Face*)
{
}

template<typename HE, typename El, bool o>
void VertexHalfEdgeReference<HE, El, o>::updateFaceReferencesAfterCompact(
	const Face*,
	const std::vector<int>&)
{
}

template<typename HE, typename El, bool o>
template<typename Element>
void VertexHalfEdgeReference<HE, El, o>::importFrom(const Element&)
{
}

template<typename HE, typename El, bool o>
template<typename OtherVertex, typename OtherHEType>
void VertexHalfEdgeReference<HE, El, o>::importHalfEdgeReferencesFrom(
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

template<typename HE, typename El, bool o>
HE*& VertexHalfEdgeReference<HE, El, o>::he()
{
	return data.template get<El>(this);
}

template<typename HE, typename El, bool o>
const HE* VertexHalfEdgeReference<HE, El, o>::he() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
