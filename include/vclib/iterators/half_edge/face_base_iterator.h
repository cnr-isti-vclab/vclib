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

#ifndef VCL_ITERATORS_FACE_BASE_ITERATOR_H
#define VCL_ITERATORS_FACE_BASE_ITERATOR_H

#include <iterator>

namespace vcl {

template<typename HalfEdge>
class FaceBaseIterator
{
public:
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	FaceBaseIterator();
	FaceBaseIterator(HalfEdge* start);
	FaceBaseIterator(HalfEdge* start, const HalfEdge* end);

	bool operator==(const FaceBaseIterator& oi) const;
	bool operator!=(const FaceBaseIterator& oi) const;

	FaceBaseIterator operator++();
	FaceBaseIterator operator++(int);
	FaceBaseIterator operator--();
	FaceBaseIterator operator--(int);

protected:
	HalfEdge* current = nullptr;
	const HalfEdge* end = nullptr; // when the current is equal to this he, it will be set to nullptr
};

template<typename HalfEdge>
class ConstFaceHEIterator
{
public:
	using difference_type   = ptrdiff_t;
	using iterator_category = std::forward_iterator_tag;

	ConstFaceHEIterator();
	ConstFaceHEIterator(const HalfEdge* start);
	ConstFaceHEIterator(const HalfEdge* start, const HalfEdge* end);
	ConstFaceHEIterator(const FaceBaseIterator<HalfEdge>& it);

	bool operator==(const ConstFaceHEIterator& oi) const;
	bool operator!=(const ConstFaceHEIterator& oi) const;

	ConstFaceHEIterator operator++();
	ConstFaceHEIterator operator++(int);
	ConstFaceHEIterator operator--();
	ConstFaceHEIterator operator--(int);

protected:
	const HalfEdge* current = nullptr;
	const HalfEdge* end = nullptr; // when the current is equal to this he, it will be set to nullptr
};

} // namespace vcl

#include "face_base_iterator.cpp"

#endif // VCL_ITERATORS_FACE_BASE_ITERATOR_H
