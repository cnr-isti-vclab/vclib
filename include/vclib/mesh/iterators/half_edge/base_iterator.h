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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_BASE_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_BASE_ITERATOR_H

#include <iterator>

namespace vcl::internal {

template<typename HalfEdge, bool CNST>
class BaseIterator
{
	using CurrentHEdgeType = std::conditional_t<CNST, const HalfEdge*, HalfEdge*>;
public:
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	BaseIterator() {}
	BaseIterator(CurrentHEdgeType start) : current(start), end(start) {}
	BaseIterator(CurrentHEdgeType start, const HalfEdge* end) : current(start), end(end) {}

	bool operator==(const BaseIterator& oi) const { return current == oi.current; }
	bool operator!=(const BaseIterator& oi) const { return current != oi.current; }

protected:
	CurrentHEdgeType current = nullptr;
	const HalfEdge* end = nullptr; // when the current is equal to end, it will be set to nullptr
};

} // namespace vcl::internal

#endif // VCL_MESH_ITERATORS_HALF_EDGE_BASE_ITERATOR_H
