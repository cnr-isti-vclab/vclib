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

#include "array.h"

#include <iomanip>

namespace vcl {

/**
 * @brief Creates an N-Dimensional Array with size 0 for every dimension.
 */
template<class T, uint N>
Array<T, N>::Array() : v(0)
{
	sizes.fill(0);
}

/**
 * @brief Creates an N-Dimensional Array with the given sizes. All its elements are initialized to
 * 0.
 *
 * @param[in] s: N sizes, one for every dimension of the array.
 */
template<class T, uint N>
template<typename... Sizes>
Array<T, N>::Array(Sizes... s) requires(sizeof...(s) == N)
{
	std::size_t args[N]   = {static_cast<std::size_t>(s)...};
	std::size_t totalSize = 1;
	for (uint i = 0; i < N; i++) {
		sizes[i] = args[i];
		totalSize *= args[i];
	}
	v.resize(totalSize);
}

/**
 * @brief Creates and initializes an N-Dimensional Array. Sizes are given by the maximum size of the
 * initializer lists for every dimension, and missing values are automatically setted to zero.
 *
 * Example code:
 * @code{.cpp}
 * Array<int, 2> array = { {1,2,3,4}, {5}, {9, 10} };
 * @endcode
 *
 * This code initializes a 3x4 2D array with the following values:
 * @code{.cpp}
 *  1  2  3  4
 *  5  0  0  0
 *  9 10  0  0
 * @endcode
 *
 * @warning The number of levels of the nested initializer lists must correspond to the number of
 * dimensions of the array. The following example generates a compilation error:
 * @code{.cpp}
 * Array<int, 2> array = {1, 2, 3}; // Error: it is a 2 dimensional array but the initializer list
 *                                  // has one level.
 * @endcode
 * @param[in] values: the nested initializer lists of values.
 */
template<class T, uint N>
Array<T, N>::Array(NestedInitializerLists<T, N> values)
{
	initializeNestedLists(values);
}

template<class T, uint N>
bool Array<T, N>::empty() const
{
	return v.empty();
}

/**
 * @brief Returns the size of the given dimension.
 * @param[in] dim
 */
template<class T, uint N>
std::size_t Array<T, N>::size(std::size_t dim) const
{
	assert(dim < N);
	return sizes[dim];
}

template<class T, uint N>
std::size_t Array<T, N>::rows() const requires (N == 2)
{
	return sizes[0];
}

template<class T, uint N>
std::size_t Array<T, N>::cols() const requires (N == 2)
{
	return sizes[1];
}

template<class T, uint N>
std::size_t Array<T, N>::sizeX() const requires (N >= 1)
{
	return sizes[0];
}

template<class T, uint N>
std::size_t Array<T, N>::sizeY() const requires (N >= 2)
{
	return sizes[1];
}

template<class T, uint N>
std::size_t Array<T, N>::sizeZ() const requires (N >= 3)
{
	return sizes[2];
}

template<class T, uint N>
std::size_t Array<T, N>::sizeW() const requires (N >= 4)
{
	return sizes[3];
}

/**
 * @brief Operator () that allows to access one element of the array. It can be used as left or
 * right value.
 * @param[in] indices: N indices that allows to access to an element of the array.
 * A number of indices not equal to N will generate a compilation error.
 * @return a reference to the element of the array.
 */
template<class T, uint N>
template<typename... I>
typename Array<T, N>::Reference Array<T, N>::operator()(I... indices) requires(sizeof...(indices) == N)
{
	std::size_t args[N] = {static_cast<std::size_t>(indices)...};
	return v[getIndex(args)];
}

/**
 * @brief Operator () that allows to access one element of the array. It can be used only as right
 * value.
 * @param[in] indices: N indices that allows to access to an element of the array.
 * A number of indices not equal to N will generate a compilation error.
 * @return a reference to the element of the array.
 */
template<class T, uint N>
template<typename... I>
typename Array<T, N>::ConstReference Array<T, N>::operator()(I... indices) const
	requires(sizeof...(indices) == N)
{
	std::size_t args[N] = {static_cast<std::size_t>(indices)...};
	return v[getIndex(args)];
}

/**
 * @brief Allows to get a C array of the Array, that can be also modified.
 * Example:
 * @code{.cpp}
 * Array<int, 3> array(10, 13, 4);
 * //...
 * int* carray = array.cArray(3); //carray will point to the element in position (3,0,0).
 * for (uint i = 0; i < 13*4; i++)
 *    std::cout << carry[i]; // will print all the elements of the sub array starting from
 *                           // position (3,0,0).
 *
 * carray = array.cArray(4, 2); // carray will point to the element in position (4, 2, 0).
 * for (uint i = 0; i < 4; i++)
 *    std::cout << carry[i]; // will print all the elements of the sub array starting from
 *                           // position (4,2,0).
 *
 * carray = array.cArray(); // carray will point to the element in position (0, 0, 0).
 *
 * @endcode
 *
 * @param[in] indices: a number of indices that is less than the number of dimensions of the array.
 * @return a C array starting from the indicized element.
 */
template<class T, uint N>
template<typename... I>
typename Array<T, N>::Pointer Array<T, N>::cArray(I... indices) requires(sizeof...(indices) < N)
{
	const std::size_t n = sizeof...(indices);
	if (n == 0) {
		return v.data();
	}
	std::size_t args[] = {static_cast<std::size_t>(indices)...};
	std::size_t ind    = args[0];
	assert(args[0] < sizes[0]);
	uint i;
	for (i = 1; i < n; i++) {
		assert(args[i] < sizes[i]);
		ind *= sizes[i];
		ind += args[i];
	}
	for (; i < N; i++) {
		ind *= sizes[i];
	}
	return &v[ind];
}

/**
 * @brief Allows to get a C array of the Array, that cannot be modified.
 * Example:
 * @code{.cpp}
 * Array<int, 3> array(10, 13, 4);
 * //...
 * const int* carray = array.cArray(3); //carray will point to the element in position (3,0,0).
 * for (uint i = 0; i < 13*4; i++)
 *    std::cout << carry[i]; // will print all the elements of the sub 2D array starting from
 *                           // (3,0,0).
 *
 * carray = array.cArray(4, 2); // carray will point to the element in position (4, 2, 0).
 * for (uint i = 0; i < 4; i++)
 *    std::cout << carry[i]; // will print all the elements of the sub 1D array starting from
 *                           // (4,2,0).
 *
 * carray = array.cArray(); // carray will point to the element in position (0, 0, 0).
 *
 * @endcode
 *
 * @param[in] indices: a number of indices that is less than the number of dimensions of the array.
 * @return a C array starting from the indicized element.
 */
template<class T, uint N>
template<typename... I>
typename Array<T, N>::ConstPointer Array<T, N>::cArray(I... indices) const requires(sizeof...(indices) < N)
{
	const std::size_t n = sizeof...(indices);
	if (n == 0) {
		return v.data();
	}
	std::size_t args[] = {static_cast<std::size_t>(indices)...};
	std::size_t ind    = args[0];
	assert(args[0] < sizes[0]);
	uint i;
	for (i = 1; i < n; i++) {
		assert(args[i] < sizes[i]);
		ind *= sizes[i];
		ind += args[i];
	}
	for (; i < N; i++) {
		ind *= sizes[i];
	}
	return &v[ind];
}

template<class T, uint N>
typename Array<T, N>::Pointer Array<T, N>::data()
{
	return v.data();
}

template<class T, uint N>
typename Array<T, N>::ConstPointer Array<T, N>::data() const
{
	return v.data();
}

/**
 * @brief Returns a std::vector containing the elements of the array in row-major order
 */
template<class T, uint N>
std::vector<T> Array<T, N>::stdVector()
{
	return v;
}

/**
 * @brief Returns a std::vector containing the elements of the array in row-major order
 */
template<class T, uint N>
const std::vector<T>& Array<T, N>::stdVector() const
{
	return v;
}

/**
 * @brief Fills the entire Array with the value t.
 * @param[in] t
 */
template<class T, uint N>
void Array<T, N>::fill(const T& t)
{
	std::fill(v.begin(), v.end(), t);
}

/**
 * @brief Fills the entire Array with the values contained in the range r, in row-major
 * order.
 *
 * If the size of the container is greater than the total size of the array, the remaining
 * of the container will be ignored. If otherwise the size of the container is less, the
 * remaining values in the array will be left as they were.
 *
 * @param[in] r: a range of the same type of the array (begin() and end() members must
 * be provided in the container).
 */
template<class T, uint N>
template<Range Rng>
void Array<T, N>::fill(Rng&& r)
{
	uint i = 0;
	for (auto it = std::ranges::begin(r); it != std::ranges::end(r) && i < v.size(); ++i, ++it)
		v[i] = *it;
}

/**
 * @brief Allows to resize the Array, not conserving the values of the previous array.
 * @param[in] s: N elements representing the new sizes of the Array.
 */
template<class T, uint N>
template<typename... Sizes>
void Array<T, N>::resize(Sizes... s) requires(sizeof...(s) == N)
{
	std::size_t args[N]   = {static_cast<std::size_t>(s)...};
	std::size_t totalSize = 1;
	for (uint i = 0; i < N; i++) {
		sizes[i] = args[i];
		totalSize *= args[i];
	}
	v.resize(totalSize);
}

/**
 * @brief Allows to resize the Array, conserving the values of the previous array.
 * @param[in] s: N elements representing the new sizes of the Array.
 */
template<class T, uint N>
template<typename... Sizes>
void Array<T, N>::conservativeResize(Sizes... s) requires(sizeof...(s) == N)
{
	std::size_t newSizes[N]  = {static_cast<std::size_t>(s)...};
	std::size_t newTotalSize = 1;
	for (uint i = 0; i < N; i++)
		newTotalSize *= newSizes[i];
	std::vector<T> newVector(newTotalSize);

	for (std::size_t i = 0; i < v.size(); i++) {
		std::array<std::size_t, N> indices  = reverseIndex(i);
		bool outOfBound = false;
		for (std::size_t j = 0; j < N; j++)
			if (indices[j] >= newSizes[j] || indices[j] >= sizes[j])
				outOfBound = true;
		if (!outOfBound) {
			newVector[getIndex(indices.data(), newSizes)] = v[i];
		}
	}

	for (uint i = 0; i < sizes.size(); i++) {
		sizes[i] = newSizes[i];
	}

	v = std::move(newVector);
}

/**
 * @brief Clear the entire array, setting every dimension to size 0.
 */
template<class T, uint N>
void Array<T, N>::clear()
{
	v.clear();
	for (uint i = 0; i < N; i++)
		sizes[i] = 0;
}

/**
 * @brief Creates a new subArray of dimension N-1, starting from the given index at its first
 * dimension.
 *
 * Example:
 * @code{.cpp}
 * Array<int, 3> a(4,2,6);
 * Array<int, 2> sa = a.subArray(1);
 * // sa is a 2x6 2D Array, containing the elements at the second "row" of Array a.
 * @endcode
 *
 * @param r
 */
template<class T, uint N>
Array<T, N - 1> Array<T, N>::subArray(uint r) const requires (N > 1)
{
	assert(r < sizes[0]);
	Array<T, N - 1> sub;
	size_t          size = 1;
	for (uint i = 0; i < sizes.size() - 1; i++) {
		sub.sizes[i] = sizes[i + 1];
		size *= sub.sizes[i];
	}
	sub.v = std::vector<T>(v.begin() + r * size, v.begin() + (r + 1) * size);
	return sub;
}

template<class T, uint N>
std::size_t Array<T, N>::getIndex(const std::size_t indices[]) const
{
	std::size_t ind = indices[0];
	assert(indices[0] < sizes[0]);
	for (uint i = 1; i < N; i++) {
		assert(indices[i] < sizes[i]);
		ind *= sizes[i];
		ind += indices[i];
	}
	return ind;
}

template<class T, uint N>
std::array<std::size_t, N> Array<T, N>::reverseIndex(uint index)
{
	std::array<std::size_t, N> indices;
	for (long int i = N - 1; i >= 0; i--) {
		indices[i] = index % sizes[i];
		index /= sizes[i];
	}
	return indices;
}

template<class T, uint N>
std::size_t Array<T, N>::getIndex(const std::size_t indices[], const std::size_t sizes[])
{
	std::size_t ind = indices[0];
	assert(indices[0] < sizes[0]);
	for (uint i = 1; i < N; i++) {
		assert(indices[i] < sizes[i]);
		ind *= sizes[i];
		ind += indices[i];
	}
	return ind;
}

template<typename T, uint N>
void Array<T, N>::initializeNestedLists(NestedInitializerLists<T, N> values)
{
	std::list<std::size_t> szs = NestedInitializerListsProcessor<T, N>::maxDimensionsLevels(values);

	uint   i         = 0;
	size_t totalSize = 1;
	for (std::size_t s : szs) {
		totalSize *= s;
		sizes[i++] = s;
	}
	v.resize(totalSize);

	typename std::vector<T>::iterator iterator = v.begin();
	NestedInitializerListsProcessor<T, N>::processElements(
		values, [&iterator](T value) { *(iterator++) = value; }, szs);
}

template<typename Scalar>
std::ostream& operator<<(std::ostream& out, const Array<Scalar, 2>& a)
{
	for (uint i = 0; i < a.sizeX(); i++) {
		for (uint j = 0; j < a.sizeY(); j++) {
			out << std::setw(4) << a(i, j) << " ";
		}
		out << std::endl;
	}
	return out;
}

} // namespace vcl
