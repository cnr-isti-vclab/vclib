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

#include <array>
#include <assert.h>
#include <iomanip>
#include <utility>
#include <vector>

#include <vclib/concepts/range.h>
#include <vclib/concepts/space/array.h>
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

    std::array<std::size_t, N> sizes;
    std::vector<T>             v;

public:
    /** @brief The type of the elements stored in the array. */
    using ValueType = std::vector<T>::value_type;

    /** @brief Same of ValueType, just for compatibility with Eigen Matrices */
    using Scalar = ValueType;

    /**
     * @brief A const reference to the type of the elements stored in the array.
     */
    using ConstReference = std::vector<T>::const_reference;

    /** @brief A reference to the type of the elements stored in the array. */
    using Reference = std::vector<T>::reference;

    /**
     * @brief A const pointer to the type of the elements stored in the array.
     */
    using ConstPointer = std::vector<T>::const_pointer;

    /** @brief A pointer to the type of the elements stored in the array. */
    using Pointer = std::vector<T>::pointer;

    /** @brief An iterator to the elements of the array. */
    using Iterator = std::vector<T>::iterator;

    /** @brief A const iterator to the elements of the array. */
    using ConstIterator = std::vector<T>::const_iterator;

    /** @brief The number of dimensions of the array. */
    static constexpr uint DIM = N;

    /**
     * @brief Default constructor for the Array class.
     *
     * Creates an N-dimensional array with size 0 for every dimension.
     */
    Array() : v(0) { sizes.fill(0); }

    /**
     * @brief Constructor for the Array class that creates an N-dimensional
     * array with the given sizes. All its elements are initialized to 0.
     *
     * @tparam Sizes: Variadic parameter pack containing N sizes, one for every
     * dimension of the array.
     *
     * @param[in] s: Variadic parameter pack containing N sizes, one for every
     * dimension of the array.
     *
     * @note The number of arguments must match the number of dimensions of the
     * array.
     */
    template<typename... Sizes>
    Array(Sizes... s) requires (sizeof...(s) == N)
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
     * @brief Creates and initializes an N-dimensional array. Sizes are given by
     * the maximum size of the initializer lists for every dimension, and
     * missing values are automatically set to zero.
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
     * @param[in] values: The nested initializer lists of values.
     *
     * @note The number of levels of the nested initializer lists must
     * correspond to the number of dimensions of the array.
     */
    Array(vcl::NestedInitializerLists<T, N> values)
    {
        initializeNestedLists(values);
    }

    /**
     * @brief Checks whether the array is empty.
     *
     * @return `true` if the array is empty, `false` otherwise.
     */
    bool empty() const { return v.empty(); }

    /**
     * @brief Returns the size of the given dimension.
     *
     * @param[in] dim The dimension to query the size of.
     *
     * @return The size of the given dimension.
     */
    std::size_t size(std::size_t dim) const
    {
        assert(dim < N);
        return sizes[dim];
    }

    /**
     * @brief Returns the number of rows of a 2-dimensional array.
     *
     * @return The number of rows of the array.
     *
     * @note This function can only be called for 2-dimensional arrays.
     */
    std::size_t rows() const requires (N == 2) { return sizes[0]; }

    /**
     * @brief Returns the number of columns of a 2-dimensional array.
     *
     * @return The number of columns of the array.
     *
     * @note This function can only be called for 2-dimensional arrays.
     */
    std::size_t cols() const requires (N == 2) { return sizes[1]; }

    /**
     * @brief Returns the size of the X dimension of the array.
     *
     * @return The size of the X dimension of the array.
     *
     * @note This function can only be called for arrays with at least one
     * dimension.
     */
    std::size_t sizeX() const requires (N >= 1) { return sizes[0]; }

    /**
     * @brief Returns the size of the Y dimension of the array.
     *
     * @return The size of the Y dimension of the array.
     *
     * @note This function can only be called for arrays with at least two
     * dimensions.
     */
    std::size_t sizeY() const requires (N >= 2) { return sizes[1]; }

    /**
     * @brief Returns the size of the Z dimension of the array.
     *
     * @return The size of the Z dimension of the array.
     *
     * @note This function can only be called for arrays with at least three
     * dimensions.
     */
    std::size_t sizeZ() const requires (N >= 3) { return sizes[2]; }

    /**
     * @brief Returns the size of the W dimension of the array.
     *
     * @return The size of the W dimension of the array.
     *
     * @note This function can only be called for arrays with at least four
     * dimensions.
     */
    std::size_t sizeW() const requires (N >= 4) { return sizes[3]; }

    /**
     * @brief Operator () that allows to access one element of the array. It can
     * be used as left or right value.
     *
     * @tparam I: Types of the indices used to access the element of the array.
     *
     * @param[in] indices: N indices that allow to access an element of the
     * array. A number of indices not equal to N will generate a compilation
     * error.
     *
     * @return A reference to the element of the array.
     */
    template<typename... I>
    Reference operator()(I... indices) requires (sizeof...(indices) == N)
    {
        std::size_t args[N] = {static_cast<std::size_t>(indices)...};
        return v[getIndex(args)];
    }

    /**
     * @brief Operator () that allows to access one element of the array. It can
     * be used only as right value.
     *
     * @tparam I: Types of the indices used to access the element of the array.
     *
     * @param[in] indices: N indices that allow to access an element of the
     * array. A number of indices not equal to N will generate a compilation
     * error.
     *
     * @return A const reference to the element of the array.
     */
    template<typename... I>
    ConstReference operator()(I... indices) const
        requires (sizeof...(indices) == N)
    {
        std::size_t args[N] = {static_cast<std::size_t>(indices)...};
        return v[getIndex(args)];
    }

    /**
     * @brief Allows to get the data of the Array, through a pointer to the
     * first element.
     *
     * The function also allows to get the pointer of a specific position in the
     * array.
     *
     * Example:
     * @code{.cpp}
     * Array<int, 3> array(10, 13, 4);
     * //...
     * int* carray = array.data(3); // carray will point to the element in
     *                              // position (3, 0, 0).
     *
     * carray = array.data(5, 2); // carray will point to the element in
     *                            // position (5, 2, 0).
     *
     * carray = array.data(); // carray will point to the element in position
     *                        // (0, 0, 0).
     * @endcode
     *
     * @tparam I: Types of the indices used to access a subarray of the array.
     *
     * @param[in] indices: A number of indices that is less than the number of
     * dimensions of the array.
     *
     * @return A pointer to the requested subarray.
     */
    template<typename... I>
    Pointer data(I... indices) requires (sizeof...(indices) < N)
    {
        return const_cast<Pointer>(std::as_const(*this).data(indices...));
    }

    /**
     * @brief Allows to get the data of the Array, through a pointer to the
     * first element.
     *
     * The function also allows to get the pointer of a specific position in the
     * array.
     *
     * Example:
     * @code{.cpp}
     * Array<int, 3> array(10, 13, 4);
     * //...
     * const int* carray = array.data(3); // carray will point to the element in
     *                                    // position (3, 0, 0).
     *
     * carray = array.data(5, 2); // carray will point to the element in
     *                            // position (5, 2, 0).
     *
     * carray = array.data(); // carray will point to the element in position
     *                        // (0, 0, 0).
     * @endcode
     *
     * @tparam I: Types of the indices used to access a subarray of the array.
     *
     * @param[in] indices: A number of indices that is less than the number of
     * dimensions of the array.
     *
     * @return A const pointer to the requested subarray.
     */
    template<typename... I>
    ConstPointer data(I... indices) const requires (sizeof...(indices) < N)
    {
        constexpr std::size_t n = sizeof...(indices);
        if constexpr (n == 0) {
            return v.data();
        }
        else {
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
    }

    /**
     * @brief Returns a std::vector containing the elements of the array in
     * row-major order.
     *
     * @return A std::vector containing the elements of the array in row-major
     * order.
     */
    std::vector<T> stdVector() & { return v; }

    /**
     * @brief Returns a std::vector containing the elements of the array in
     * row-major order.
     *
     * @return A std::vector containing the elements of the array in row-major
     * order.
     */
    std::vector<T>&& stdVector() && { return std::move(v); }

    /**
     * @brief Returns a std::vector containing the elements of the array in
     * row-major order.
     *
     * @return A const reference to a std::vector containing the elements of the
     * array in row-major order.
     */
    const std::vector<T>& stdVector() const& { return v; }

    /**
     * @brief Fills the entire Array with the value t.
     *
     * @param[in] t: The value to fill the array with.
     */
    void fill(const T& t) { std::fill(v.begin(), v.end(), t); }

    /**
     * @brief Fills the entire Array with the values contained in the range r,
     * in row-major order.
     *
     * If the size of the container is greater than the total size of the array,
     * the remaining elements of the container will be ignored. If the size of
     * the container is less than the total size of the array, the remaining
     * values in the array will be left unchanged.
     *
     * @tparam Rng: Type of the range of values to fill the array with. It must
     * satisfy the Range concept.
     *
     * @param[in] r: A range of the same type as the array. The range must have
     * `begin()` and `end()` members provided.
     */
    template<Range Rng>
    void fill(Rng&& r)
    {
        uint i = 0;
        for (auto it = std::ranges::begin(r);
             it != std::ranges::end(r) && i < v.size();
             ++i, ++it)
        {
            v[i] = *it;
        }
    }

    /**
     * @brief Allows to resize the Array, not conserving the values of the
     * previous array.
     *
     * @tparam Sizes: Types of the arguments representing the new sizes of the
     * Array.
     *
     * @param[in] s: N elements representing the new sizes of the Array.
     */
    template<typename... Sizes>
    void resize(Sizes... s) requires (sizeof...(s) == N)
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
     * @brief Allows to resize the Array, conserving the values of the previous
     * array.
     *
     * The new array will have the specified sizes if possible, but if any
     * dimension is smaller than the previous size, then the values in those
     * dimensions will be conserved.
     *
     * @tparam Sizes: Types of the arguments representing the new sizes of the
     * Array.
     *
     * @param[in] s: N elements representing the new sizes of the Array.
     */
    template<typename... Sizes>
    void conservativeResize(Sizes... s) requires (sizeof...(s) == N)
    {
        std::size_t newSizes[N]  = {static_cast<std::size_t>(s)...};
        std::size_t newTotalSize = 1;
        for (uint i = 0; i < N; i++)
            newTotalSize *= newSizes[i];
        std::vector<T> newVector(newTotalSize);

        for (std::size_t i = 0; i < v.size(); i++) {
            std::array<std::size_t, N> indices    = reverseIndex(i);
            bool                       outOfBound = false;
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
    void clear()
    {
        v.clear();
        for (uint i = 0; i < N; i++)
            sizes[i] = 0;
    }

    /**
     * @brief Creates a new subArray of dimension N-1, starting from the given
     * index at its first dimension.
     *
     * The new subArray has the same type as the original array, but one
     * dimension less, and contains the elements of the original array starting
     * from the given index at its first dimension.
     *
     * Example:
     * @code{.cpp}
     * Array<int, 3> a(4,2,6);
     * Array<int, 2> sa = a.subArray(1);
     * // sa is a 2x6 2D Array, containing the elements at the second "row" of
     * // Array a.
     * @endcode
     *
     * @param r: Index at the first dimension to start the sub-array.
     *
     * @return The sub-array of dimension N-1.
     *
     * @pre N > 1
     * @pre r < size(0)
     */
    Array<T, N - 1> subArray(uint r) const requires (N > 1)
    {
        assert(r < sizes[0]);
        Array<T, N - 1> sub;
        size_t          size = 1;
        for (uint i = 0; i < sizes.size() - 1; i++) {
            sub.sizes[i] = sizes[i + 1];
            size *= sub.sizes[i];
        }
        sub.v =
            std::vector<T>(v.begin() + r * size, v.begin() + (r + 1) * size);
        return sub;
    }

    /**
     * @brief Returns an iterator to the beginning of the array.
     *
     * @return Iterator to the beginning of the array.
     */
    Iterator begin() { return v.begin(); }

    /**
     * @brief Returns an iterator to the end of the array.
     *
     * @return Iterator to the end of the array.
     */
    Iterator end() { return v.end(); }

    /**
     * @brief Returns a const iterator to the beginning of the array.
     *
     * @return Const iterator to the beginning of the array.
     */
    ConstIterator begin() const { return v.begin(); }

    /**
     * @brief Returns a const iterator to the end of the array.
     *
     * @return Const iterator to the end of the array.
     */
    ConstIterator end() const { return v.end(); }

    /// @private
    template<typename S>
    friend std::ostream& operator<<(std::ostream& out, const Array<S, 2>& a);

private:
    std::size_t getIndex(const std::size_t indices[]) const
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

    std::array<std::size_t, N> reverseIndex(uint index)
    {
        std::array<std::size_t, N> indices;
        for (long int i = N - 1; i >= 0; i--) {
            indices[i] = index % sizes[i];
            index /= sizes[i];
        }
        return indices;
    }

    static std::size_t getIndex(
        const std::size_t indices[],
        const std::size_t sizes[])
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

    void initializeNestedLists(vcl::NestedInitializerLists<T, N> values)
    {
        std::list<std::size_t> szs =
            NestedInitializerListsProcessor<T, N>::maxDimensionsLevels(values);

        uint   i         = 0;
        size_t totalSize = 1;
        for (std::size_t s : szs) {
            totalSize *= s;
            sizes[i++] = s;
        }
        v.resize(totalSize);

        typename std::vector<T>::iterator iterator = v.begin();
        NestedInitializerListsProcessor<T, N>::processElements(
            values,
            [&iterator](T value) {
                *(iterator++) = value;
            },
            szs);
    }
};

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

/* Specialization Aliases */

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

#endif // VCL_SPACE_ARRAY_H
