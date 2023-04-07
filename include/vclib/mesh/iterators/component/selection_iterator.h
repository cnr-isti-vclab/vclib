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

#include "component_view.h"

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
	using ElementType = typename std::conditional_t<
		IteratesOverClass<It>,
		typename std::remove_pointer_t<typename It::pointer>,
		typename std::remove_pointer_t<typename It::value_type>>;

	using BoolType = typename std::conditional_t <
		std::is_const_v<ElementType>,
		bool,
		BitProxy>;

	int m;
	mutable BitProxy bp = BitProxy(m, 0);

public:
	using value_type = BoolType;
	using reference  = typename std::conditional_t <
		std::is_const_v<ElementType>, bool, BitProxy&>;
	using pointer    = BoolType*;

	using It::It;
	SelectionIterator(const It& it) : It(it) {}

	reference operator*() const
	{
		if constexpr (std::is_const_v<ElementType>) {
			if constexpr (IteratesOverClass<It>) {
				return It::operator*().selected();
			}
			else {
				return It::operator*()->selected();
			}
		}
		else {
			if constexpr (IteratesOverClass<It>) {
				bp = It::operator*().selected();
			}
			else {
				bp = It::operator*()->selected();
			}
			return bp;
		}
	}
	pointer operator->() const
	{
		return &(operator*());
	}
};

#ifdef __clang__
template<std::ranges::range RngType>
class SelectionView : public vcl::View<SelectionIterator<std::ranges::iterator_t<RngType>>>
{
	using Base = vcl::View<SelectionIterator<std::ranges::iterator_t<RngType>>>;
public:
	SelectionView(RngType&& r) :
			Base(SelectionIterator(std::ranges::begin(r)), SelectionIterator(std::ranges::end(r)))
	{
	}
};
#else
template<std::ranges::range RngType>
using SelectionView = internal::ComponentView<RngType, SelectionIterator>;
#endif

} // namespace vcl

#endif // VCL_MESH_ITERATORS_COMPONENT_SELECTION_ITERATOR_H
