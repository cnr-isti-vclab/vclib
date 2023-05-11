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

#include "half_edge_pointers.h"

#include <cstddef>

namespace vcl::comp {

template<typename HE, typename V, typename F, typename El, bool O>
HalfEdgePointers<HE, V, F, El, O>::HalfEdgePointers()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

template<typename HE, typename V, typename F, typename El, bool O>
void HalfEdgePointers<HE, V, F, El, O>::init()
{
	n() = nullptr;
	p() = nullptr;
	t() = nullptr;
	v() = nullptr;
	f() = nullptr;
}

template<typename HE, typename V, typename F, typename El, bool O>
bool HalfEdgePointers<HE, V, F, El, O>::isEnabled()
{
	return Base::isEnabled(this);
}

template<typename HE, typename V, typename F, typename El, bool O>
const HE* HalfEdgePointers<HE, V, F, El, O>::next() const
{
	return n();
}

template<typename HE, typename V, typename F, typename El, bool O>
HE*& HalfEdgePointers<HE, V, F, El, O>::next()
{
	return n();
}

template<typename HE, typename V, typename F, typename El, bool O>
const HE* HalfEdgePointers<HE, V, F, El, O>::prev() const
{
	return p();
}

template<typename HE, typename V, typename F, typename El, bool O>
HE*& HalfEdgePointers<HE, V, F, El, O>::prev()
{
	return p();
}

template<typename HE, typename V, typename F, typename El, bool O>
const HE* HalfEdgePointers<HE, V, F, El, O>::twin() const
{
	return t();
}

template<typename HE, typename V, typename F, typename El, bool O>
HE*& HalfEdgePointers<HE, V, F, El, O>::twin()
{
	return t();
}

template<typename HE, typename V, typename F, typename El, bool O>
const V* HalfEdgePointers<HE, V, F, El, O>::fromVertex() const
{
	return v();
}

template<typename HE, typename V, typename F, typename El, bool O>
V*& HalfEdgePointers<HE, V, F, El, O>::fromVertex()
{
	return v();
}

template<typename HE, typename V, typename F, typename El, bool O>
const V* HalfEdgePointers<HE, V, F, El, O>::toVertex() const
{
	return n()->fromVertex();
}

template<typename HE, typename V, typename F, typename El, bool O>
V*& HalfEdgePointers<HE, V, F, El, O>::toVertex()
{
	return n()->fromVertex();
}

template<typename HE, typename V, typename F, typename El, bool O>
const V* HalfEdgePointers<HE, V, F, El, O>::vertex(uint i) const
{
	switch(i) {
	case 0: return v();
	case 1: return toVertex();
	default: assert(0); return nullptr;
	}
}

template<typename HE, typename V, typename F, typename El, bool O>
V*& HalfEdgePointers<HE, V, F, El, O>::vertex(uint i)
{
	switch(i) {
	case 0: return v();
	case 1: return toVertex();
	default: assert(0); return nullptr;
	}
}

template<typename HE, typename V, typename F, typename El, bool O>
const F* HalfEdgePointers<HE, V, F, El, O>::face() const
{
	return f();
}

template<typename HE, typename V, typename F, typename El, bool O>
F*& HalfEdgePointers<HE, V, F, El, O>::face()
{
	return f();
}

template<typename HE, typename V, typename F, typename El, bool O>
template<typename Element>
void HalfEdgePointers<HE, V, F, El, O>::importFrom(const Element&)
{
}

/**
 * @brief Import the half edge pointers from another half edge (e), which is of a different type.
 *
 * @param e: the half edge from which import the half edge pointers
 * @param base: the base of this container: necessary to compute the imported pointers
 * @param ebase: the base of the other container of half edges, from which we import the pointers
 */
template<typename HE, typename V, typename F, typename El, bool O>
template<typename OHE, typename HEType>
void HalfEdgePointers<HE, V, F, El, O>::importPointersFrom(
	const OHE&    e,
	HE*           base,
	const HEType* ebase)
{
	if constexpr (HasHalfEdgePointers<OHE>) {
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

template<typename HE, typename V, typename F, typename El, bool O>
template<typename OHE, typename VType>
void HalfEdgePointers<HE, V, F, El, O>::importPointersFrom(
	const OHE&   e,
	V*           base,
	const VType* ebase)
{
	if constexpr (HasHalfEdgePointers<OHE>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.fromVertex() != nullptr) {
				v() = base + (e.fromVertex() - ebase);
			}
		}
	}
}

template<typename HE, typename V, typename F, typename El, bool O>
template<typename OHE, typename FType>
void HalfEdgePointers<HE, V, F, El, O>::importPointersFrom(
	const OHE&   e,
	F*           base,
	const FType* ebase)
{
	if constexpr (HasHalfEdgePointers<OHE>) {
		if (base != nullptr && ebase != nullptr) {
			if (e.face() != nullptr) {
				f() = base + (e.face() - ebase);
			}
		}
	}
}

template<typename HE, typename V, typename F, typename El, bool O>
void HalfEdgePointers<HE, V, F, El, O>::updatePointers(
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

template<typename HE, typename V, typename F, typename El, bool O>
void HalfEdgePointers<HE, V, F, El, O>::updatePointersAfterCompact(
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

template<typename HE, typename V, typename F, typename El, bool O>
void HalfEdgePointers<HE, V, F, El, O>::updatePointers(
	const F* oldBase,
	const F* newBase)
{
	if (f() != nullptr) {
		size_t diff = f() - oldBase;
		f() = (F*)newBase + diff;
	}
}

template<typename HE, typename V, typename F, typename El, bool O>
void HalfEdgePointers<HE, V, F, El, O>::updatePointersAfterCompact(
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

template<typename HE, typename V, typename F, typename El, bool O>
void HalfEdgePointers<HE, V, F, El, O>::updatePointers(
	const V* oldBase,
	const V* newBase)
{
	if (v() != nullptr) {
		size_t diff = v() - oldBase;
		v() = (V*)newBase + diff;
	}
}

template<typename HE, typename V, typename F, typename El, bool O>
void HalfEdgePointers<HE, V, F, El, O>::updatePointersAfterCompact(
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

template<typename HE, typename V, typename F, typename El, bool O>
HE*& HalfEdgePointers<HE, V, F, El, O>::n()
{
	return Base::data(this).n;
}

template<typename HE, typename V, typename F, typename El, bool O>
const HE* HalfEdgePointers<HE, V, F, El, O>::n() const
{
	return Base::data(this).n;
}

template<typename HE, typename V, typename F, typename El, bool O>
HE*& HalfEdgePointers<HE, V, F, El, O>::p()
{
	return Base::data(this).p;
}

template<typename HE, typename V, typename F, typename El, bool O>
const HE* HalfEdgePointers<HE, V, F, El, O>::p() const
{
	return Base::data(this).p;
}

template<typename HE, typename V, typename F, typename El, bool O>
HE*& HalfEdgePointers<HE, V, F, El, O>::t()
{
	return Base::data(this).t;
}

template<typename HE, typename V, typename F, typename El, bool O>
const HE* HalfEdgePointers<HE, V, F, El, O>::t() const
{
	return Base::data(this).t;
}

template<typename HE, typename V, typename F, typename El, bool O>
V*& HalfEdgePointers<HE, V, F, El, O>::v()
{
	return Base::data(this).v;
}

template<typename HE, typename V, typename F, typename El, bool O>
const V* HalfEdgePointers<HE, V, F, El, O>::v() const
{
	return Base::data(this).v;
}

template<typename HE, typename V, typename F, typename El, bool O>
F*& HalfEdgePointers<HE, V, F, El, O>::f()
{
	return Base::data(this).f;
}

template<typename HE, typename V, typename F, typename El, bool O>
const F* HalfEdgePointers<HE, V, F, El, O>::f() const
{
	return Base::data(this).f;
}

} // namespace vcl::comp
