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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_ADJ_FACE_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_ADJ_FACE_ITERATOR_H

#include "vertex_base_iterator.h"

namespace vcl {

template<typename HalfEdge>
class VertexAdjFaceIterator : public VertexBaseIterator<HalfEdge>
{
	using Base = VertexBaseIterator<HalfEdge>;
public:
	using value_type        = typename HalfEdge::FaceType*;
	using reference         = typename HalfEdge::FaceType*&;
	using pointer           = typename HalfEdge::FaceType**;

	using Base::Base;

	reference operator*() const { return Base::current->face(); }
	pointer operator->() const { return &(Base::current->face()); }
};

template<typename HalfEdge>
class ConstVertexAdjFaceIterator : public ConstVertexBaseIterator<HalfEdge>
{
	using Base = ConstVertexBaseIterator<HalfEdge>;
public:
	using value_type        = const typename HalfEdge::FaceType*;
	using reference         = const typename HalfEdge::FaceType*;
	using pointer           = const typename HalfEdge::FaceType**;

	using Base::Base;

	reference operator*() const { return Base::current->face(); }
	pointer operator->() const { return &(Base::current->face()); }
};

} // namespace vcl

#endif // VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_ADJ_FACE_ITERATOR_H
