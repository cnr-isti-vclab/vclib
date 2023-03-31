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

#ifndef VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_BASE_ITERATOR_H
#define VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_BASE_ITERATOR_H

#include "base_iterator.h"

namespace vcl::internal {

template<typename HalfEdge, bool CNST, typename DerivedIterator>
class VertexBaseIterator : public BaseIterator<HalfEdge, CNST>
{
	using Base = BaseIterator<HalfEdge, CNST>;
public:
	using Base::BaseIterator;

	auto& operator++();
	auto operator++(int);
	auto& operator--();
	auto operator--(int);
};

} // namespace vcl::internal

#include "vertex_base_iterator.cpp"

#endif // VCL_MESH_ITERATORS_HALF_EDGE_VERTEX_BASE_ITERATOR_H
