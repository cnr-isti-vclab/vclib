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

#ifndef VCL_MESH_VIEWS_COMPONENTS_SELECTION_H
#define VCL_MESH_VIEWS_COMPONENTS_SELECTION_H

#include <vclib/mesh/iterators/component/selection_iterator.h>

#include "component_view.h"

namespace vcl {

#ifdef __clang__
template<std::ranges::range RngType>
class SelectionView : public vcl::View<SelectionIterator<std::ranges::iterator_t<RngType>>>
{
	using Base = vcl::View<SelectionIterator<std::ranges::iterator_t<RngType>>>;
public:
	SelectionView(const RngType& r) :
			Base(SelectionIterator(std::ranges::begin(r)), SelectionIterator(std::ranges::end(r)))
	{
	}
};
#else
template<std::ranges::range RngType>
using SelectionView = internal::ComponentView<RngType, SelectionIterator>;
#endif

namespace views {

namespace internal {

struct SelectionViewClosure
{
	constexpr SelectionViewClosure(){}

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		return SelectionView(r);
	}
};

namespace sel {
template<typename T>
struct ET{
	using t = std::remove_pointer_t<T>;
};

template<IteratorConcept T>
struct ET<T>{
	using t = typename T::value_type;
};
}

template<typename Element>
inline constexpr auto isSelected = [](const Element& e)
{
	if constexpr(vcl::IsPointer<Element>) {
		return e->isSelected();
	}
	else {
		return e.isSelected();
	}
};

struct SelectedViewClosure
{
	constexpr SelectedViewClosure(){}

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		using CleanRange = std::remove_const_t<std::remove_reference_t<R>>;
		using ElemType = sel::ET<typename CleanRange::iterator>::t;
		return r | std::views::filter(isSelected<ElemType>);
	}
};

template<typename Element>
inline constexpr auto isNotSelected = [](const Element& e)
{
	if constexpr(vcl::IsPointer<Element>) {
		return !e->isSelected();
	}
	else {
		return !e.isSelected();
	}
};

struct NotSelectedViewClosure
{
	constexpr NotSelectedViewClosure(){}

	template <std::ranges::range R>
	constexpr auto operator()(R && r) const
	{
		using CleanRange = std::remove_const_t<std::remove_reference_t<R>>;
		using ElemType = sel::ET<typename CleanRange::iterator>::t;
		return r | std::views::filter(isNotSelected<ElemType>);
	}
};

} // namespace vcl::views::internal

inline constexpr internal::SelectionViewClosure selection;
inline constexpr internal::SelectedViewClosure selected;
inline constexpr internal::NotSelectedViewClosure notSelected;

} // namespace vcl::views

} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_SELECTION_H
