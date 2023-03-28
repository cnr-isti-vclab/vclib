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

#ifndef VCL_MESH_ITERATORS_ELEMENT_SELECTION_ITERATOR_H
#define VCL_MESH_ITERATORS_ELEMENT_SELECTION_ITERATOR_H

#include <vclib/iterator/iterator_wrapper.h>
#include <vclib/iterator/range.h>
#include <vclib/misc/bit_proxy.h>

namespace vcl {

template<typename It>
class ElementSelectionIterator : public It
{
	using ElementType = typename std::remove_pointer_t<typename It::pointer>;
	using BoolType = typename std::conditional_t <
		std::is_const_v<ElementType>,
		bool, BitProxy>;

public:
	using value_type = BoolType;
	using reference  = BoolType;
	using pointer    = BoolType*;

	using It::It;
	ElementSelectionIterator(const It& it) : It(it) {}

	reference operator*() const { return It::operator*().selected(); }
	pointer operator->() const { return &It::operator*().selected(); }
};

template<typename Range>
class ElementSelectionRange : public vcl::Range<ElementSelectionIterator<typename Range::iterator>>
{
	using Base = vcl::Range<ElementSelectionIterator<typename Range::iterator>>;
public:
	using Base::Range;
	ElementSelectionRange(const Range& r) :
			Base(ElementSelectionIterator(r.begin()), ElementSelectionIterator(r.end()))
	{
	}
};

} // namespace vcl

#endif // VCL_MESH_ITERATORS_ELEMENT_SELECTION_ITERATOR_H
