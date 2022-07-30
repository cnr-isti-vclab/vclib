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

#include "face_base_iterator.h"

namespace vcl {

template<typename HalfEdge>
FaceBaseIterator<HalfEdge>::FaceBaseIterator()
{
}

template<typename HalfEdge>
FaceBaseIterator<HalfEdge>::FaceBaseIterator(HalfEdge* start) : current(start), end(start)
{
}

template<typename HalfEdge>
FaceBaseIterator<HalfEdge>::FaceBaseIterator(HalfEdge* start, const HalfEdge* end) :
		current(start), end(end)
{
}

template<typename HalfEdge>
bool FaceBaseIterator<HalfEdge>::operator==(const FaceBaseIterator& oi) const
{
	return current == oi.current;
}

template<typename HalfEdge>
bool FaceBaseIterator<HalfEdge>::operator!=(const FaceBaseIterator& oi) const
{
	return current != oi.current;
}

template<typename HalfEdge>
FaceBaseIterator<HalfEdge> FaceBaseIterator<HalfEdge>::operator++()
{
	current = current->next();
	if (current == end) current = nullptr;
	return *this;
}

template<typename HalfEdge>
FaceBaseIterator<HalfEdge> FaceBaseIterator<HalfEdge>::operator++(int)
{
	auto it = *this;
	current = current->next();
	if (current == end) current = nullptr;
	return it;
}

template<typename HalfEdge>
FaceBaseIterator<HalfEdge> FaceBaseIterator<HalfEdge>::operator--()
{
	current = current->prev();
	if (current == end) current = nullptr;
	return *this;
}

template<typename HalfEdge>
FaceBaseIterator<HalfEdge> FaceBaseIterator<HalfEdge>::operator--(int)
{
	auto it = *this;
	current = current->prev();
	if (current == end) current = nullptr;
	return it;
}

template<typename HalfEdge>
ConstFaceBaseIterator<HalfEdge>::ConstFaceBaseIterator()
{
}

template<typename HalfEdge>
ConstFaceBaseIterator<HalfEdge>::ConstFaceBaseIterator(const HalfEdge* start) :
		current(start), end(start)
{
}

template<typename HalfEdge>
ConstFaceBaseIterator<HalfEdge>::ConstFaceBaseIterator(const HalfEdge* start, const HalfEdge* end) :
		current(start), end(end)
{
}

template<typename HalfEdge>
bool ConstFaceBaseIterator<HalfEdge>::operator==(const ConstFaceBaseIterator &oi) const
{
	return current == oi.current;
}

template<typename HalfEdge>
bool ConstFaceBaseIterator<HalfEdge>::operator!=(const ConstFaceBaseIterator &oi) const
{
	return current != oi.current;
}

template<typename HalfEdge>
ConstFaceBaseIterator<HalfEdge> ConstFaceBaseIterator<HalfEdge>::operator++()
{
	current = current->next();
	if (current == end) current = nullptr;
	return *this;
}

template<typename HalfEdge>
ConstFaceBaseIterator<HalfEdge> ConstFaceBaseIterator<HalfEdge>::operator++(int)
{
	auto it = *this;
	current = current->next();
	if (current == end) current = nullptr;
	return it;
}

template<typename HalfEdge>
ConstFaceBaseIterator<HalfEdge> ConstFaceBaseIterator<HalfEdge>::operator--()
{
	current = current->prev();
	if (current == end) current = nullptr;
	return *this;
}

template<typename HalfEdge>
ConstFaceBaseIterator<HalfEdge> ConstFaceBaseIterator<HalfEdge>::operator--(int)
{
	auto it = *this;
	current = current->prev();
	if (current == end) current = nullptr;
	return it;
}

} // namespace vcl
