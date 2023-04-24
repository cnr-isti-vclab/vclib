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

#ifndef VCL_SPACE_ARRAY_H
#define VCL_SPACE_ARRAY_H

#include <assert.h>
#include <array>
#include <vector>

#include <vclib/concepts/range.h>
#include <vclib/misc/nested_initializer_lists.h>
#include <vclib/types.h>

namespace vcl {

/**
 * @brief The Array class
 * is a dynamically allocated N-dimensional array, stored in RowWise mode.
 * All its elements are stored contiguously. For array 1D please use std::vector or std::array.
 *
 * For D-dimensional array you can declare your array in this way:
 * @code{.cpp}
 * vcl::Array<Type, D> array;
 * @endcode
 *
 * Where Type is a generic primitive or user-defined type and D is an unsigned integer that
 * represents the dimensions of the array.
 *
 * You can also specify the sizes of the array on initialization:
 * @code{.cpp}
 * vcl::Array<int, 3> array(n,m,l);
 * @endcode
 *
 * In this example, array is a 3-dimensional array with sizes n*m*l.
 * The sizes of the array can be accessed using "size" member function.
 * You can access to its elements by using () operator:
 * @code{.cpp}
 * for (unsigned int i = 0; i < array.size(0); i++)
 *     for (unsigned int j = 0; j < array.size(1); j++)
 *         for (unsigned int k = 0; k < array.size(2); k++)
 *             array(i,j,k) = someFunction();
 * @endcode
 *
 * You can also declare and initialize a generic N-Dimensional Array with nested initializer lists
 * (vcl::NestedInitializerLists).
 *
 * \code{.cpp}
 * vcl::Array<int, 3> array =
 * {
 *     {
 *         {1, 2, 3},
 *         {1, 2   }
 *     },
 *     {
 *         {1},
 *         {1, 4, 7},
 *         {2, 4},
 *         {5}
 *     }
 * }
 * \endcode
 *
 * In this example, array is a 3-dimensional array with sizes 2*4*3.
 * All the missing numbers are filled with zeros (in every dimension).
 *
 * @ingroup space
 */
template<class T, uint N>
class Array
{
	static_assert(N > 0, "Array dimension must be > 0.");
	friend class Array<T, N + 1>;

public:
	using ValueType      = typename std::vector<T>::value_type;
	using ConstReference = typename std::vector<T>::const_reference;
	using Reference      = typename std::vector<T>::reference;
	using ConstPointer   = typename std::vector<T>::const_pointer;
	using Pointer        = typename std::vector<T>::pointer;
	using Iterator       = typename std::vector<T>::iterator;
	using ConstIterator  = typename std::vector<T>::const_iterator;

	static constexpr uint DIM = N;

	Array();

	template<typename... Sizes>
	Array(Sizes... sizes) requires(sizeof...(sizes) == N);

	Array(vcl::NestedInitializerLists<T, N> values);

	bool empty() const;

	std::size_t size(std::size_t dim) const;

	std::size_t rows() const requires (N == 2);
	std::size_t cols() const requires (N == 2);

	std::size_t sizeX() const requires (N >= 1);
	std::size_t sizeY() const requires (N >= 2);
	std::size_t sizeZ() const requires (N >= 3);
	std::size_t sizeW() const requires (N >= 4);

	template<typename... I>
	Reference operator()(I... indices) requires(sizeof...(indices) == N);

	template<typename... I>
	ConstReference operator()(I... indices) const requires(sizeof...(indices) == N);

	template<typename... I>
	Pointer cArray(I... indices) requires(sizeof...(indices) < N);

	template<typename... I>
	ConstPointer cArray(I... indices) const requires(sizeof...(indices) < N);

	Pointer               data();
	ConstPointer          data() const;
	std::vector<T>        stdVector();
	const std::vector<T>& stdVector() const;

	void fill(const T& t);

	template<Range Rng>
	void fill(Rng&& r);

	template<typename... Sizes>
	void resize(Sizes... s) requires(sizeof...(s) == N);

	template<typename... Sizes>
	void conservativeResize(Sizes... s) requires(sizeof...(s) == N);

	void clear();

	Array<T, N - 1> subArray(uint r) const requires (N > 1);

		   /// @private
	template<typename S>
	friend std::ostream& operator<<(std::ostream& out, const Array<S, 2>& a);

protected:
	std::size_t getIndex(const std::size_t indices[]) const;
	std::array<std::size_t, N> reverseIndex(uint index);
	static std::size_t getIndex(const std::size_t indices[], const std::size_t sizes[]);

	void initializeNestedLists(vcl::NestedInitializerLists<T, N> values);

	std::array<std::size_t, N> sizes;
	std::vector<T>             v;
};

template<typename Scalar>
std::ostream& operator<<(std::ostream& out, const Array<Scalar, 2>& a);

template<typename Scalar>
using Array2 = Array<Scalar, 2>;

template<typename Scalar>
using Array3 = Array<Scalar, 3>;

template<typename Scalar>
using Array4 = Array<Scalar, 4>;

} // namespace vcl

#include "array.cpp"

#endif // VCL_SPACE_ARRAY_H
