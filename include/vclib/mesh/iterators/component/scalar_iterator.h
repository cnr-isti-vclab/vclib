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

#ifndef VCL_MESH_ITERATORS_COMPONENT_SCALAR_ITERATOR_H
#define VCL_MESH_ITERATORS_COMPONENT_SCALAR_ITERATOR_H

#include "component_range.h"

#include <vclib/iterator/iterator_wrapper.h>

namespace vcl {

/**
 * @brief The ScalarIterator class allows to iterate over the scalars of the elements given
 * an iterator It that iterates over the elements.
 */
template<typename It>
class ScalarIterator : public It
{
	using ElementType = typename std::remove_pointer_t<typename It::pointer>;
	using ScalarType = typename std::conditional_t <
		std::is_const_v<ElementType>,
		const typename ElementType::ScalarType,
		typename ElementType::ScalarType>;

public:
	using value_type = typename std::remove_const_t<ScalarType>;
	using reference  = ScalarType&;
	using pointer    = ScalarType*;

	using It::It;
	ScalarIterator(It& it) : It(it) {}

	reference operator*() const { return It::operator*().scalar(); }
	pointer operator->() const { return &It::operator*().scalar(); }
};

// todo: remove this when clang will support P1814R0 (https://clang.llvm.org/cxx_status.html)
#ifdef __clang__
template<typename Rng>
class ScalarRange : public vcl::Range<ScalarIterator<typename Rng::iterator>>
{
	using Base = vcl::Range<ScalarIterator<typename Rng::iterator>>;
public:
	ScalarRange(const Rng& r) :
			Base(ScalarIterator(r.begin()), ScalarIterator(r.end()))
	{
	}
};
#else
template<typename Rng>
using ScalarRange = internal::ComponentRange<Rng, ScalarIterator>;
#endif

} // namespace vcl

#endif // VCL_MESH_ITERATORS_COMPONENT_SCALAR_ITERATOR_H
