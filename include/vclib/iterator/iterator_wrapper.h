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

#ifndef VCL_ITERATOR_ITERATOR_WRAPPER_H
#define VCL_ITERATOR_ITERATOR_WRAPPER_H

#include <vclib/types.h>

namespace vcl {

/**
 * @brief The IteratorWrapper class creates an random access iterator starting from a pointer,
 * exposing all the operators available to a pointer and exposing all the types that are normally
 * exposed by an iterator.
 *
 * It is useful when you need to treat some pointers to buffer data as iterators.
 * For example, the iterator of the stl container std::array is actually a typedef of a const
 * pointer to the data, and it cannot be treated as an actual iterator class (it is not a class or
 * struct, it cannot be inherited and it does not expose the common types exposed by iterators,
 * like value_type, reference, pointer, ...).
 *
 */
template<typename PointerType>
class IteratorWrapper
{
public:
	using value_type = typename std::remove_pointer_t<PointerType>;
	using reference = value_type&;
	using pointer = value_type*;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::random_access_iterator_tag;

	IteratorWrapper();
	IteratorWrapper(PointerType ptr);

	IteratorWrapper& operator++();

	IteratorWrapper operator++(int);

	IteratorWrapper& operator--();

	IteratorWrapper operator--(int);

	IteratorWrapper operator+(difference_type n) const;

	IteratorWrapper& operator+=(difference_type n);

	IteratorWrapper operator-(difference_type n) const;

	IteratorWrapper& operator-=(difference_type n);

	difference_type operator-(const IteratorWrapper& rhs) const;

	bool operator==(const IteratorWrapper& rhs) const;

	bool operator!=(const IteratorWrapper& rhs) const;

	bool operator<(const IteratorWrapper& rhs) const;

	bool operator<=(const IteratorWrapper& rhs) const;

	bool operator>(const IteratorWrapper& rhs) const;

	bool operator>=(const IteratorWrapper& rhs) const;

	reference operator*() const;

	pointer operator->() const;

	reference operator[](difference_type n) const;

private:
	PointerType ptr;
};

} // namespace vcl

#include "iterator_wrapper.cpp"

#endif // VCL_ITERATOR_ITERATOR_WRAPPER_H
