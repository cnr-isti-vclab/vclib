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

#ifndef VCL_ITERATORS_FACE_ADJ_FACE_ITERATOR_H
#define VCL_ITERATORS_FACE_ADJ_FACE_ITERATOR_H

#include "face_base_iterator.h"

namespace vcl {

template<typename HalfEdge>
class FaceAdjFaceIterator : public FaceBaseIterator<HalfEdge>
{
	using Base = FaceBaseIterator<HalfEdge>;
public:
	using value_type        = typename HalfEdge::FaceType*;
	using reference         = typename HalfEdge::FaceType*&;
	using pointer           = typename HalfEdge::FaceType**;

	using Base::Base;

	reference operator*() const
	{
		if (Base::current->twin() == nullptr) return nullptr;
		return Base::current->twin()->face();
	}
	pointer operator->() const
	{
		if (Base::current->twin() == nullptr) return nullptr;
		return &(Base::current->twin()->face());
	}
};

template<typename HalfEdge>
class ConstFaceAdjFaceIterator : public ConstFaceBaseIterator<HalfEdge>
{
	using Base = ConstFaceBaseIterator<HalfEdge>;
public:
	using value_type        = const typename HalfEdge::FaceType*;
	using reference         = const typename HalfEdge::FaceType*;
	using pointer           = const typename HalfEdge::FaceType**;

	using Base::Base;

	reference operator*() const
	{
		if (Base::current->twin() == nullptr) return nullptr;
		return Base::current->twin()->face();
	}
	pointer operator->() const
	{
		if (Base::current->twin() == nullptr) return nullptr;
		return &(Base::current->twin()->face());
	}
};

} // namespace vcl

#endif // VCL_ITERATORS_FACE_ADJ_FACE_ITERATOR_H
