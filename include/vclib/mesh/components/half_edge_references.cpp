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

template<typename HE, typename V, typename F, typename El, bool h>
void HalfEdgeReferences<HE, V, F, El, h>::init()
{
	n() = nullptr;
	p() = nullptr;
	t() = nullptr;
	v() = nullptr;
	f() = nullptr;
}

template<typename HE, typename V, typename F, typename El, bool h>
const HE* HalfEdgeReferences<HE, V, F, El, h>::next() const
{
	return n();
}

template<typename HE, typename V, typename F, typename El, bool h>
HE*& HalfEdgeReferences<HE, V, F, El, h>::next()
{
	return n();
}

template<typename HE, typename V, typename F, typename El, bool h>
const HE* HalfEdgeReferences<HE, V, F, El, h>::prev() const
{
	return p();
}

template<typename HE, typename V, typename F, typename El, bool h>
HE*& HalfEdgeReferences<HE, V, F, El, h>::prev()
{
	return p();
}

template<typename HE, typename V, typename F, typename El, bool h>
const HE* HalfEdgeReferences<HE, V, F, El, h>::twin() const
{
	return t();
}

template<typename HE, typename V, typename F, typename El, bool h>
HE*& HalfEdgeReferences<HE, V, F, El, h>::twin()
{
	return t();
}

template<typename HE, typename V, typename F, typename El, bool h>
const V* HalfEdgeReferences<HE, V, F, El, h>::fromVertex() const
{
	return v();
}

template<typename HE, typename V, typename F, typename El, bool h>
V*& HalfEdgeReferences<HE, V, F, El, h>::fromVertex()
{
	return v();
}

template<typename HE, typename V, typename F, typename El, bool h>
const V* HalfEdgeReferences<HE, V, F, El, h>::toVertex() const
{
	return n()->fromVertex();
}

template<typename HE, typename V, typename F, typename El, bool h>
V*& HalfEdgeReferences<HE, V, F, El, h>::toVertex()
{
	return n()->fromVertex();
}

template<typename HE, typename V, typename F, typename El, bool h>
const V* HalfEdgeReferences<HE, V, F, El, h>::vertex(uint i) const
{
	switch(i) {
	case 0: return v();
	case 1: return toVertex();
	default: assert(0); return nullptr;
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
V*& HalfEdgeReferences<HE, V, F, El, h>::vertex(uint i)
{
	switch(i) {
	case 0: return v();
	case 1: return toVertex();
	default: assert(0); return nullptr;
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
const F* HalfEdgeReferences<HE, V, F, El, h>::face() const
{
	return f();
}

template<typename HE, typename V, typename F, typename El, bool h>
F*& HalfEdgeReferences<HE, V, F, El, h>::face()
{
	return f();
}

template<typename HE, typename V, typename F, typename El, bool h>
void HalfEdgeReferences<HE, V, F, El, h>::updateHalfEdgeReferences(
	const HE* oldBase,
	const HE* newBase)
{
	if (n() != nullptr) {
		size_t diff = n() - oldBase;
		n() = (HE*)newBase + diff;
	}
	if (p() != nullptr) {
		size_t diff = p() - oldBase;
		p() = (HE*)newBase + diff;
	}
	if (t() != nullptr) {
		size_t diff = t() - oldBase;
		t() = (HE*)newBase + diff;
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
void HalfEdgeReferences<HE, V, F, El, h>::updateHalfEdgeReferencesAfterCompact(
	const HE*               base,
	const std::vector<int>& newIndices)
{
	if (n() != nullptr) {
		size_t diff = n() - base;
		if (newIndices[diff] < 0)
			n() = nullptr;
		else
			n() = (HE*)base + newIndices[diff];
	}
	if (p() != nullptr) {
		size_t diff = p() - base;
		if (newIndices[diff] < 0)
			p() = nullptr;
		else
			p() = (HE*)base + newIndices[diff];
	}
	if (t() != nullptr) {
		size_t diff = t() - base;
		if (newIndices[diff] < 0)
			t() = nullptr;
		else
			t() = (HE*)base + newIndices[diff];
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
void HalfEdgeReferences<HE, V, F, El, h>::updateFaceReferences(
	const F* oldBase,
	const F* newBase)
{
	if (f() != nullptr) {
		size_t diff = f() - oldBase;
		f() = (F*)newBase + diff;
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
void HalfEdgeReferences<HE, V, F, El, h>::updateFaceReferencesAfterCompact(
	const F*                base,
	const std::vector<int>& newIndices)
{
	if (f() != nullptr) {
		size_t diff = f() - base;
		if (newIndices[diff] < 0)
			f() = nullptr;
		else
			f() = base + newIndices[diff];
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
void HalfEdgeReferences<HE, V, F, El, h>::updateVertexReferences(
	const V* oldBase,
	const V* newBase)
{
	if (v() != nullptr) {
		size_t diff = v() - oldBase;
		v() = (V*)newBase + diff;
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
void HalfEdgeReferences<HE, V, F, El, h>::updateVertexReferencesAfterCompact(
	const V*                base,
	const std::vector<int>& newIndices)
{
	if (v() != nullptr) {
		size_t diff = v() - base;
		if (newIndices[diff] < 0)
			v() = nullptr;
		else
			v() = base + newIndices[diff];
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
template<typename Element>
void HalfEdgeReferences<HE, V, F, El, h>::importFrom(const Element&)
{
}

/**
 * @brief Import the half edge references from another half edge (e), which is of a different type.
 *
 * @param e: the half edge from which import the half edge references
 * @param base: the base of this container: necessary to compute the imported references
 * @param ebase: the base of the other container of half edges, from which we import the references
 */
template<typename HE, typename V, typename F, typename El, bool h>
template<typename OHE, typename HEType>
void HalfEdgeReferences<HE, V, F, El, h>::importHalfEdgeReferencesFrom(
	const OHE&    e,
	HE*           base,
	const HEType* ebase)
{
	if constexpr (HasHalfEdgeReferences<OHE>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.next() != nullptr) { // if the other half edge has a next
				// this next will be the base of this container plus the offset between the next of
				// the other half edge and the base of the other container
				n() = base + (e.next() - ebase);
			}
			if (e.prev() != nullptr) {
				p() = base + (e.prev() - ebase);
			}
			if (e.twin() != nullptr) {
				t() = base + (e.twin() - ebase);
			}
		}
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
template<typename OHE, typename VType>
void HalfEdgeReferences<HE, V, F, El, h>::importVertexReferencesFrom(
	const OHE&   e,
	V*           base,
	const VType* ebase)
{
	if constexpr (HasHalfEdgeReferences<OHE>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.fromVertex() != nullptr) {
				v() = base + (e.fromVertex() - ebase);
			}
		}
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
template<typename OHE, typename FType>
void HalfEdgeReferences<HE, V, F, El, h>::importFaceReferencesFrom(
	const OHE&   e,
	F*           base,
	const FType* ebase)
{
	if constexpr (HasHalfEdgeReferences<OHE>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.face() != nullptr) {
				f() = base + (e.face() - ebase);
			}
		}
	}
}

template<typename HE, typename V, typename F, typename El, bool h>
HE*& HalfEdgeReferences<HE, V, F, El, h>::n()
{
	return data.template get<El>(this).n;
}

template<typename HE, typename V, typename F, typename El, bool h>
const HE* HalfEdgeReferences<HE, V, F, El, h>::n() const
{
	return data.template get<El>(this).n;
}

template<typename HE, typename V, typename F, typename El, bool h>
HE*& HalfEdgeReferences<HE, V, F, El, h>::p()
{
	return data.template get<El>(this).p;
}

template<typename HE, typename V, typename F, typename El, bool h>
const HE* HalfEdgeReferences<HE, V, F, El, h>::p() const
{
	return data.template get<El>(this).p;
}

template<typename HE, typename V, typename F, typename El, bool h>
HE*& HalfEdgeReferences<HE, V, F, El, h>::t()
{
	return data.template get<El>(this).t;
}

template<typename HE, typename V, typename F, typename El, bool h>
const HE* HalfEdgeReferences<HE, V, F, El, h>::t() const
{
	return data.template get<El>(this).t;
}

template<typename HE, typename V, typename F, typename El, bool h>
V*& HalfEdgeReferences<HE, V, F, El, h>::v()
{
	return data.template get<El>(this).v;
}

template<typename HE, typename V, typename F, typename El, bool h>
const V* HalfEdgeReferences<HE, V, F, El, h>::v() const
{
	return data.template get<El>(this).v;
}

template<typename HE, typename V, typename F, typename El, bool h>
F*& HalfEdgeReferences<HE, V, F, El, h>::f()
{
	return data.template get<El>(this).f;
}

template<typename HE, typename V, typename F, typename El, bool h>
const F* HalfEdgeReferences<HE, V, F, El, h>::f() const
{
	return data.template get<El>(this).f;
}

} // namespace vcl::comp
