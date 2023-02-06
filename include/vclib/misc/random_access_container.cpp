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

#include "random_access_container.h"

namespace vcl {

/**
 * @brief Empty constructor of the container. If the container is dynamic, is size is 0. When the
 * container is static, the size is N and its elements are initialized with their empty constructor.
 */
template<typename C, int N>
RandomAccessContainer<C, N>::RandomAccessContainer()
{
}

/**
 * @brief Returns the size of the container, which is N if the container is static.
 * @return the size of the container.
 */
template<typename C, int N>
uint RandomAccessContainer<C, N>::size() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return container.size();
	}
}

/**
 * @brief Access the specified element with bounds checking.
 * @param i: the position of the element.
 * @return a reference of the element at position i.
 */
template<typename C, int N>
C& RandomAccessContainer<C, N>::at(uint i)
{
	assert(i < size());
	return container[i];
}

template<typename C, int N>
const C& RandomAccessContainer<C, N>::at(uint i) const
{
	assert(i < size());
	return container[i];
}

/**
 * @brief Access the specified element, computing first the module of the position w.r.t. the size
 * of the container.
 *
 * Takes into account negative indices: atMod(-1) will access to the last element of the container.
 *
 * @param i: the position of the element.
 * @return a reference of the element at position i % size().
 */
template<typename C, int N>
C& RandomAccessContainer<C, N>::atMod(int i)
{
	int n = size(); // need to save n as int to avoid unwanted casts
	return container[(i % n + n) % n];
}

template<typename C, int N>
const C& RandomAccessContainer<C, N>::atMod(int i) const
{
	int n = size(); // need to save n as int to avoid unwanted casts
	return container[(i % n + n) % n];
}

template<typename C, int N>
void RandomAccessContainer<C, N>::set(const C& e, uint i)
{
	assert(i < size());
	container[i] = e;
}

template<typename C, int N>
void RandomAccessContainer<C, N>::set(const std::vector<C>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		uint i = 0;
		for (const auto& v : list) {
			set(v, i);
			++i;
		}
	}
	else {
		container = list;
	}
}

template<typename C, int N>
bool RandomAccessContainer<C, N>::contains(const typename MakeConstPointer<C>::type& e) const
{
	return std::find(container.begin(), container.end(), e) != container.end();
}

template<typename C, int N>
typename RandomAccessContainer<C, N>::Iterator
RandomAccessContainer<C, N>::find(const typename MakeConstPointer<C>::type& e)
{
	return std::find(container.begin(), container.end(), e);
}

template<typename C, int N>
typename RandomAccessContainer<C, N>::ConstIterator
RandomAccessContainer<C, N>::find(const typename MakeConstPointer<C>::type& e) const
{
	return std::find(container.begin(), container.end(), e);
}

template<typename C, int N>
int RandomAccessContainer<C, N>::indexOf(const typename MakeConstPointer<C>::type& e) const
{
	auto it = find(e);
	if (it == end())
		return -1;
	else
		return it - begin();
}

template<typename C, int N>
void RandomAccessContainer<C, N>::resize(uint n) requires (N < 0)
{
	container.resize(n);
}

template<typename C, int N>
void RandomAccessContainer<C, N>::pushBack(const C& v) requires (N < 0)
{
	container.push_back(v);
}

template<typename C, int N>
void RandomAccessContainer<C, N>::insert(uint i, const C& v) requires (N < 0)
{
	assert(i < size() + 1);
	container.insert(container.begin() + i, v);
}

template<typename C, int N>
void RandomAccessContainer<C, N>::erase(uint i) requires (N < 0)
{
	assert(i < size());
	container.erase(container.begin() + i);
}

template<typename C, int N>
void RandomAccessContainer<C, N>::clear() requires (N < 0)
{
	container.clear();
}

template<typename C, int N>
typename RandomAccessContainer<C, N>::Iterator RandomAccessContainer<C, N>::begin()
{
	return container.begin();
}

template<typename C, int N>
typename RandomAccessContainer<C, N>::Iterator RandomAccessContainer<C, N>::end()
{
	return container.end();
}

template<typename C, int N>
typename RandomAccessContainer<C, N>::ConstIterator RandomAccessContainer<C, N>::begin() const
{
	return container.begin();
}

template<typename C, int N>
typename RandomAccessContainer<C, N>::ConstIterator RandomAccessContainer<C, N>::end() const
{
	return container.end();
}

template<typename C, int N>
typename RandomAccessContainer<C, N>::RACRangeIterator RandomAccessContainer<C, N>::rangeIterator()
{
	return RangeIterator(*this, &RandomAccessContainer::begin, &RandomAccessContainer::end);
}

template<typename C, int N>
typename RandomAccessContainer<C, N>::RACConstRangeIterator RandomAccessContainer<C, N>::rangeIterator() const
{
	return ConstRangeIterator(*this, &RandomAccessContainer::begin, &RandomAccessContainer::end);
}

} // namespace vcl
