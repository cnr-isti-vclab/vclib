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

#ifndef VCL_MESH_ITERATORS_COMPONENT_SELECTION_ITERATOR_H
#define VCL_MESH_ITERATORS_COMPONENT_SELECTION_ITERATOR_H

#include <vclib/iterators/iterator_wrapper.h>
#include <vclib/misc/bit_proxy.h>

namespace vcl {

template<typename T>
class SelectionIterator
{
	SelectionIterator(T) {}
	static_assert(sizeof(T) != sizeof(T), "");
};

template<IteratorConcept It>
class SelectionIterator<It> : public It
{
public:
	using value_type = bool;
	using reference  = bool;
	using pointer    = bool*;

	using It::It;
	SelectionIterator(const It& it) : It(it) {}

	reference operator*() const
	{
		if constexpr (IteratesOverClass<It>) {
			return It::operator*().selected();
		}
		else {
			return It::operator*()->selected();
		}
	}
	pointer operator->() const
	{
		return &(operator*());
	}
};

} // namespace vcl

#endif // VCL_MESH_ITERATORS_COMPONENT_SELECTION_ITERATOR_H
