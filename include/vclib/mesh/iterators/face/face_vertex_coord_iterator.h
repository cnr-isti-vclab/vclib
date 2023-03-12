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

#ifndef VCL_MESH_ITERATORS_FACE_VERTEX_COORD_ITERATOR_H
#define VCL_MESH_ITERATORS_FACE_VERTEX_COORD_ITERATOR_H

#include <vclib/misc/types.h>

namespace vcl {

template<typename It>
class FaceVertexCoordIterator
{
	using VertexType = typename std::remove_pointer<typename It::value_type>::type;
	using CoordType = typename VertexType::CoordType;

public:
	using value_type = CoordType;
	using reference  = value_type&;
	using pointer    = value_type*;
	using difference_type   = typename It::ptrdiff_t;
	using iterator_category = typename It::iterator_category;

	FaceVertexCoordIterator(const It& it) : It(it) {}

	reference operator*() const { return It::operator->()->coord(); }
	pointer operator->() const { return &It::operator->()->coord(); }

};

} // namespace vcl

#endif // VCL_MESH_ITERATORS_FACE_VERTEX_COORD_ITERATOR_H
