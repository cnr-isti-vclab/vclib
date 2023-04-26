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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_FACE_WEDGE_TEX_COORD_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_FACE_WEDGE_TEX_COORD_ITERATOR_H

#include "face_base_iterator.h"

#include <vclib/space/tex_coord.h>

namespace vcl {

template<typename HalfEdge, bool CNST = false>
class FaceWedgeTexCoordIterator
{
	friend class internal::FaceBaseIterator;

	using CurrentHEdgeType = std::conditional_t<CNST, const HalfEdge*, HalfEdge*>;
public:
	using value_type = std::conditional_t<CNST,
			const typename HalfEdge::TexCoordType,
			typename HalfEdge::TexCoordType>;
	using reference  = value_type&;
	using pointer    = value_type*;
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	FaceWedgeTexCoordIterator() = default;
	FaceWedgeTexCoordIterator(CurrentHEdgeType start) : current(start), end(start) {}
	FaceWedgeTexCoordIterator(CurrentHEdgeType start, const HalfEdge* end) : current(start), end(end) {}

	bool operator==(const FaceWedgeTexCoordIterator& oi) const { return current == oi.current; }
	bool operator!=(const FaceWedgeTexCoordIterator& oi) const { return current != oi.current; }

	reference operator*() const { return current->texCoord(); }
	pointer operator->() const { return &(current->texCoord()); }

	auto& operator++()   { return internal::FaceBaseIterator::increment(*this); }
	auto operator++(int) { return internal::FaceBaseIterator::postIncrement(*this); }
	auto& operator--()   { return internal::FaceBaseIterator::decrement(*this); }
	auto operator--(int) { return internal::FaceBaseIterator::postDecrement(*this); }

protected:
	CurrentHEdgeType current = nullptr;
	const HalfEdge* end = nullptr; // when the current is equal to end, it will be set to nullptr
};

template<typename HalfEdge>
using ConstFaceWedgeTexCoordIterator = FaceWedgeTexCoordIterator<HalfEdge, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_HALF_EDGE_FACE_WEDGE_TEX_COORD_ITERATOR_H
