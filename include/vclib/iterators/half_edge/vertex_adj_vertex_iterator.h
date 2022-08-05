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

#ifndef VCL_ITERATORS_VERTEX_ADJ_VERTEX_ITERATOR_H
#define VCL_ITERATORS_VERTEX_ADJ_VERTEX_ITERATOR_H

#include "vertex_base_iterator.h"

namespace vcl {

template<typename HalfEdge>
class VertexAdjVertexIterator : public VertexBaseIterator<HalfEdge>
{
	using Base = VertexBaseIterator<HalfEdge>;
public:
	using value_type        = typename HalfEdge::VertexType*;
	using reference         = typename HalfEdge::VertexType*&;
	using pointer           = typename HalfEdge::VertexType**;

	using Base::Base;

	reference operator*() const { return Base::current->toVertex(); }
	pointer operator->() const { return &(Base::current->toVertex()); }
};

template<typename HalfEdge>
class ConstVertexAdjVertexIterator : public ConstVertexBaseIterator<HalfEdge>
{
	using Base = ConstVertexBaseIterator<HalfEdge>;
public:
	using value_type        = const typename HalfEdge::VertexType*;
	using reference         = const typename HalfEdge::VertexType*&;
	using pointer           = const typename HalfEdge::VertexType**;

	using Base::Base;

	reference operator*() const { return Base::current->toVertex(); }
	pointer operator->() const { return &(Base::current->toVertex()); }
};

} // namespace vcl

#endif // VCL_ITERATORS_VERTEX_ADJ_VERTEX_ITERATOR_H
