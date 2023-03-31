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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_ADJ_VERTEX_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_ADJ_VERTEX_ITERATOR_H

#include "vertex_base_iterator.h"

namespace vcl {

namespace internal {

template<typename HalfEdge, bool CNST>
class VertexAdjVertexIterator : public VertexBaseIterator<HalfEdge, CNST>
{
	using Base = VertexBaseIterator<HalfEdge, CNST>;
public:
	using value_type = std::conditional_t<CNST,
			const typename HalfEdge::VertexType*,
			typename HalfEdge::VertexType*>;
	using reference  = std::conditional_t<CNST, value_type, value_type&>;
	using pointer    = value_type*;

	using Base::Base;

	reference operator*() const { return Base::current->toVertex(); }
	pointer operator->() const { return &(Base::current->toVertex()); }
};

} // namespace vcl::internal

template<typename HalfEdge>
using VertexAdjVertexIterator = internal::VertexAdjVertexIterator<HalfEdge, false>;

template<typename HalfEdge>
using ConstVertexAdjVertexIterator = internal::VertexAdjVertexIterator<HalfEdge, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_ADJ_VERTEX_ITERATOR_H
