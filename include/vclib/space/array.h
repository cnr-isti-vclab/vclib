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

#include <vclib/concepts/space/array.h>
#include <vclib/concepts/range.h>
#include <vclib/misc/nested_initializer_lists.h>
#include <vclib/types.h>

namespace vcl {

/**
 * @brief The Array class is a dynamically allocated N-dimensional array stored
 * in RowWise mode.
 *
 * This class is suitable for storing multi-dimensional data that has a fixed
 * size, as it provides efficient access to its elements. All the elements of
 * the array are stored contiguously in memory, so the access to them using the
 * `()` operator is faster than accessing them through nested vectors.
 *
 * The size of the array is specified at compile time via the template parameter
 * `N`, which indicates the number of dimensions of the array. The size of each
 * dimension can be specified at runtime either on initialization or by calling
 * the `resize` or `conservativeResize` member functions.
 *
 * For one-dimensional arrays, it is recommended to use `std::vector` or
 * `std::array`, as they are more efficient.
 *
 * @tparam T: The type of the elements stored in the array.
 * @tparam N: The number of dimensions of the array.
 *
 * @ingroup space
 */
template<class T, uint N>
class Array
{
	static_assert(N > 0, "Array dimension must be > 0.");
	friend class Array<T, N + 1>;

public:
	/** @brief The type of the elements stored in the array. */
	using ValueType      = std::vector<T>::value_type;

	/** @brief Same of ValueType, just for compatibility with Eigen Matrices */
	using Scalar = ValueType;

	/**
	 * @brief A const reference to the type of the elements stored in the array.
	 */
	using ConstReference = std::vector<T>::const_reference;

	/** @brief A reference to the type of the elements stored in the array. */
	using Reference      = std::vector<T>::reference;

	/**
	 * @brief A const pointer to the type of the elements stored in the array.
	 */
	using ConstPointer   = std::vector<T>::const_pointer;

	/** @brief A pointer to the type of the elements stored in the array. */
	using Pointer        = std::vector<T>::pointer;

	/** @brief An iterator to the elements of the array. */
	using Iterator       = std::vector<T>::iterator;

	/** @brief A const iterator to the elements of the array. */
	using ConstIterator  = std::vector<T>::const_iterator;

	/** @brief The number of dimensions of the array. */
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
	Reference operator()(I... indices) requires (sizeof...(indices) == N);

	template<typename... I>
	ConstReference operator()(I... indices) const
		requires (sizeof...(indices) == N);

	template<typename... I>
	Pointer data(I... indices) requires (sizeof...(indices) < N);

	template<typename... I>
	ConstPointer data(I... indices) const requires (sizeof...(indices) < N);

	std::vector<T>        stdVector() &;
	std::vector<T>&&      stdVector() &&;
	const std::vector<T>& stdVector() const&;

	void fill(const T& t);

	template<Range Rng>
	void fill(Rng&& r);

	template<typename... Sizes>
	void resize(Sizes... s) requires (sizeof...(s) == N);

	template<typename... Sizes>
	void conservativeResize(Sizes... s) requires (sizeof...(s) == N);

	void clear();

	Array<T, N - 1> subArray(uint r) const requires (N > 1);

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

	/// @private
	template<typename S>
	friend std::ostream& operator<<(std::ostream& out, const Array<S, 2>& a);

private:
	std::size_t getIndex(const std::size_t indices[]) const;
	std::array<std::size_t, N> reverseIndex(uint index);
	static std::size_t
	getIndex(const std::size_t indices[], const std::size_t sizes[]);

	void initializeNestedLists(vcl::NestedInitializerLists<T, N> values);

	std::array<std::size_t, N> sizes;
	std::vector<T>             v;
};

template<typename Scalar>
std::ostream& operator<<(std::ostream& out, const Array<Scalar, 2>& a);

/**
 * @brief A convenience alias for a 2-dimensional Array.
 *
 * It is an alias is a shorthand for a Array class template specialization
 * with a scalar type of Scalar and two dimensions.
 *
 * @tparam Scalar: The scalar type of the array components.
 *
 * @ingroup space
 */
template<typename Scalar>
using Array2 = Array<Scalar, 2>;

/**
 * @brief A convenience alias for a 3-dimensional Array.
 *
 * It is an alias is a shorthand for a Array class template specialization with
 * a scalar type of Scalar and three dimensions.
 *
 * @tparam Scalar: The scalar type of the array components.
 *
 * @ingroup space
 */
template<typename Scalar>
using Array3 = Array<Scalar, 3>;

/**
 * @brief A convenience alias for a 4-dimensional Array.
 *
 * It is an alias is a shorthand for a Array class template specialization with
 * a scalar type of Scalar and four dimensions.
 *
 * @tparam Scalar: The scalar type of the array components.
 *
 * @ingroup space
 */
template<typename Scalar>
using Array4 = Array<Scalar, 4>;

} // namespace vcl

#include "array.cpp"

#endif // VCL_SPACE_ARRAY_H
