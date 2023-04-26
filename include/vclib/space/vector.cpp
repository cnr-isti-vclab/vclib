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

#include "vector.h"
#include <algorithm>
#include <bits/ranges_base.h>
#include <vector>

namespace vcl {

/**
 * @brief Empty constructor of the Vector. If the container is dynamic, its size is 0. When the
 * container is static, the size is N and its elements are initialized with their empty constructor.
 */
template<typename T, int N>
Vector<T, N>::Vector()
{
}

/**
 * @brief Returns the size of the container, which is N if the container is static.
 * @return the size of the container.
 */
template<typename T, int N>
uint Vector<T, N>::size() const
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
template<typename T, int N>
T& Vector<T, N>::at(uint i)
{
	assert(i < size());
	return container[i];
}

template<typename T, int N>
const T& Vector<T, N>::at(uint i) const
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
template<typename T, int N>
T& Vector<T, N>::atMod(int i)
{
	int n = size(); // need to save n as int to avoid unwanted casts
	return container[(i % n + n) % n];
}

template<typename T, int N>
const T& Vector<T, N>::atMod(int i) const
{
	int n = size(); // need to save n as int to avoid unwanted casts
	return container[(i % n + n) % n];
}

template<typename T, int N>
void Vector<T, N>::set(const T& e, uint i)
{
	assert(i < size());
	container[i] = e;
}

template<typename T, int N>
template<Range Rng>
void Vector<T, N>::set(Rng&& r)
{
	if constexpr (N >= 0) {
		std::copy_n(std::ranges::begin(r), N, container.begin());
	}
	else {
		container = std::vector<T>(std::ranges::begin(r), std::ranges::end(r));
	}
}

template<typename T, int N>
void Vector<T, N>::fill(const T& e)
{
	std::fill(container.begin(), container.end(), e);
}

template<typename T, int N>
bool Vector<T, N>::contains(const typename MakeConstPointer<T>::type& e) const
{
	return std::find(container.begin(), container.end(), e) != container.end();
}

template<typename T, int N>
typename Vector<T, N>::Iterator
Vector<T, N>::find(const typename MakeConstPointer<T>::type& e)
{
	return std::find(container.begin(), container.end(), e);
}

template<typename T, int N>
typename Vector<T, N>::ConstIterator
Vector<T, N>::find(const typename MakeConstPointer<T>::type& e) const
{
	return std::find(container.begin(), container.end(), e);
}

template<typename T, int N>
int Vector<T, N>::indexOf(const typename MakeConstPointer<T>::type& e) const
{
	auto it = find(e);
	if (it == end())
		return -1;
	else
		return it - begin();
}

template<typename T, int N>
void Vector<T, N>::resize(uint n) requires (N < 0)
{
	container.resize(n);
}

template<typename T, int N>
void Vector<T, N>::pushBack(const T& v) requires (N < 0)
{
	container.push_back(v);
}

template<typename T, int N>
void Vector<T, N>::insert(uint i, const T& v) requires (N < 0)
{
	assert(i < size() + 1);
	container.insert(container.begin() + i, v);
}

template<typename T, int N>
void Vector<T, N>::erase(uint i) requires (N < 0)
{
	assert(i < size());
	container.erase(container.begin() + i);
}

template<typename T, int N>
void Vector<T, N>::clear() requires (N < 0)
{
	container.clear();
}

template<typename T, int N>
typename Vector<T, N>::Iterator Vector<T, N>::begin()
{
	return container.begin();
}

template<typename T, int N>
typename Vector<T, N>::Iterator Vector<T, N>::end()
{
	return container.end();
}

template<typename T, int N>
typename Vector<T, N>::ConstIterator Vector<T, N>::begin() const
{
	return container.begin();
}

template<typename T, int N>
typename Vector<T, N>::ConstIterator Vector<T, N>::end() const
{
	return container.end();
}

} // namespace vcl
