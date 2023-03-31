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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_BASE_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_BASE_ITERATOR_H

#include <iterator>

namespace vcl {

template<typename HalfEdge>
class VertexBaseIterator
{
public:
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	VertexBaseIterator();
	VertexBaseIterator(HalfEdge* start);
	VertexBaseIterator(HalfEdge* start, const HalfEdge* end);

	bool operator==(const VertexBaseIterator& oi) const;
	bool operator!=(const VertexBaseIterator& oi) const;

	VertexBaseIterator operator++();
	VertexBaseIterator operator++(int);
	VertexBaseIterator operator--();
	VertexBaseIterator operator--(int);

protected:
	HalfEdge* current = nullptr;
	const HalfEdge* end = nullptr; // when the current is equal to end, it will be set to nullptr
};

template<typename HalfEdge>
class ConstVertexBaseIterator
{
public:
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	ConstVertexBaseIterator();
	ConstVertexBaseIterator(const HalfEdge* start);
	ConstVertexBaseIterator(const HalfEdge* start, const HalfEdge* end);

	bool operator==(const ConstVertexBaseIterator& oi) const;
	bool operator!=(const ConstVertexBaseIterator& oi) const;

	ConstVertexBaseIterator operator++();
	ConstVertexBaseIterator operator++(int);
	ConstVertexBaseIterator operator--();
	ConstVertexBaseIterator operator--(int);

protected:
	const HalfEdge* current = nullptr;
	const HalfEdge* end = nullptr; // when the current is equal to end, it will be set to nullptr
};

} // namespace vcl

#include "vertex_base_iterator.cpp"

#endif // VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_BASE_ITERATOR_H
