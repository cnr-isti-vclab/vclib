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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_FACE_WEDGE_COLOR_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_FACE_WEDGE_COLOR_ITERATOR_H

#include "face_base_iterator.h"

#include <vclib/space/color.h>

namespace vcl {

namespace internal {

template<typename HalfEdge, bool CNST>
class FaceWedgeColorIterator :
		public FaceBaseIterator<HalfEdge, CNST, FaceWedgeColorIterator<HalfEdge, CNST>>
{
	using Base = FaceBaseIterator<HalfEdge, CNST, FaceWedgeColorIterator<HalfEdge, CNST>>;
public:
	using value_type = std::conditional_t<CNST, const vcl::Color, vcl::Color>;
	using reference  = value_type&;
	using pointer    = value_type*;

	using Base::Base;

	reference operator*() const
	{
		return Base::current->color();
	}
	pointer operator->() const
	{
		return &(Base::current->color());
	}
};

} // namespace vcl::internal

template<typename HalfEdge>
using FaceWedgeColorIterator = internal::FaceWedgeColorIterator<HalfEdge, false>;

template<typename HalfEdge>
using ConstFaceWedgeColorIterator = internal::FaceWedgeColorIterator<HalfEdge, true>;

} // namespace vcl

#endif // VCL_MESH_ITERATORS_HALF_EDGE_FACE_WEDGE_COLOR_ITERATOR_H
