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

#ifndef VCL_ITERATORS_FACE_WEDGE_TEX_COORD_ITERATOR_H
#define VCL_ITERATORS_FACE_WEDGE_TEX_COORD_ITERATOR_H

#include "face_base_iterator.h"

#include <vclib/space/tex_coord.h>

namespace vcl {

template<typename HalfEdge>
class FaceWedgeTexCoordIterator : public FaceBaseIterator<HalfEdge>
{
	using Base = FaceBaseIterator<HalfEdge>;
public:
	using value_type        = typename HalfEdge::TexCoordType;
	using reference         = typename HalfEdge::TexCoordType&;
	using pointer           = typename HalfEdge::TexCoordType*;

	using Base::Base;

	reference operator*() const
	{
		return Base::current->texCoord();
	}
	pointer operator->() const
	{
		return &(Base::current->texCoord());
	}
};

template<typename HalfEdge>
class ConstFaceWedgeTexCoordIterator : public ConstFaceBaseIterator<HalfEdge>
{
	using Base = ConstFaceBaseIterator<HalfEdge>;
public:
	using value_type        = const typename HalfEdge::TexCoordType;
	using reference         = const typename HalfEdge::TexCoordType&;
	using pointer           = const typename HalfEdge::TexCoordType*;

	using Base::Base;

	reference operator*() const
	{
		return Base::current->texCoord();
	}
	pointer operator->() const
	{
		return &(Base::current->texCoord());
	}
};

} // namespace vcl

#endif // VCL_ITERATORS_FACE_WEDGE_TEX_COORD_ITERATOR_H
