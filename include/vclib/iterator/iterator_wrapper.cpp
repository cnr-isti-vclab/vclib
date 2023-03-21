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

#include "iterator_wrapper.h"

namespace vcl {

template<typename PointerType>
IteratorWrapper<PointerType>::IteratorWrapper()
{
}

template<typename PointerType>
IteratorWrapper<PointerType>::IteratorWrapper(PointerType ptr) : ptr(ptr)
{
}

template<typename PointerType>
IteratorWrapper<PointerType>& IteratorWrapper<PointerType>::operator++()
{
	++ptr;
	return *this;
}

template<typename PointerType>
IteratorWrapper<PointerType> IteratorWrapper<PointerType>::operator++(int)
{
	IteratorWrapper tmp(*this);
	++ptr;
	return tmp;
}

template<typename PointerType>
IteratorWrapper<PointerType> IteratorWrapper<PointerType>::operator--(int)
{
	IteratorWrapper tmp(*this);
	--ptr;
	return tmp;
}

template<typename PointerType>
IteratorWrapper<PointerType>& IteratorWrapper<PointerType>::operator--()
{
	--ptr;
	return *this;
}

template<typename PointerType>
IteratorWrapper<PointerType> IteratorWrapper<PointerType>::operator+(difference_type n) const
{
	return IteratorWrapper(ptr + n);
}

template<typename PointerType>
IteratorWrapper<PointerType>& IteratorWrapper<PointerType>::operator+=(difference_type n) {
	ptr += n;
	return *this;
}



template<typename PointerType>
IteratorWrapper<PointerType> IteratorWrapper<PointerType>::operator-(difference_type n) const {
	return IteratorWrapper(ptr - n);
}

template<typename PointerType>
bool IteratorWrapper<PointerType>::operator==(const IteratorWrapper& rhs) const {
	return ptr == rhs.ptr;
}

template<typename PointerType>
bool IteratorWrapper<PointerType>::operator!=(const IteratorWrapper& rhs) const {
	return ptr != rhs.ptr;
}

template<typename PointerType>
bool IteratorWrapper<PointerType>::operator<(const IteratorWrapper& rhs) const {
	return ptr < rhs.ptr;
}

template<typename PointerType>
bool IteratorWrapper<PointerType>::operator<=(const IteratorWrapper& rhs) const {
	return ptr <= rhs.ptr;
}

template<typename PointerType>
bool IteratorWrapper<PointerType>::operator>(const IteratorWrapper& rhs) const {
	return ptr > rhs.ptr;
}

template<typename PointerType>
bool IteratorWrapper<PointerType>::operator>=(const IteratorWrapper& rhs) const {
	return ptr >= rhs.ptr;
}

template<typename PointerType>
typename IteratorWrapper<PointerType>::reference IteratorWrapper<PointerType>::operator*() const {
	return *ptr;
}

template<typename PointerType>
typename IteratorWrapper<PointerType>::pointer IteratorWrapper<PointerType>::operator->() const {
	return ptr;
}

template<typename PointerType>
typename IteratorWrapper<PointerType>::reference IteratorWrapper<PointerType>::operator[](difference_type n) const {
	return *(ptr + n);
}

template<typename PointerType>
IteratorWrapper<PointerType>& IteratorWrapper<PointerType>::operator-=(difference_type n) {
	ptr -= n;
	return *this;
}

template<typename PointerType>
typename IteratorWrapper<PointerType>::difference_type IteratorWrapper<PointerType>::operator-(const IteratorWrapper& rhs) const {
	return ptr - rhs.ptr;
}

} // namespace vcl
