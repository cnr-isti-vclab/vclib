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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_FACE_VERTEX_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_FACE_VERTEX_ITERATOR_H

#include <vclib/types.h>

namespace vcl {

namespace internal {

template<typename HalfEdge, bool CNST>
class FaceVertexIterator
{
	using CurrentHEdgeType = std::conditional_t<CNST, const HalfEdge*, HalfEdge*>;

public :
	using value_type = std::conditional_t<CNST,
			const typename HalfEdge::VertexType*,
			typename HalfEdge::VertexType*>;

	using reference  = std::conditional_t<CNST, value_type, value_type&>;
	using pointer    = value_type*;
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	FaceVertexIterator() = default;
	FaceVertexIterator(CurrentHEdgeType start) : current(start), end(start) {}
	FaceVertexIterator(CurrentHEdgeType start, const HalfEdge* end) : current(start), end(end) {}

	bool operator==(const FaceVertexIterator& oi) const { return current == oi.current; }
	bool operator!=(const FaceVertexIterator& oi) const { return current != oi.current; }

	reference operator*() const { return current->fromVertex(); }
	pointer operator->() const { return &(current->fromVertex()); }

	auto& operator++()
	{
		current = current->next();
		if (current == end) current = nullptr;
		return *this;
	}
	auto operator++(int)
	{
		auto it = *this;
		current = current->next();
		if (current == end) current = nullptr;
		return it;

	}
	auto& operator--()
	{
		current = current->prev();
		if (current == end) current = nullptr;
		return *this;
	}
	auto operator--(int)
	{
		auto it = *this;
		current = current->prev();
		if (current == end) current = nullptr;
		return it;
	}

protected:
	CurrentHEdgeType current = nullptr;
	const HalfEdge* end = nullptr; // when the current is equal to end, it will be set to nullptr
};

} // namespace vcl::internal

template<typename HalfEdge>
using FaceVertexIterator = internal::FaceVertexIterator<HalfEdge, false>;

template<typename HalfEdge>
using ConstFaceVertexIterator = internal::FaceVertexIterator<HalfEdge, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_HALF_EDGE_FACE_VERTEX_ITERATOR_H
