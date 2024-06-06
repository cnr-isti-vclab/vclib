/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_SPACE_POLYMORPHIC_OBJECT_VECTOR_H
#define VCL_SPACE_POLYMORPHIC_OBJECT_VECTOR_H

#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <vector>

#include <vclib/concepts/polymorphism.h>
#include <vclib/concepts/ranges/range.h>
#include <vclib/exceptions.h>
#include <vclib/iterators/const_pointer_iterator.h>
#include <vclib/types.h>
#include <vclib/views/view.h>

namespace vcl {

/**
 * @brief The PolymorphicObjectVector class is a container that stores a
 * collection of polymorphic objects, having a common base class T.
 *
 * The PolymorphicObjectVector class is a container that stores a collection of
 * polymorphic objects, having a common base class T. The class is designed to
 * work with objects that are clonable, i.e., that implement the Clonable
 * concept (they must implement a member function `clone()` that returns a
 * shared pointer that points to a newly created object).
 *
 * The container stores the objects as shared pointers to the base class.
 * All the member functions of this class that take a value of type T as
 * argument will clone the object before storing it in the container. This way,
 * the container will own a copy of the object, and the original object will
 * not be modified (as it is made normally on std::vector). To avoid cloning,
 * you can pass a shared pointer to the object.
 *
 * The size of the container can be either fixed or dynamic, depending
 * on the value of the template parameter `N`. If `N` is greater than or equal
 * to zero, the container will have a fixed size of N elements, and it will use
 * an array to store the elements. If `N` is less than zero, the container will
 * have a dynamic size, and it will use a vector to store the elements.
 *
 * @tparam T: The base class of the polymorphic objects stored in the container.
 * The class T must implement the Clonable concept.
 * @tparam N: the size of the container. If `N` is greater than or equal to
 * zero, the container will have a fixed size of `N` elements, and it will use
 * an array to store the elements. If `N` is less than zero, the container will
 * have a dynamic size, and it will use a vector to store the elements.
 *
 * @ingroup space
 */
template<Cloneable T, int N = -1>
class PolymorphicObjectVector
{
    // if we use the vector, the size of the array will be 0
    // actually the array will never be used and will not use memory, it's just
    // for declaration
    static const int ARRAY_SIZE = N >= 0 ? N : 0;

    // the Container type will be array or vector, depending on N value
    using Container = std::conditional_t<
        (N >= 0),
        typename std::array<std::shared_ptr<T>, ARRAY_SIZE>,
        typename std::vector<std::shared_ptr<T>>>;

protected:
    Container mContainer;

public:
    /** @brief The type of the elements stored in the Vector. */
    using ValueType = Container::value_type;

    using ConstValueType = vcl::MakeConstPointerT<ValueType>;

    /** @brief An iterator to the elements of the Vector. */
    using Iterator = Container::const_iterator;

    /** @brief A const iterator to the elements of the Vector. */
    using ConstIterator =
        ConstPointerIterator<typename Container::const_iterator>;

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
    PolymorphicObjectVector() = default;

    /**
     * @brief Creates a Vector object with the specified size.
     *
     * If the container is dynamic, its size is set to `size`.
     * When the container is static, the given size must be equal to N.
     * The elements are initialized to nullptr.
     *
     * @throws WrongSizeException if the given size is not equal to N and the
     * container is static.
     *
     * @param[in] size: the size of the container.
     */
    PolymorphicObjectVector(std::size_t size)
    {
        if constexpr (N < 0) {
            mContainer.resize(size);
        }
    }

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
    PolymorphicObjectVector(std::size_t size, const T& value)
    {
        if constexpr (N >= 0) {
            if (size != N) {
                throw WrongSizeException(
                    "Vector must have " + std::to_string(N) + " size.");
            }
            fill(value);
        }
        else {
            resize(size, value);
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
    PolymorphicObjectVector(ItType first, ItType last)
    {
        set(vcl::View(first, last));
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
    PolymorphicObjectVector(RangeType&& rng)
    {
        set(rng);
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
    ValueType at(uint i) { return mContainer.at(i); }

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
    ConstValueType at(uint i) const { return mContainer.at(i); }

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
    ValueType atMod(int i)
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
    ConstValueType atMod(int i) const
    {
        int n = size(); // need to save n as int to avoid unwanted casts
        return mContainer[(i % n + n) % n];
    }

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
        mContainer[i] = e.clone();
    }

    void set(uint i, const std::shared_ptr<T>& e)
    {
        assert(i < size());
        mContainer[i] = e;
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
        mContainer[it - begin()] = e.clone();
    }

    void set(ConstIterator it, const std::shared_ptr<T>& e)
    {
        assert(it < end());
        mContainer[it - begin()] = e;
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
    void set(Rng&& r) requires RangeOfConvertibleTo<Rng, T>
    {
        if constexpr (N >= 0) {
            uint n = std::min(
                N,
                (int) std::distance(
                    std::ranges::begin(r), std::ranges::end(r)));

            // for each element in the range, clone it and store it in the
            // vector
            std::transform(
                std::ranges::begin(r),
                std::ranges::begin(r) + n,
                mContainer.begin(),
                [](const auto& e) {
                    return e.clone();
                });
        }
        else {
            // create a new vector from the range, containing clones
            mContainer = std::vector<std::shared_ptr<T>>(
                std::ranges::begin(r), std::ranges::end(r), [](const auto& e) {
                    return e.clone();
                });
        }
    }

    /**
     * @brief Fill all elements of the Vector with clones of the specified
     * value.
     *
     * Sets all elements of the Vector to clones of the value specified by `e`,
     * using the `std::fill()` algorithm.
     *
     * @param[in] e: The value to fill the Vector with.
     */
    void fill(const T& e)
    {
        std::fill(mContainer.begin(), mContainer.end(), [e]() {
            return e.clone();
        });
    }

    /* Member functions specific for dynamic vector */

    /**
     * @brief Resize the Vector to the specified size.
     *
     * Resizes the Vector to the specified size `n` by resizing the underlying
     * `std::vector`. This member function is only available if the size of the
     * Vector is not known at compile-time, as specified by the concept
     * requirement `requires (N < 0)`.
     *
     * If the new size is greater than the current size, the new elements are
     * initialized with nullptr.
     *
     * @param[in] n: The new size of the Vector.
     */
    void resize(uint n) requires (N < 0)
    {
        // resize the vector, cloning the value of v on each new element
        mContainer.resize(n);
    }

    /**
     * @brief Resize the Vector to the specified size.
     *
     * Resizes the Vector to the specified size `n` by resizing the underlying
     * `std::vector`. This member function is only available if the size of the
     * Vector is not known at compile-time, as specified by the concept
     * requirement `requires (N < 0)`.
     *
     * If the new size is greater than the current size, the new elements are
     * initialized with a clone of the value `v`.
     *
     * @param[in] n: The new size of the Vector.
     * @param[in] v: The value to use to fill the new elements of the Vector.
     */
    void resize(uint n, const T& v) requires (N < 0)
    {
        if (n > size()) {
            uint oldSize = size();
            mContainer.resize(n);
            std::fill(mContainer.begin() + oldSize, mContainer.end(), [v]() {
                return v.clone();
            });
        }
        else {
            mContainer.resize(n);
        }
    }

    /**
     * @brief Add an element to the end of the Vector.
     *
     * Adds the clone of the  element `v` to the end of the Vector by calling
     * the `push_back()` member function of the underlying `std::vector`. This
     * member function is only available if the size of the Vector is not known
     * at compile-time, as specified by the concept requirement `requires (N <
     * 0)`.
     *
     * @param[in] v: The value to add to the end of the Vector.
     */
    void pushBack(const T& v) requires (N < 0)
    {
        mContainer.push_back(v.clone());
    }

    void pushBack(const std::shared_ptr<T>& v) requires (N < 0)
    {
        mContainer.push_back(v);
    }

    /**
     * @brief Insert an element at the specified position in the Vector.
     *
     * Inserts the clone of the element `v` at the position specified by `i` in
     * the Vector by calling the `insert()` member function of the underlying
     * `std::vector`. This member function is only available if the size of the
     * Vector is not known at compile-time, as specified by the concept
     * requirement `requires (N < 0)`.
     *
     * @param[in] i: The index at which to insert the element.
     * @param[in] v: The value to insert into the Vector.
     */
    void insert(uint i, const T& v) requires (N < 0)
    {
        assert(i < size() + 1);
        mContainer.insert(mContainer.begin() + i, v.clone());
    }

    void insert(uint i, const std::shared_ptr<T>& v) requires (N < 0)
    {
        assert(i < size() + 1);
        mContainer.insert(mContainer.begin() + i, v);
    }

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

    /* Operators */

    /**
     * @brief Returns a reference to the element at specified location i. No
     * bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A reference to the requested element.
     */
    ValueType operator[](uint i) { return mContainer[i]; }

    /**
     * @brief Returns a const reference to the element at specified location i.
     * No bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A const reference to the requested element.
     */
    ConstValueType operator[](uint i) const { return mContainer[i]; }

    /**
     * @brief Returns a reference to the element at specified location i. No
     * bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A reference to the requested element.
     */
    ValueType operator()(uint i) { return mContainer[i]; }

    /**
     * @brief Returns a const reference to the element at specified location i.
     * No bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A const reference to the requested element.
     */
    ConstValueType operator()(uint i) const { return mContainer[i]; }

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

#endif // VCL_SPACE_POLYMORPHIC_OBJECT_VECTOR_H
