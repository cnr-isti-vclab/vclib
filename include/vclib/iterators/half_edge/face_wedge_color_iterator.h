/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_ITERATORS_FACE_WEDGE_COLOR_ITERATOR_H
#define VCL_ITERATORS_FACE_WEDGE_COLOR_ITERATOR_H

#include "face_base_iterator.h"

#include <vclib/space/color.h>

namespace vcl {

template<typename HalfEdge>
class FaceWedgeColorIterator : public FaceBaseIterator<HalfEdge>
{
	using Base = FaceBaseIterator<HalfEdge>;
public:
	using value_type        = vcl::Color;
	using reference         = vcl::Color&;
	using pointer           = vcl::Color*;

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

template<typename HalfEdge>
class ConstFaceWedgeColorIterator : public ConstFaceBaseIterator<HalfEdge>
{
	using Base = ConstFaceBaseIterator<HalfEdge>;
public:
	using value_type        = const vcl::Color;
	using reference         = const vcl::Color&;
	using pointer           = const vcl::Color*;

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

} // namespace vcl

#endif // VCL_ITERATORS_FACE_WEDGE_COLOR_ITERATOR_H
