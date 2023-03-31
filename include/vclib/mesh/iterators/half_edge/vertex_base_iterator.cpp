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

#include "vertex_base_iterator.h"

namespace vcl::internal {

template<typename HalfEdge, bool CNST, typename DerivedIterator>
auto& VertexBaseIterator<HalfEdge, CNST, DerivedIterator>::operator++()
{
	Base::current = Base::current->prev();
	Base::current = Base::current->twin();
	if (Base::current == Base::end) Base::current = nullptr;
	return *(static_cast<const DerivedIterator*>(this));
}

template<typename HalfEdge, bool CNST, typename DerivedIterator>
auto VertexBaseIterator<HalfEdge, CNST, DerivedIterator>::operator++(int)
{
	auto it = *(static_cast<const DerivedIterator*>(this));
	Base::current = Base::current->prev();
	Base::current = Base::current->twin();
	if (Base::current == Base::end) Base::current = nullptr;
	return it;
}

template<typename HalfEdge, bool CNST, typename DerivedIterator>
auto& VertexBaseIterator<HalfEdge, CNST, DerivedIterator>::operator--()
{
	Base::current = Base::current->twin();
	if (Base::current != nullptr)
		Base::current = Base::current->next();
	if (Base::current == Base::end) Base::current = nullptr;
	return *(static_cast<const DerivedIterator*>(this));
}

template<typename HalfEdge, bool CNST, typename DerivedIterator>
auto VertexBaseIterator<HalfEdge, CNST, DerivedIterator>::operator--(int)
{
	auto it = *(static_cast<const DerivedIterator*>(this));
	Base::current = Base::current->twin();
	if (Base::current != nullptr)
		Base::current = Base::current->next();
	if (Base::current == Base::end) Base::current = nullptr;
	return it;
}

} // namespace vcl::internal
