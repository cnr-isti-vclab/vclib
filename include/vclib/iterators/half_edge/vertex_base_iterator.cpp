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

#include "vertex_base_iterator.h"

namespace vcl {

template<typename HalfEdge>
VertexBaseIterator<HalfEdge>::VertexBaseIterator()
{
}

template<typename HalfEdge>
VertexBaseIterator<HalfEdge>::VertexBaseIterator(HalfEdge* start) : current(start), end(start)
{
}

template<typename HalfEdge>
VertexBaseIterator<HalfEdge>::VertexBaseIterator(HalfEdge* start, const HalfEdge* end) :
		current(start), end(end)
{
}

template<typename HalfEdge>
bool VertexBaseIterator<HalfEdge>::operator==(const VertexBaseIterator &oi) const
{
	return current == oi.current;
}

template<typename HalfEdge>
bool VertexBaseIterator<HalfEdge>::operator!=(const VertexBaseIterator &oi) const
{
	return current != oi.current;
}

template<typename HalfEdge>
VertexBaseIterator<HalfEdge> VertexBaseIterator<HalfEdge>::operator++()
{
	current = current->prev();
	current = current->twin();
	if (current == end) current = nullptr;
	return *this;
}

template<typename HalfEdge>
VertexBaseIterator<HalfEdge> VertexBaseIterator<HalfEdge>::operator++(int)
{
	auto it = *this;
	current = current->prev();
	current = current->twin();
	if (current == end) current = nullptr;
	return it;
}

template<typename HalfEdge>
VertexBaseIterator<HalfEdge> VertexBaseIterator<HalfEdge>::operator--()
{
	current = current->twin();
	if (current != nullptr)
		current = current->next();
	if (current == end) current = nullptr;
	return *this;
}

template<typename HalfEdge>
VertexBaseIterator<HalfEdge> VertexBaseIterator<HalfEdge>::operator--(int)
{
	auto it = *this;
	current = current->twin();
	if (current != nullptr)
		current = current->next();
	if (current == end) current = nullptr;
	return it;
}

template<typename HalfEdge>
ConstVertexBaseIterator<HalfEdge>::ConstVertexBaseIterator()
{

}

template<typename HalfEdge>
ConstVertexBaseIterator<HalfEdge>::ConstVertexBaseIterator(const HalfEdge *start) :
		current(start), end(start)
{
}

template<typename HalfEdge>
ConstVertexBaseIterator<HalfEdge>::ConstVertexBaseIterator(
	const HalfEdge* start,
	const HalfEdge* end) :
		current(start), end(end)
{
}

template<typename HalfEdge>
bool ConstVertexBaseIterator<HalfEdge>::operator==(const ConstVertexBaseIterator &oi) const
{
	return current == oi.current;
}

template<typename HalfEdge>
bool ConstVertexBaseIterator<HalfEdge>::operator!=(const ConstVertexBaseIterator &oi) const
{
	return current != oi.current;
}

template<typename HalfEdge>
ConstVertexBaseIterator<HalfEdge> ConstVertexBaseIterator<HalfEdge>::operator++()
{
	current = current->prev();
	current = current->twin();
	if (current == end) current = nullptr;
	return *this;
}

template<typename HalfEdge>
ConstVertexBaseIterator<HalfEdge> ConstVertexBaseIterator<HalfEdge>::operator++(int)
{
	auto it = *this;
	current = current->prev();
	current = current->twin();
	if (current == end) current = nullptr;
	return it;
}

template<typename HalfEdge>
ConstVertexBaseIterator<HalfEdge> ConstVertexBaseIterator<HalfEdge>::operator--()
{
	current = current->twin();
	if (current != nullptr)
		current = current->next();
	if (current == end) current = nullptr;
	return *this;
}

template<typename HalfEdge>
ConstVertexBaseIterator<HalfEdge> ConstVertexBaseIterator<HalfEdge>::operator--(int)
{
	auto it = *this;
	current = current->twin();
	if (current != nullptr)
		current = current->next();
	if (current == end) current = nullptr;
	return it;
}

} // namespace vcl
