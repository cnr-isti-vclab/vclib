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

#ifndef VCL_MESH_ITERATORS_VERTEX_COORD_ITERATOR_H
#define VCL_MESH_ITERATORS_VERTEX_COORD_ITERATOR_H

#include <vclib/misc/types.h>

namespace vcl {

template<typename It>
class VertexCoordIterator : public It
{
	using VertexType = typename It::value_type;

	using CoordType = typename std::conditional_t<
		std::is_const_v<VertexType>,
		const typename VertexType::CoordType,
		typename VertexType::CoordType>;

public:
	using value_type = typename std::remove_const<CoordType>::type;
	using reference  = CoordType&;
	using pointer    = CoordType*;

	VertexCoordIterator(const It& it) : It(it) {}

	reference operator*() const { return It::operator*().coord(); }
	pointer operator->() const { return &It::operator*().coord(); }

};

template<typename It>
class VertexPointerCoordIterator : public It
{
	using VertexType = typename std::remove_pointer<typename It::value_type>::type;

	using CoordType = typename std::conditional_t<
		std::is_const_v<VertexType>,
		const typename VertexType::CoordType,
		typename VertexType::CoordType>;

public:
	using value_type = typename std::remove_const<CoordType>::type;
	using reference  = CoordType&;
	using pointer    = CoordType*;

	VertexPointerCoordIterator(const It& it) : It(it) {}

	reference operator*() const { return It::operator*()->coord(); }
	pointer operator->() const { return &It::operator*()->coord(); }

};

} // namespace vcl

#endif // VCL_MESH_ITERATORS_VERTEX_COORD_ITERATOR_H
