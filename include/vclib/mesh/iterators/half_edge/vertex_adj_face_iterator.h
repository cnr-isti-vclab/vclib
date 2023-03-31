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

namespace internal {

template<typename HalfEdge, bool CNST>
class VertexAdjFaceIterator :
		public VertexBaseIterator<HalfEdge, CNST, VertexAdjFaceIterator<HalfEdge, CNST>>
{
	using Base = VertexBaseIterator<HalfEdge, CNST, VertexAdjFaceIterator<HalfEdge, CNST>>;
public:
	using value_type = std::conditional_t<CNST,
			const typename HalfEdge::FaceType*,
			typename HalfEdge::FaceType*>;
	using reference = std::conditional_t<CNST, value_type, value_type&>;
	using pointer   = value_type*;

	using Base::Base;

	reference operator*() const { return Base::current->face(); }
	pointer operator->() const { return &(Base::current->face()); }
};

} // namespace vcl::internal

template<typename HalfEdge>
using VertexAdjFaceIterator = internal::VertexAdjFaceIterator<HalfEdge, false>;

template<typename HalfEdge>
using ConstVertexAdjFaceIterator = internal::VertexAdjFaceIterator<HalfEdge, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_ADJ_FACE_ITERATOR_H
