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

#ifndef VCL_MESH_ITERATORS_EDGE_ADJ_FACE_ITERATOR_H
#define VCL_MESH_ITERATORS_EDGE_ADJ_FACE_ITERATOR_H

#include <iterator>

#include <vclib/types.h>

namespace vcl {

namespace internal {

template<typename FT, bool CNST = false>
class EdgeAdjFaceIterator
{
	using FaceType = typename std::conditional_t<CNST, FT, const FT>;

	using VertexType = typename std::conditional_t<CNST,
		typename FaceType::VertexType,
		const typename FaceType::VertexType>;
public:
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;
	using value_type        = FaceType*;
	using reference         = FaceType*&;
	using pointer           = FaceType**;

	EdgeAdjFaceIterator();
	EdgeAdjFaceIterator(FaceType& f, uint edge);

	bool operator==(const EdgeAdjFaceIterator& oi) const;
	bool operator!=(const EdgeAdjFaceIterator& oi) const;

	EdgeAdjFaceIterator& operator++();
	EdgeAdjFaceIterator operator++(int);

	reference operator*() const;
	pointer operator->() const;

private:
	FaceType* current = nullptr;
	FaceType* end = nullptr;
	VertexType* v0 = nullptr;
	VertexType* v1 = nullptr;
};

} // namespace vcl::internal

template<typename FaceType>
using EdgeAdjFaceIterator = internal::EdgeAdjFaceIterator<FaceType, false>;

template<typename FaceType>
using ConstEdgeAdjFaceIterator = internal::EdgeAdjFaceIterator<FaceType, true>;

} // namespace vcl

#include "edge_adj_face_iterator.cpp"

#endif // VCL_MESH_ITERATORS_EDGE_ADJ_FACE_ITERATOR_H
