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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_FACE_ADJ_FACE_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_FACE_ADJ_FACE_ITERATOR_H

#include "face_base_iterator.h"

namespace vcl {

namespace internal {

template<typename HalfEdge, bool CNST>
class FaceAdjFaceIterator
{
	friend class FaceBaseIterator;

	using CurrentHEdgeType = std::conditional_t<CNST, const HalfEdge*, HalfEdge*>;
public:
	using value_type = std::conditional_t<CNST,
			const typename HalfEdge::FaceType*,
			typename HalfEdge::FaceType*>;
	using reference = std::conditional_t<CNST, value_type, value_type&>;
	using pointer   = value_type*;
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	FaceAdjFaceIterator() = default;
	FaceAdjFaceIterator(CurrentHEdgeType start) : current(start), end(start) {}
	FaceAdjFaceIterator(CurrentHEdgeType start, const HalfEdge* end) : current(start), end(end) {}

	bool operator==(const FaceAdjFaceIterator& oi) const { return current == oi.current; }
	bool operator!=(const FaceAdjFaceIterator& oi) const { return current != oi.current; }

	reference operator*() const
	{
		if (current->twin() == nullptr) return nullptr;
		return current->twin()->face();
	}
	pointer operator->() const
	{
		if (current->twin() == nullptr) return nullptr;
		return &(current->twin()->face());
	}

	auto& operator++()   { return FaceBaseIterator::increment(*this); }
	auto operator++(int) { return FaceBaseIterator::postIncrement(*this); }
	auto& operator--()   { return FaceBaseIterator::decrement(*this); }
	auto operator--(int) { return FaceBaseIterator::postDecrement(*this); }

protected:
	CurrentHEdgeType current = nullptr;
	const HalfEdge* end = nullptr; // when the current is equal to end, it will be set to nullptr
};

} // namespace vcl::internal

template<typename HalfEdge>
using FaceAdjFaceIterator = internal::FaceAdjFaceIterator<HalfEdge, false>;

template<typename HalfEdge>
using ConstFaceAdjFaceIterator = internal::FaceAdjFaceIterator<HalfEdge, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_HALF_EDGE_FACE_ADJ_FACE_ITERATOR_H
