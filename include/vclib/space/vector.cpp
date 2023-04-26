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
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace vcl {

/**
 * @brief Creates an empty Vector object.
 *
 * If the container is dynamic, its size is 0. When the container is static, the size is N and its
 * elements are initialized with their empty constructor.
 */
template<typename T, int N>
Vector<T, N>::Vector()
{
}

/**
 * @brief Returns the size of the container.
 *
 * If the container is static, the size is N. If the container is dynamic, the size is determined by
 * the number of elements currently stored.
 *
 * @return The size of the container.
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
 *
 * Returns a reference to the element at position `i` in the Vector, with bounds checking. If `i` is
 * not within the range of valid indices for the Vector, an `std::out_of_range` exception is thrown.
 *
 * @param[in] i: The position of the element.
 * @return A reference to the element at position `i`.
 *
 * @throws std::out_of_range If `i` is not within the range of valid indices for the Vector.
 */
template<typename T, int N>
typename Vector<T, N>::Reference Vector<T, N>::at(uint i)
{
	return container.at(i);
}

/**
 * @brief Access the specified element with bounds checking.
 *
 * Returns a const reference to the element at position `i` in the Vector, with bounds checking. If
 * `i` is not within the range of valid indices for the Vector, an `std::out_of_range` exception is
 * thrown.
 *
 * @param[in] i: The position of the element.
 * @return A const reference to the element at position `i`.
 *
 * @throws std::out_of_range If `i` is not within the range of valid indices for the Vector.
 */
template<typename T, int N>
typename Vector<T, N>::ConstReference Vector<T, N>::at(uint i) const
{
	return container.at(i);
}

/**
 * @brief Access the specified element, computing first the module of the position w.r.t. the size
 * of the container.
 *
 * Takes into account negative indices: atMod(-1) will access to the last element of the container.
 *
 * @param[in] i: The position of the element.
 * @return A reference to the element at position i % size().
 */
template<typename T, int N>
typename Vector<T, N>::Reference Vector<T, N>::atMod(int i)
{
	int n = size(); // need to save n as int to avoid unwanted casts
	return container[(i % n + n) % n];
}

/**
 * @brief Access the specified element, computing first the module of the position w.r.t. the size
 * of the container.
 *
 * Takes into account negative indices: atMod(-1) will access to the last element of the container.
 *
 * @param[in] i The position of the element.
 * @return A const reference to the element at position i % size().
 */
template<typename T, int N>
typename Vector<T, N>::ConstReference Vector<T, N>::atMod(int i) const
{
	int n = size(); // need to save n as int to avoid unwanted casts
	return container[(i % n + n) % n];
}

/**
 * @brief Set the value of the element at the specified position.
 *
 * Sets the value of the element at position `i` in the Vector to the specified value `e`.
 *
 * @param[in] e: The new value of the element.
 * @param[in] i: The position of the element.
 */
template<typename T, int N>
void Vector<T, N>::set(const T& e, uint i)
{
	assert(i < size());
	container[i] = e;
}

/**
 * @brief Set the elements of the Vector using the values from a range.
 *
 * Sets the elements of the Vector to the values from the range `r`. If the size of the Vector is
 * known at compile-time and is not negative, the first N elements from the range `r` are copied
 * into the Vector using `std::copy_n()`. If the size of the Vector is negative or not known at
 * compile-time, the elements of the Vector are set to the values from the range `r` by
 * constructing a new std::vector from the range `r`.
 *
 * @tparam Rng: The type of the range.
 *
 * @param[in] r: The range of values to set the elements of the Vector to.
 */
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

/**
 * @brief Fill all elements of the Vector with the specified value.
 *
 * Sets all elements of the Vector to the value specified by `e`, using the `std::fill()` algorithm.
 *
 * @param[in] e: The value to fill the Vector with.
 */
template<typename T, int N>
void Vector<T, N>::fill(const T& e)
{
	std::fill(container.begin(), container.end(), e);
}

/**
 * @brief Check if the Vector contains the specified element.
 *
 * Checks if the Vector contains the element specified by `e`, using the `std::find()` algorithm.
 *
 * @param[in] e: The element to search for in the Vector.
 * @return `true` if the element is found in the Vector, `false` otherwise.
 */
template<typename T, int N>
bool Vector<T, N>::contains(const typename MakeConstPointer<T>::type& e) const
{
	return std::find(container.begin(), container.end(), e) != container.end();
}

/**
 * @brief Find the first occurrence of the specified element in the Vector.
 *
 * Finds the first occurrence of the element specified by `e` in the Vector, using the `std::find()`
 * algorithm.
 *
 * @param[in] e: The element to search for in the Vector.
 * @return An iterator to the first occurrence of the element in the Vector, or `end()` if the
 * element is not found.
 */
template<typename T, int N>
typename Vector<T, N>::Iterator Vector<T, N>::find(const typename MakeConstPointer<T>::type& e)
{
	return std::find(container.begin(), container.end(), e);
}

/**
 * @brief Find the first occurrence of the specified element in the Vector.
 *
 * Finds the first occurrence of the element specified by `e` in the Vector, using the `std::find()`
 * algorithm.
 *
 * @param[in] e: The element to search for in the Vector.
 * @return A const iterator to the first occurrence of the element in the Vector, or `end()` if the
 * element is not found.
 */
template<typename T, int N>
typename Vector<T, N>::ConstIterator
Vector<T, N>::find(const typename MakeConstPointer<T>::type& e) const
{
	return std::find(container.begin(), container.end(), e);
}

/**
 * @brief Get the index of the first occurrence of the specified element in the Vector.
 *
 * Finds the first occurrence of the element specified by `e` in the Vector, using the `std::find()`
 * algorithm, and returns its index. If the element is not found in the Vector, the member function
 * returns -1.
 *
 * @param[in] e: The element to search for in the Vector.
 * @return The index of the first occurrence of the element in the Vector, or -1 if the element is
 * not found.
 */
template<typename T, int N>
int Vector<T, N>::indexOf(const typename MakeConstPointer<T>::type& e) const
{
	auto it = find(e);
	if (it == end())
		return -1;
	else
		return it - begin();
}

/**
 * @brief Resize the Vector to the specified size.
 *
 * Resizes the Vector to the specified size `n` by resizing the underlying `std::vector`. This
 * member function is only available if the size of the Vector is not known at compile-time, as
 * specified by the concept requirement `requires (N < 0)`.
 *
 * @param[in] n: The new size of the Vector.
 */
template<typename T, int N>
void Vector<T, N>::resize(uint n) requires (N < 0)
{
	container.resize(n);
}

/**
 * @brief Add an element to the end of the Vector.
 *
 * Adds the element `v` to the end of the Vector by calling the `push_back()` member function of the
 * underlying `std::vector`. This member function is only available if the size of the Vector is not
 * known at compile-time, as specified by the concept requirement `requires (N < 0)`.
 *
 * @param[in] v: The value to add to the end of the Vector.
 */
template<typename T, int N>
void Vector<T, N>::pushBack(const T& v) requires (N < 0)
{
	container.push_back(v);
}

/**
 * @brief Insert an element at the specified position in the Vector.
 *
 * Inserts the element `v` at the position specified by `i` in the Vector by calling the `insert()`
 * member function of the underlying `std::vector`. This member function is only available if the
 * size of the Vector is not known at compile-time, as specified by the concept requirement
 * `requires (N < 0)`.
 *
 * @param[in] i: The index at which to insert the element.
 * @param[in] v: The value to insert into the Vector.
 */
template<typename T, int N>
void Vector<T, N>::insert(uint i, const T& v) requires (N < 0)
{
	assert(i < size() + 1);
	container.insert(container.begin() + i, v);
}

/**
 * @brief Remove the element at the specified index from the Vector.
 *
 * Removes the element at the position specified by `i` in the Vector by calling the `erase()`
 * member function of the underlying `std::vector`. This member function is only available if the
 * size of the Vector is not known at compile-time, as specified by the concept requirement
 * `requires (N < 0)`.
 *
 * @param[in] i: The index of the element to remove from the Vector.
 */
template<typename T, int N>
void Vector<T, N>::erase(uint i) requires (N < 0)
{
	assert(i < size());
	container.erase(container.begin() + i);
}

/**
 * @brief Remove all elements from the Vector.
 *
 * Removes all elements from the Vector by calling the `clear()` member function of the underlying
 * `std::vector`. This member function is only available if the size of the Vector is not known at
 * compile-time, as specified by the concept requirement `requires (N < 0)`.
 */
template<typename T, int N>
void Vector<T, N>::clear() requires (N < 0)
{
	container.clear();
}

/**
 * @brief Return an iterator pointing to the beginning of the Vector.
 *
 * @return An iterator pointing to the beginning of the Vector.
 */
template<typename T, int N>
typename Vector<T, N>::Iterator Vector<T, N>::begin()
{
	return container.begin();
}

/**
 * @brief Return an iterator pointing to the end of the Vector.
 *
 * @return An iterator pointing to the end of the Vector.
 */
template<typename T, int N>
typename Vector<T, N>::Iterator Vector<T, N>::end()
{
	return container.end();
}

/**
 * @brief Return a const iterator pointing to the beginning of the Vector.
 *
 * @return A const iterator pointing to the beginning of the Vector.
 */
template<typename T, int N>
typename Vector<T, N>::ConstIterator Vector<T, N>::begin() const
{
	return container.begin();
}

/**
 * @brief Return a const iterator pointing to the end of the Vector.
 *
 * @return A const iterator pointing to the end of the Vector.
 */
template<typename T, int N>
typename Vector<T, N>::ConstIterator Vector<T, N>::end() const
{
	return container.end();
}

} // namespace vcl
