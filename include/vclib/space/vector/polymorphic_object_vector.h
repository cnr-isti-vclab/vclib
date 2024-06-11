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

#ifndef VCL_SPACE_VECTOR_POLYMORPHIC_OBJECT_VECTOR_H
#define VCL_SPACE_VECTOR_POLYMORPHIC_OBJECT_VECTOR_H

#include "pointer_vector.h"

#include <vclib/concepts/polymorphism.h>


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
class PolymorphicObjectVector : protected PointerVector<std::shared_ptr<T>, N>
{
    using Base = PointerVector<std::shared_ptr<T>, N>;
    using BaseVector = Base::Vector;

public:
    // types
    using ValueType = Base::ValueType;

    using ConstValueType = Base::ConstValueType;

    // the iterator of this vector is the const iterator of the base Vector
    // because we don't want to allow to modify the shared pointers contained
    // in the vector, but allow only to modify the objects pointed by the shared
    // pointers
    using Iterator = BaseVector::ConstIterator;

    // the const iterator of this vector is the const iterator of the
    // PointerVector, which transforms pointers to const pointers
    using ConstIterator = Base::ConstIterator;

    using Base::SIZE;

    // constructors
    using Base::Base;

    // exposing members of base class, later we will redefine non-const members
    // that should return values instead of references, and functions that
    // allow to set directly shared pointers
    using Base::size;
    using Base::at;
    using Base::atMod;
    using Base::front;
    using Base::back;
    using Base::data;
    using Base::set;
    using Base::fill;
    using Base::contains;
    using Base::find;
    using Base::indexOf;
    using Base::swap;
    using Base::resize;
    using Base::pushBack;
    using Base::insert;
    using Base::erase;
    using Base::clear;
    using Base::operator[];
    using Base::operator();
    using Base::begin;
    using Base::end;

    /**
     * @brief Creates a Vector object with copied of the elements of the other
     * Vector.
     *
     * If the container is dynamic, its size is set to the size of the other
     * Vector. When the container is static, the size is N and the elements are
     * initialized with the same elements as the other Vector.
     *
     * @param[in] other: The other Vector to copy.
     */
    PolymorphicObjectVector(const PolymorphicObjectVector& other)
    {
        if constexpr (N < 0) {
            Base::resize(other.size());
        }
        std::transform(
            other.begin(), other.end(), Base::begin(), [](const auto& e) {
                return e->clone();
            });
    }

    /**
     * @brief Move constructor.
     * @param[in] other: Another PolymorphicObjectVector container of the same
     * type.
     */
    PolymorphicObjectVector(PolymorphicObjectVector&& other) noexcept
    {
        swap(other);
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
    ValueType at(uint i) { return Base::at(i); }

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
    ValueType atMod(int i) { return Base::atMod(i); }

    /**
     * @brief Access the first element of the Vector.
     *
     * Returns a reference to the first element of the Vector. If  the Vector is
     * empty, the behavior is undefined.
     *
     * @return A reference to the first element.
     */
    ValueType front() { return Base::front(); }

    /**
     * @brief Access the last element of the Vector.
     *
     * Returns a reference to the last element of the Vector. If the Vector is
     * empty, the behavior is undefined.
     *
     * @return A reference to the last element.
     */
    ValueType back() { return Base::back(); }

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
        assert(i < Base::size());
        Base::at(i) = e.clone();
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
    void set(Base::ConstIterator it, const T& e)
    {
        assert(it < Base::end());
        Base::at(it - Base::begin()) = e.clone();
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
        uint n = std::ranges::distance(r);

        if constexpr (N >= 0) {
            n = std::min((uint)N, n);
        }
        else {
            Base::resize(n);
        }

        // for each element in the range, clone it and store it in the
        // vector
        std::transform(
            std::ranges::begin(r),
            std::ranges::begin(r) + n,
            Base::begin(),
            [](const auto& e) {
                return e.clone();
            });
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
        std::fill(Base::begin(), Base::end(), [e]() {
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
     * initialized with a clone of the value `v`.
     *
     * @param[in] n: The new size of the Vector.
     * @param[in] v: The value to use to fill the new elements of the Vector.
     */
    void resize(uint n, const T& v) requires (N < 0)
    {
        if (n > Base::size()) {
            uint oldSize = Base::size();
            Base::resize(n);
            std::fill(Base::begin() + oldSize, Base::end(), [v]() {
                return v.clone();
            });
        }
        else {
            Base::resize(n);
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
        Base::pushBack(v.clone());
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
        assert(i < Base::size() + 1);
        Base::insert(Base::begin() + i, v.clone());
    }

    /**
     * @brief Exchanges the contents of the container with those of other.
     * @param[in] other: Another PolymorphicObjectVector container of the same
     * type.
     */
    void swap(PolymorphicObjectVector& other)
    {
        Base::swap(other);
    }

    /* Operators */

    /**
     * @brief Returns a reference to the element at specified location i. No
     * bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A reference to the requested element.
     */
    ValueType operator[](uint i) { return Base::operator[](i); }

    /**
     * @brief Returns a reference to the element at specified location i. No
     * bounds checking is performed.
     * @param[in] i: Position of the element to return
     * @return A reference to the requested element.
     */
    ValueType operator()(uint i) { return Base::operator()(i); }

    /**
     * @brief Assignment operator of the PolymorphicObjectVector.
     * @param[in] other: Another PolymorphicObjectVector container of the same
     * type.
     * @return A reference to this.
     */
    PolymorphicObjectVector& operator=(PolymorphicObjectVector other)
    {
        swap(other);
        return *this;
    }

    /* Iterator member functions */

    /**
     * @brief Return an iterator pointing to the beginning of the Vector.
     *
     * @return An iterator pointing to the beginning of the Vector.
     */
    Iterator begin() { return Base::begin(); }

    /**
     * @brief Return an iterator pointing to the end of the Vector.
     *
     * @return An iterator pointing to the end of the Vector.
     */
    Iterator end() { return Base::end(); }
};

} // namespace vcl

#endif // VCL_SPACE_VECTOR_POLYMORPHIC_OBJECT_VECTOR_H
