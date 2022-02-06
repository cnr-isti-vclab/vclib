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
Face* HalfEdgeReferences<HalfEdge, Vertex, Face>::face()
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
		n = newBase + diff;
	}
	if (p != nullptr) {
		size_t diff = p - oldBase;
		p = newBase + diff;
	}
	if (t != nullptr) {
		size_t diff = t - oldBase;
		t = newBase + diff;
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
			n = base + newIndices[diff];
	}
	if (p != nullptr) {
		size_t diff = p - base;
		if (newIndices[diff] < 0)
			p = nullptr;
		else
			p = base + newIndices[diff];
	}
	if (t != nullptr) {
		size_t diff = t - base;
		if (newIndices[diff] < 0)
			t = nullptr;
		else
			t = base + newIndices[diff];
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
void HalfEdgeReferences<HalfEdge, Vertex, Face>::updateFaceReferences(
	const Face* oldBase,
	const Face* newBase)
{
	if (f != nullptr) {
		size_t diff = f - oldBase;
		f = newBase + diff;
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
		v = newBase + diff;
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

} // namespace vcl::comp
