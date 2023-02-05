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

#include "half_edge_references.h"

#include <cstddef>

namespace vcl::comp {

template<typename HalfEdge, typename Vertex, typename Face>
HalfEdgeReferences<HalfEdge, Vertex, Face>::HalfEdgeReferences()
{
}

template<typename HalfEdge, typename Vertex, typename Face>
const HalfEdge* HalfEdgeReferences<HalfEdge, Vertex, Face>::next() const
{
	return n;
}

template<typename HalfEdge, typename Vertex, typename Face>
HalfEdge*& HalfEdgeReferences<HalfEdge, Vertex, Face>::next()
{
	return n;
}

template<typename HalfEdge, typename Vertex, typename Face>
const HalfEdge* HalfEdgeReferences<HalfEdge, Vertex, Face>::prev() const
{
	return p;
}

template<typename HalfEdge, typename Vertex, typename Face>
HalfEdge*& HalfEdgeReferences<HalfEdge, Vertex, Face>::prev()
{
	return p;
}

template<typename HalfEdge, typename Vertex, typename Face>
const HalfEdge* HalfEdgeReferences<HalfEdge, Vertex, Face>::twin() const
{
	return t;
}

template<typename HalfEdge, typename Vertex, typename Face>
HalfEdge*& HalfEdgeReferences<HalfEdge, Vertex, Face>::twin()
{
	return t;
}

template<typename HalfEdge, typename Vertex, typename Face>
const Vertex* HalfEdgeReferences<HalfEdge, Vertex, Face>::fromVertex() const
{
	return v;
}

template<typename HalfEdge, typename Vertex, typename Face>
Vertex*& HalfEdgeReferences<HalfEdge, Vertex, Face>::fromVertex()
{
	return v;
}

template<typename HalfEdge, typename Vertex, typename Face>
const Vertex* HalfEdgeReferences<HalfEdge, Vertex, Face>::toVertex() const
{
	return n->fromVertex();
}

template<typename HalfEdge, typename Vertex, typename Face>
Vertex*& HalfEdgeReferences<HalfEdge, Vertex, Face>::toVertex()
{
	return n->fromVertex();
}

template<typename HalfEdge, typename Vertex, typename Face>
const Vertex* HalfEdgeReferences<HalfEdge, Vertex, Face>::vertex(uint i) const
{
	switch(i) {
	case 0: return v;
	case 1: return toVertex();
	default: assert(0); return nullptr;
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
Vertex*& HalfEdgeReferences<HalfEdge, Vertex, Face>::vertex(uint i)
{
	switch(i) {
	case 0: return v;
	case 1: return toVertex();
	default: assert(0); return nullptr;
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
const Face* HalfEdgeReferences<HalfEdge, Vertex, Face>::face() const
{
	return f;
}

template<typename HalfEdge, typename Vertex, typename Face>
Face*& HalfEdgeReferences<HalfEdge, Vertex, Face>::face()
{
	return f;
}

template<typename HalfEdge, typename Vertex, typename Face>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::updateHalfEdgeReferences(
	const HalfEdge* oldBase,
	const HalfEdge* newBase)
{
	if (n != nullptr) {
		size_t diff = n - oldBase;
		n = (HalfEdge*)newBase + diff;
	}
	if (p != nullptr) {
		size_t diff = p - oldBase;
		p = (HalfEdge*)newBase + diff;
	}
	if (t != nullptr) {
		size_t diff = t - oldBase;
		t = (HalfEdge*)newBase + diff;
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::updateHalfEdgeReferencesAfterCompact(
	const HalfEdge*         base,
	const std::vector<int>& newIndices)
{
	if (n != nullptr) {
		size_t diff = n - base;
		if (newIndices[diff] < 0)
			n = nullptr;
		else
			n = (HalfEdge*)base + newIndices[diff];
	}
	if (p != nullptr) {
		size_t diff = p - base;
		if (newIndices[diff] < 0)
			p = nullptr;
		else
			p = (HalfEdge*)base + newIndices[diff];
	}
	if (t != nullptr) {
		size_t diff = t - base;
		if (newIndices[diff] < 0)
			t = nullptr;
		else
			t = (HalfEdge*)base + newIndices[diff];
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::updateFaceReferences(
	const Face* oldBase,
	const Face* newBase)
{
	if (f != nullptr) {
		size_t diff = f - oldBase;
		f = (Face*)newBase + diff;
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::updateFaceReferencesAfterCompact(
	const Face*             base,
	const std::vector<int>& newIndices)
{
	if (f != nullptr) {
		size_t diff = f - base;
		if (newIndices[diff] < 0)
			f = nullptr;
		else
			f = base + newIndices[diff];
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	if (v != nullptr) {
		size_t diff = v - oldBase;
		v = (Vertex*)newBase + diff;
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	if (v != nullptr) {
		size_t diff = v - base;
		if (newIndices[diff] < 0)
			v = nullptr;
		else
			v = base + newIndices[diff];
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
template<typename Element>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::importFrom(const Element &e)
{
}

/**
 * @brief Import the half edge references from another half edge (e), which is of a different type.
 *
 * @param e: the half edge from which import the half edge references
 * @param base: the base of this container: necessary to compute the imported references
 * @param ebase: the base of the other container of half edges, from which we import the references
 */
template<typename HalfEdge, typename Vertex, typename Face>
template<typename HE, typename HEType>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::importHalfEdgeReferencesFrom(
	const HE&     e,
	HalfEdge*     base,
	const HEType* ebase)
{
	if constexpr (HasHalfEdgeReferences<HE>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.next() != nullptr) { // if the other half edge has a next
				// this next will be the base of this container plus the offset between the next of
				// the other half edge and the base of the other container
				n = base + (e.next() - ebase);
			}
			if (e.prev() != nullptr) {
				p = base + (e.prev() - ebase);
			}
			if (e.twin() != nullptr) {
				t = base + (e.twin() - ebase);
			}
		}
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
template<typename HE, typename VType>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::importVertexReferencesFrom(
	const HE&    e,
	Vertex*      base,
	const VType* ebase)
{
	if constexpr (HasHalfEdgeReferences<HE>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.fromVertex() != nullptr) {
				v = base + (e.fromVertex() - ebase);
			}
		}
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
template<typename HE, typename FType>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::importFaceReferencesFrom(
	const HE&    e,
	Face*        base,
	const FType* ebase)
{
	if constexpr (HasHalfEdgeReferences<HE>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.face() != nullptr) {
				f = base + (e.face() - ebase);
			}
		}
	}
}

} // namespace vcl::comp
