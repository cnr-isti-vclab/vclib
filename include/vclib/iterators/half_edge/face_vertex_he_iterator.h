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

#ifndef VCL_ITERATORS_FACE_VERTEX_HE_ITERATOR_H
#define VCL_ITERATORS_FACE_VERTEX_HE_ITERATOR_H

#include "face_he_iterator.h"

namespace vcl {

template<typename HalfEdge>
class FaceVertexHEIterator : public FaceHEIterator<HalfEdge>
{
	using Base = FaceHEIterator<HalfEdge>;
public:
	using value_type        = typename HalfEdge::VertexType;
	using reference         = typename HalfEdge::VertexType*&;
	using pointer           = typename HalfEdge::VertexType**;

	using Base::Base;

	reference operator*() const { return Base::current->fromVertex(); }
	pointer operator->() const { return &(Base::current->fromVertex()); }
};

} // namespace vcl

#endif // VCL_ITERATORS_FACEVERTEXHEITERATOR_H
