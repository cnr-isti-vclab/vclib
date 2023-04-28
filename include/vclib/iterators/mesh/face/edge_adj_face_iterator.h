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

#ifndef VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H
#define VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H

#include <iterator>

#include <vclib/types.h>

namespace vcl {

template<typename FaceType, bool CNST = false>
class EdgeAdjFaceIterator
{
	using FT = typename std::conditional_t<CNST, FaceType, const FaceType>;

	using VT = typename std::conditional_t<CNST,
		typename FT::VertexType,
		const typename FT::VertexType>;
public:
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;
	using value_type        = FT*;
	using reference         = FT*&;
	using pointer           = FT**;

	EdgeAdjFaceIterator();
	EdgeAdjFaceIterator(FT& f, uint edge);

	bool operator==(const EdgeAdjFaceIterator& oi) const;
	bool operator!=(const EdgeAdjFaceIterator& oi) const;

	EdgeAdjFaceIterator& operator++();
	EdgeAdjFaceIterator operator++(int);

	reference operator*() const;
	pointer operator->() const;

private:
	FT* current = nullptr;
	FT* end     = nullptr;
	VT* v0      = nullptr;
	VT* v1      = nullptr;
};

template<typename FaceType>
using ConstEdgeAdjFaceIterator = EdgeAdjFaceIterator<FaceType, true>;

} // namespace vcl

#include "edge_adj_face_iterator.cpp"

#endif // VCL_ITERATORS_MESH_EDGE_ADJ_FACE_ITERATOR_H
