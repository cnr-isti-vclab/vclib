/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SPACE_CORE_VECTOR_VECTOR_H
#define VCL_SPACE_CORE_VECTOR_VECTOR_H

#include <vclib/base.h>
#include <vclib/exceptions.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <utility>
#include <vector>

namespace vcl {

/**
 * @brief The Vector class is a generic container of objects of type `T`, that
 * could have fixed or dynamic size, depending on the templated size `N`.
 *
 * The Vector class is a container that can hold objects of type `T`. The size
 * of the container can be either fixed or dynamic, depending on the value of
 * the template parameter `N`. If `N` is greater than or equal to zero, the
 * container will have a fixed size of N elements, and it will use an array to
 * store the elements. If `N` is less than zero, the container will have a
 * dynamic size, and it will use a vector to store the elements. The Vector
 * class provides several member functions to manipulate the elements of the
 * container, such as getting and setting individual elements, filling the
 * container with a value, checking if the container contains a certain element,
 * and finding the index of a certain element.
 *
 * @tparam T: the type of the objects stored in the container.
 * @tparam N: the size of the container. If `N` is greater than or equal to
 * zero, the container will have a fixed size of `N` elements, and it will use
 * an array to store the elements. If `N` is less than zero, the container will
 * have a dynamic size, and it will use a vector to store the elements.
 *
 * @ingroup space_core
 */
template<typename T, int N>
class Vector
{
    // if we use the vector, the size of the array will be 0
    // actually the array will never be used and will not use memory, it's just
    // for declaration
    static const int ARRAY_SIZE = N >= 0 ? N : 0;

    // the Container type will be array or vector, depending on N value
    using Container = std::conditional_t<
        (N >= 0),
        typename std::array<T, ARRAY_SIZE>,
        typename std::vector<T>>;

protected:
    Container mContainer;

public:
    /** @brief The type of the elements stored in the Vector. */
    using ValueType = Container::value_type;

    /**
     * @brief A const reference to the type of the elements stored in the
     * Vector.
     */
    using ConstReference = Container::const_reference;

    /** @brief A reference to the type of the elements stored in the Vector. */
    using Reference = Container::reference;

    /**
     * @brief A const pointer to the type of the elements stored in the Vector.
     */
    using ConstPointer = Container::const_pointer;

    /** @brief A pointer to the type of the elements stored in the Vector. */
    using Pointer = Container::pointer;

    /** @brief An iterator to the elements of the Vector. */
    using Iterator = Container::iterator;

    /** @brief A const iterator to the elements of the Vector. */
    using ConstIterator = Container::const_iterator;

    /**
     * @brief Size of the vector at compile time. It will be -1 if the Vector
     * has dynamic size.
     */
    static const int SIZE = N;

    /**
     * @brief Creates an empty Vector object.
     *
     * If the container is dynamic, its size is 0. When the container is static,
     * the size is N and its elements are initialized with their empty
     * constructor.
     */
    Vector() = default;

    /**
     * @brief Creates a Vector object with the specified size.
     *
     * If the container is dynamic, its size is set to `size`.
     * When the container is static, the given size must be equal to N.
     * The elements are initialized with their empty constructor if the argument
     * `value` is not specified. Otherwise, the elements are initialized with
     * the given value.
     *
     * @throws WrongSizeException if the given size is not equal to N and the
     * container is static.
     *
     * @param[in] size: the size of the container.
     * @param[in] value: the value to initialize the elements with.
     */
    Vector(std::size_t size, const T& value = T())
    {
        if constexpr (N >= 0) {
            if (size != N) {
                throw WrongSizeException(
                    "Vector must have " + std::to_string(N) + " size.");
            }
            fill(value);
        }
        else {
            mContainer.resize(size, value);
        }
    }

    /**
     * @brief Constructs the container with the contents of the range [first,
     * last).
     *
     * If the container is dynamic, the size of the container is determined by
     * the number of elements in the range [first, last). If the container is
     * static, the elements are initialized with the contents of the first N
     * elements of the range [first, last). If the range contains less than N
     * elements, the remaining elements are initialized with their empty
     * constructor.
     *
     * @tparam ItType: The type of the iterators used to access the elements in
     * the range.
     *
     * @param[in] first: The beginning of the range.
     * @param[in] last: The end of the range.
     */
    template<typename ItType>
    Vector(ItType first, ItType last)
    {
        set(View(first, last));
    }

    /**
     * @brief Constructs the container with the contents of the range rng.
     *
     * If the container is dynamic, the size of the container is determined by
     * the number of elements in the input range. If the container is static,
     * the elements are initialized with the contents of the first N elements of
     * the range. If the range contains less than N elements, the remaining
     * elements are initialized with their empty constructor.
     *
     * @tparam RangeType: The type of the range used to access the elements in
     * the range. It must satisfy the vcl::Range concept.
     *
     * @param[in] rng: a range of T elements.
     */
    template<Range RangeType>
    Vector(RangeType&& rng)
    {
        set(rng);
    }

    /**
     * @brief Constructs the container with the contents of the initializer list
     * `list`.
     *
     * If the container is dynamic, the size of the container is determined by
     * the number of elements in the initializer list `list`. If the container
     * is static, the elements are initialized with the contents of the first N
     * elements of the initializer list `list`. If the initializer list contains
     * less than N elements, the remaining elements are initialized with their
     * empty constructor.
     *
     * @param[in] list: The initializer list to initialize the elements of the
     * container with.
     */
    explicit Vector(std::initializer_list<T> list)
    {
        set(View(std::begin(list), std::end(list)));
    }

    /**
     * @brief Returns the size of the container.
     *
     * If the container is static, the size is N. If the container is dynamic,
     * the size is determined by the number of elements currently stored.
     *
     * @return The size of the container.
     */
    std::size_t size() const
    {
        if constexpr (N >= 0) {
            return N;
        }
        else {
            return mContainer.size();
        }
    }

    /**
     * @brief Access the specified element with bounds checking.
     *
     * Returns a reference to the element at position `i` in the Vector, with
     * bounds checking. If `i` is not within the range of valid indices for the
     * Vector, an `std::out_of_range` exception is thrown.
     *
     * @param[in] i: The position of the element.
     * @return A reference to the element at position `i`.
     *
     * @throws std::out_of_range If `i` is not within the range of valid indices
     * for the Vector.
     */
    Reference at(uint i) { return mContainer.at(i); }

    /**
     * @brief Access the specified element with bounds checking.
     *
     * Returns a const reference to the element at position `i` in the Vector,
     * with bounds checking. If `i` is not within the range of valid indices for
     * the Vector, an `std::out_of_range` exception is thrown.
     *
     * @param[in] i: The position of the element.
     * @return A const reference to the element at position `i`.
     *
     * @throws std::out_of_range If `i` is not within the range of valid indices
     * for the Vector.
     */
    ConstReference at(uint i) const { return mContainer.at(i); }

    /**
     * @brief Access the specified element, computing first the module of the
     * position w.r.t. the size of the container.
     *
     * Takes into account negative indices: atMod(-1) will access to the last
     * element of the container.
     *
     * @param[in] i: The position of the element.
     * @return A reference to the element at position i % size().
     */
    Reference atMod(int i)
    {
        int n = size(); // need to save n as int to avoid unwanted casts
        return mContainer[(i % n + n) % n];
    }

    /**
     * @brief Access the specified element, computing first the module of the
     * position w.r.t. the size of the container.
     *
     * Takes into account negative indices: atMod(-1) will access to the last
     * element of the container.
     *
     * @param[in] i The position of the element.
     * @return A const reference to the element at position i % size().
     */
    ConstReference atMod(int i) const
    {
        int n = size(); // need to save n as int to avoid unwanted casts
        return mContainer[(i % n + n) % n];
    }

    /**
     * @brief Access the first element of the Vector.
     *
     * Returns a reference to the first element of the Vector. If  the Vector is
     * empty, the behavior is undefined.
     *
     * @return A reference to the first element.
     */
    Reference front() { return mContainer.front(); }

    /**
     * @brief Access the first element of the Vector.
     *
     * Returns a const reference to the first element of the Vector. If the
     * Vector is empty, the behavior is undefined.
     *
     * @return A const reference to the first element.
     */
    ConstReference front() const { return mContainer.front(); }

    /**
     * @brief Access the last element of the Vector.
     *
     * Returns a reference to the last element of the Vector. If the Vector is
     * empty, the behavior is undefined.
     *
     * @return A reference to the last element.
     */
    Reference back() { return mContainer.back(); }

    /**
     * @brief Access the last element of the Vector.
     *
     * Returns a const reference to the last element of the Vector. If the
     * Vector is empty, the behavior is undefined.
     *
     * @return A const reference to the last element.
     */
    ConstReference back() const { return mContainer.back(); }

    /**
     * @brief Returns a pointer to the underlying array serving as element
     * storage. The pointer is such that range [data(), data() + size()) is
     * always a valid range, even if the container is empty (data() is not
     * dereferenceable in that case).
     *
     * @return A pointer to the underlying element storage. For non-empty
     * containers, the returned pointer compares equal to the address of the
     * first element.
     */
    Pointer data() { return mContainer.data(); }

    /**
     * @brief Returns a const pointer to the underlying array serving as element
     * storage. The pointer is such that range [data(), data() + size()) is
     * always a valid range, even if the container is empty (data() is not
     * dereferenceable in that case).
     *
     * @return A const pointer to the underlying element storage. For non-empty
     * containers, the returned pointer compares equal to the address of the
     * first element.
     */
    ConstPointer data() const { return mContainer.data(); }

    /**
     * @brief Set the value of the element at the specified position.
     *
     * Sets the value of the element at position `i` in the Vector to the
     * specified value `e`.
     *
     * @param[in] i: The position of the element.
     * @param[in] e: The new value of the element.
     */
    void set(uint i, const T& e)
    {
        assert(i < size());
        mContainer[i] = e;
    }

    /**
     * @brief Set the value of the element at the specified position.
     *
     * Sets the value of the element at position `i` in the Vector by moving the
     * specified value `e`.
     *
     * @param[in] i: The position of the element.
     * @param[in] e: The new value of the element.
     */
    void set(uint i, T&& e)
    {
        assert(i < size());
        mContainer[i] = std::move(e);
    }

    /**
     * @brief Set the value of the element at the specified position.
     *
     * Sets the value of the element at position `it` in the Vector to the
     * specified value `e`.
     *
     * @param[in] it: The iterator pointing to the position of the element.
     * @param[in] e: The new value of the element.
     */
    void set(ConstIterator it, const T& e)
    {
        assert(it < end());
        mContainer[it - begin()] = e;
    }

    /**
     * @brief Set the value of the element at the specified position.
     *
     * Sets the value of the element at position `it` in the Vector by moving
     * the specified value `e`.
     *
     * @param[in] it: The iterator pointing to the position of the element.
     * @param[in] e: The new value of the element.
     */
    void set(ConstIterator it, T&& e)
    {
        assert(it < end());
        mContainer[it - begin()] = std::move(e);
    }

    /**
     * @brief Set the elements of the Vector using the values from a range.
     *
     * Sets the elements of the Vector to the values from the range `r`. If the
     * size of the Vector is known at compile-time and is not negative, the
     * first N elements from the range `r` (or all of them, if they are less
     * than N) are copied into the Vector using `std::copy_n()`. If the size of
     * the Vector is negative or not known at compile-time, the elements of the
     * Vector are set to the values from the range `r` by constructing a new
     * std::vector from the range `r`.
     *
     * @tparam Rng: The type of the range, it must satisfy the Range concept and
     * the value type of the range must be convertible to T.
     *
     * @param[in] r: The range of values to set the elements of the Vector to.
     */
    template<Range Rng>
    void set(Rng&& r) requires InputRange<Rng, T>
    {
        if constexpr (N >= 0) {
            uint n = std::min(
                N,
                (int) std::distance(
                    std::ranges::begin(r), std::ranges::end(r)));
            std::copy_n(std::ranges::begin(r), n, mContainer.begin());
        }
        else {
            mContainer =
                std::vector<T>(std::ranges::begin(r), std::ranges::end(r));
        }
    }

    /**
     * @brief Fill all elements of the Vector with the specified value.
     *
     * Sets all elements of the Vector to the value specified by `e`, using the
     * `std::fill()` algorithm.
     *
     * @param[in] e: The value to fill the Vector with.
     */
    void fill(const T& e)
    {
        std::fill(mContainer.begin(), mContainer.end(), e);
    }

    /**
     * @brief Check if the Vector contains the specified element.
     *
     * Checks if the Vector contains the element specified by `e`, using the
     * `std::find()` algorithm.
     *
     * @param[in] e: The element to search for in the Vector.
     * @return `true` if the element is found in the Vector, `false` otherwise.
     */
    bool contains(const MakeConstPointerT<T>& e) const
    {
        return std::find(mContainer.begin(), mContainer.end(), e) !=
               mContainer.end();
    }

    /**
     * @brief Find the first occurrence of the specified element in the Vector.
     *
     * Finds the first occurrence of the element specified by `e` in the Vector,
     * using the `std::find()` algorithm.
     *
     * @param[in] e: The element to search for in the Vector.
     * @return An iterator to the first occurrence of the element in the Vector,
     * or `end()` if the element is not found.
     */
    Iterator find(const MakeConstPointerT<T>& e)
    {
        return std::find(mContainer.begin(), mContainer.end(), e);
    }

    /**
     * @brief Find the first occurrence of the specified element in the Vector.
     *
     * Finds the first occurrence of the element specified by `e` in the Vector,
     * using the `std::find()` algorithm.
     *
     * @param[in] e: The element to search for in the Vector.
     * @return A const iterator to the first occurrence of the element in the
     * Vector, or `end()` if the element is not found.
     */
    ConstIterator find(const MakeConstPointerT<T>& e) const
    {
        return std::find(mContainer.begin(), mContainer.end(), e);
    }

    /**
     * @brief Get the index of the first occurrence of the specified element in
     * the Vector.
     *
     * Finds the first occurrence of the element specified by `e` in the Vector,
     * using the `std::find()` algorithm, and returns its index. If the element
     * is not found in the Vector, the member function returns UINT_NULL.
     *
     * @param[in] e: The element to search for in the Vector.
     * @return The index of the first occurrence of the element in the Vector,
     * or UINT_NULL if the element is not found.
     */
    uint indexOf(const MakeConstPointerT<T>& e) const
    {
        auto it = find(e);
        if (it == end())
            return UINT_NULL;
        else
            return it - begin();
    }

    /**
     * @brief Swaps the contents of the container with those of other.
     * @param[in] other: Another Vector container of the same type.
     */
    void swap(Vector& other) { mContainer.swap(other.mContainer); }

    /**
     * Specializes the swap function to allow the swapping of two Vector
     * objects.
     *
     * Swaps the content of the two Vector objects. Calls `a.swap(b)`.
     * @param[in] a: The first Vector object.
     * @param[in] b: The second Vector object.
     */
    friend void swap(Vector& a, Vector& b) { a.swap(b); }

    /* Member functions specific for dynamic vector */

    /**
     * @brief Resize the Vector to the specified size.
     *
     * Resizes the Vector to the specified size `n` by resizing the underlying
     * `std::vector`. This member function is only available if the size of the
     * Vector is not known at compile-time, as specified by the concept
     * requirement `requires (N < 0)`.
     *
     * @param[in] n: The new size of the Vector.
     * @param[in] v: The value to use to fill the new elements of the Vector.
     */
    void resize(uint n, const T& v = T()) requires (N < 0)
    {
        mContainer.resize(n, v);
    }

    /**
     * @brief Add an element to the end of the Vector.
     *
     * Adds the element `v` to the end of the Vector by calling the
     * `push_back()` member function of the underlying `std::vector`. This
     * member function is only available if the size of the Vector is not known
     * at compile-time, as specified by the concept requirement `requires (N <
     * 0)`.
     *
     * @param[in] v: The value to add to the end of the Vector.
     */
    void pushBack(const T& v) requires (N < 0) { mContainer.push_back(v); }

    /**
     * @brief Add an element to the end of the Vector.
     *
     * Moves the element `v` to the end of the Vector by calling the
     * `push_back()` member function of the underlying `std::vector`. This
     * member function is only available if the size of the Vector is not known
     * at compile-time, as specified by the concept requirement `requires (N <
     * 0)`.
     *
     * @param[in] v: The value to add to the end of the Vector.
     */
    void pushBack(T&& v) requires (N < 0)
    {
        mContainer.push_back(std::move(v));
    }

    /**
     * @brief Insert an element at the specified position in the Vector.
     *
     * Inserts the element `v` at the position specified by `i` in the Vector by
     * calling the `insert()` member function of the underlying `std::vector`.
     * This member function is only available if the size of the Vector is not
     * known at compile-time, as specified by the concept requirement `requires
     * (N < 0)`.
     *
     * @param[in] i: The index at which to insert the element.
     * @param[in] v: The value to insert into the Vector.
     */
    void insert(uint i, const T& v) requires (N < 0)
    {
        assert(i < size() + 1);
        mContainer.insert(mContainer.begin() + i, v);
    }

    /**
     * @brief Insert an element at the specified position in the Vector.
     *
     * Inserts the element `v` at the position specified by `i` in the Vector by
     * calling the `insert()` member function of the underlying `std::vector`.
     * This member function is only available if the size of the Vector is not
     * known at compile-time, as specified by the concept requirement `requires
     * (N < 0)`.
     *
     * @param[in] i: The index at which to insert the element.
     * @param[in] v: The value to insert into the Vector.
     */
    void insert(uint i, T&& v) requires (N < 0)
    {
        assert(i < size() + 1);
        mContainer.insert(mContainer.begin() + i, std::move(v));
    }

    /**
     * @brief Insert an element at the specified position in the Vector.
     *
     * Inserts the newly constructed element with the args at the position
     * specified by `i` in the Vector by calling the `emplace()` member function
     * of the underlying `std::vector`. This member function is only available
     * if the size of the Vector is not known at compile-time, as specified by
     * the concept requirement `requires (N < 0)`.
     *
     * @param[in] i: The index at which to insert the element.
     * @param[in] args: Arguments to forward to the constructor of the element.
     */
    template<typename... Args>
    void emplace(uint i, Args&&... args) requires (N < 0)
    {
        assert(i < size() + 1);
        mContainer.emplace(mContainer.begin() + i, std::forward<Args>(args)...);
    }

    /**
     * @brief Returns whether the vector is empty (i.e. whether its size is 0).
     *
     * @return `true` if the container size is 0, `false` otherwise.
     */
    bool empty() const noexcept { return mContainer.empty(); }

    /**
     * @brief Remove the element at the specified index from the Vector.
     *
     * Removes the element at the position specified by `i` in the Vector by
     * calling the `erase()` member function of the underlying `std::vector`.
     * This member function is only available if the size of the Vector is not
     * known at compile-time, as specified by the concept requirement `requires
     * (N < 0)`.
     *
     * @param[in] i: The index of the element to remove from the Vector.
     */
    void erase(uint i) requires (N < 0)
    {
        assert(i < size());
        mContainer.erase(mContainer.begin() + i);
    }

    /**
     * @brief Remove all elements from the Vector.
     *
     * Removes all elements from the Vector by calling the `clear()` member
     * function of the underlying `std::vector`. This member function is only
     * available if the size of the Vector is not known at compile-time, as
     * specified by the concept requirement `requires (N < 0)`.
     */
    void clear() requires (N < 0) { mContainer.clear(); }

    void serialize(std::ostream& os) const { vcl::serialize(os, mContainer); }

    void deserialize(std::istream& is) { vcl::deserialize(is, mContainer); }

    /* Operators */

    /**
     * @brief Returns a reference to the element at specified location i. No
     * bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A reference to the requested element.
     */
    Reference operator[](uint i) { return mContainer[i]; }

    /**
     * @brief Returns a const reference to the element at specified location i.
     * No bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A const reference to the requested element.
     */
    ConstReference operator[](uint i) const { return mContainer[i]; }

    /**
     * @brief Returns a reference to the element at specified location i. No
     * bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A reference to the requested element.
     */
    Reference operator()(uint i) { return mContainer[i]; }

    /**
     * @brief Returns a const reference to the element at specified location i.
     * No bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A const reference to the requested element.
     */
    ConstReference operator()(uint i) const { return mContainer[i]; }

    /* Iterator member functions */

    /**
     * @brief Return an iterator pointing to the beginning of the Vector.
     *
     * @return An iterator pointing to the beginning of the Vector.
     */
    Iterator begin() { return mContainer.begin(); }

    /**
     * @brief Return an iterator pointing to the end of the Vector.
     *
     * @return An iterator pointing to the end of the Vector.
     */
    Iterator end() { return mContainer.end(); }

    /**
     * @brief Return a const iterator pointing to the beginning of the Vector.
     *
     * @return A const iterator pointing to the beginning of the Vector.
     */
    ConstIterator begin() const { return mContainer.begin(); }

    /**
     * @brief Return a const iterator pointing to the end of the Vector.
     *
     * @return A const iterator pointing to the end of the Vector.
     */
    ConstIterator end() const { return mContainer.end(); }
};

} // namespace vcl

#endif // VCL_SPACE_CORE_VECTOR_VECTOR_H
