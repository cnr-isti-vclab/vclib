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

#include "markable_vector.h"

namespace vcl {

template<typename ValueType>
MarkableVector<ValueType>::MarkableVector()
{
}

template<typename ValueType>
MarkableVector<ValueType>::MarkableVector(std::size_t size) :
		vector(size), marks(size, 0)
{
}

template<typename ValueType>
MarkableVector<ValueType>::MarkableVector(std::size_t size, const ValueType& defaultValue) :
		vector(size, defaultValue), marks(size, 0)
{
}

template<typename ValueType>
template<typename ValueIterator>
MarkableVector<ValueType>::MarkableVector(ValueIterator begin, ValueIterator end) :
		vector(begin, end), marks(std::distance(begin, end), 0)
{
}

template<typename ValueType>
bool MarkableVector<ValueType>::empty() const
{
	return vector.empty();
}

template<typename ValueType>
std::size_t MarkableVector<ValueType>::size() const
{
	return vector.size();
}

template<typename ValueType>
void MarkableVector<ValueType>::clear()
{
	vector.clear();
	marks.clear();
	m = 1;
}

template<typename ValueType>
void MarkableVector<ValueType>::reserve(std::size_t size)
{
	vector.reserve(size);
	marks.reserve(size);
}

template<typename ValueType>
std::size_t MarkableVector<ValueType>::capacity() const
{
	return vector.capacity();
}

template<typename ValueType>
void MarkableVector<ValueType>::resize(std::size_t size)
{
	vector.resize(size);
	marks.resize(size, 0);
}

template<typename ValueType>
void MarkableVector<ValueType>::resize(std::size_t size, const ValueType& defaultValue)
{
	vector.resize(size, defaultValue);
	marks.resize(size, 0);
}

template<typename ValueType>
void MarkableVector<ValueType>::insert(uint p, const ValueType& v)
{
	vector.insert(vector.begin() + p, v);
	marks.insert(marks.begin() + p, 0);
}

template<typename ValueType>
void MarkableVector<ValueType>::erase(uint p)
{
	vector.erase(vector.begin() + p);
	marks.erase(marks.begin() + p);
}

template<typename ValueType>
void MarkableVector<ValueType>::pushBack(const ValueType& v)
{
	vector.push_back(v);
	marks.push_back(0);
}

template<typename ValueType>
void MarkableVector<ValueType>::popBack()
{
	vector.pop_back();
	marks.pop_back();
}

template<typename ValueType>
bool MarkableVector<ValueType>::isMarked(uint i) const
{
	return marks.at(i) == m;;
}

template<typename ValueType>
void MarkableVector<ValueType>::mark(uint i) const
{
	marks.at(i) = m;
}

template<typename ValueType>
void MarkableVector<ValueType>::unMarkAll() const
{
	m++;
}

template<typename ValueType>
ValueType* MarkableVector<ValueType>::data()
{
	return vector.data();
}

template<typename ValueType>
const ValueType* MarkableVector<ValueType>::data() const
{
	return vector.data();
}

template<typename ValueType>
ValueType& MarkableVector<ValueType>::at(uint i)
{
	return vector.at(i);
}

template<typename ValueType>
const ValueType& MarkableVector<ValueType>::at(uint i) const
{
	return vector.at(i);
}

template<typename ValueType>
ValueType& MarkableVector<ValueType>::operator[](uint i)
{
	return vector[i];
}

template<typename ValueType>
const ValueType& MarkableVector<ValueType>::operator[](uint i) const
{
	return vector[i];
}

template<typename ValueType>
ValueType& MarkableVector<ValueType>::front()
{
	return vector.front();
}

template<typename ValueType>
const ValueType& MarkableVector<ValueType>::front() const
{
	return vector.front();
}

template<typename ValueType>
ValueType& MarkableVector<ValueType>::back()
{
	return vector.back();
}

template<typename ValueType>
const ValueType& MarkableVector<ValueType>::back() const
{
	return vector.back();
}

template<typename ValueType>
typename MarkableVector<ValueType>::Iterator MarkableVector<ValueType>::begin()
{
	return vector.begin();
}

template<typename ValueType>
typename MarkableVector<ValueType>::ConstIterator MarkableVector<ValueType>::begin() const
{
	return vector.begin();
}

template<typename ValueType>
typename MarkableVector<ValueType>::Iterator MarkableVector<ValueType>::end()
{
	return vector.end();
}

template<typename ValueType>
typename MarkableVector<ValueType>::ConstIterator MarkableVector<ValueType>::end() const
{
	return vector.end();
}

} // namespace vcl
