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

#ifndef VCL_CONCEPTS_SPACE_ARRAY_H
#define VCL_CONCEPTS_SPACE_ARRAY_H

#include <vclib/concepts/const_correctness.h>
#include <vclib/concepts/iterators.h>
#include <vclib/concepts/ranges/range.h>

#include <vclib/types.h>

#include <vector>

namespace vcl {

/**
 * @brief A concept representing a N-dimensional array.
 *
 * This concept defines a set of requirements that must be satisfied by any type
 * that wants to model a N-dimensional array, where N is known at compile time.
 *
 * @tparam T: The type to be tested for conformity to the ArrayConcept.
 */
template<typename T>
concept ArrayConcept = requires (
    T&&                                                  obj,
    typename RemoveRef<T>::ValueType                     v,
    typename RemoveRef<T>::Pointer                       ptr,
    typename RemoveRef<T>::ConstPointer                  cPtr,
    std::vector<typename RemoveRef<T>::ValueType>        vecV,
    const std::vector<typename RemoveRef<T>::ValueType>& cVecVR) {
    // inner types
    typename RemoveRef<T>::ValueType;
    typename RemoveRef<T>::Scalar;
    typename RemoveRef<T>::ConstReference;
    typename RemoveRef<T>::Reference;
    typename RemoveRef<T>::ConstPointer;
    typename RemoveRef<T>::Pointer;
    typename RemoveRef<T>::ConstIterator;
    typename RemoveRef<T>::Iterator;

    obj.DIM;

    { obj.size(std::size_t()) } -> std::integral;
    { obj.empty() } -> std::convertible_to<bool>;

    { obj.data() } -> std::convertible_to<decltype(cPtr)>;

    { obj.stdVector() } -> std::convertible_to<decltype(cVecVR)>;

    obj.subArray(uint());

    { obj.begin() } -> InputIterator<decltype(v)>;
    { obj.end() } -> InputIterator<decltype(v)>;

    requires InputRange<T, decltype(v)>;

    // non const requirements
    requires vcl::IsConst<T> || requires {
        { obj.data() } -> std::same_as<decltype(ptr)>;

        { obj.stdVector() } -> std::same_as<decltype(vecV)>;

        { obj.fill(v) } -> std::same_as<void>;
        { obj.clear() } -> std::same_as<void>;

        { obj.begin() } -> OutputIterator<decltype(v)>;
        { obj.end() } -> OutputIterator<decltype(v)>;

        requires OutputRange<T, decltype(v)>;
    };
};

/**
 * @brief A concept representing a 2-dimensional array.
 *
 * This concept defines a set of requirements that must be satisfied by any type
 * that wants to model a 2-dimensional array.
 *
 * @tparam T: The type to be tested for conformity to the Array2Concept.
 */
template<typename T>
concept Array2Concept =
    ArrayConcept<T> && requires (
                           T&&                                   obj,
                           std::size_t                           n,
                           typename RemoveRef<T>::Reference      ref,
                           typename RemoveRef<T>::ConstReference cRef,
                           typename RemoveRef<T>::Pointer        ptr,
                           typename RemoveRef<T>::ConstPointer   cPtr) {
        requires RemoveRef<T>::DIM == 2;

        RemoveRef<T>(n, n);

        { obj.rows() } -> std::integral;
        { obj.cols() } -> std::integral;
        { obj.sizeX() } -> std::integral;
        { obj.sizeY() } -> std::integral;

        { obj.operator()(n, n) } -> std::convertible_to<decltype(cRef)>;

        { obj.data(n) } -> std::convertible_to<decltype(cPtr)>;

        // non const requirements
        requires vcl::IsConst<T> || requires {
            { obj.operator()(n, n) } -> std::same_as<decltype(ref)>;

            { obj.data(n) } -> std::same_as<decltype(ptr)>;

            { obj.resize(n, n) } -> std::same_as<void>;
            { obj.conservativeResize(n, n) } -> std::same_as<void>;
        };
    };

/**
 * @brief A concept representing a 3-dimensional array.
 *
 * This concept defines a set of requirements that must be satisfied by any type
 * that wants to model a 3-dimensional array.
 *
 * @tparam T: The type to be tested for conformity to the Array3Concept.
 */
template<typename T>
concept Array3Concept =
    ArrayConcept<T> && requires (
                           T&&                                   obj,
                           std::size_t                           n,
                           typename RemoveRef<T>::Reference      ref,
                           typename RemoveRef<T>::ConstReference cRef,
                           typename RemoveRef<T>::Pointer        ptr,
                           typename RemoveRef<T>::ConstPointer   cPtr) {
        requires RemoveRef<T>::DIM == 3;

        RemoveRef<T>(n, n, n);

        { obj.sizeX() } -> std::integral;
        { obj.sizeY() } -> std::integral;
        { obj.sizeZ() } -> std::integral;

        { obj.operator()(n, n, n) } -> std::convertible_to<decltype(cRef)>;

        { obj.data(n) } -> std::convertible_to<decltype(cPtr)>;

        { obj.data(n, n) } -> std::convertible_to<decltype(cPtr)>;

        // non const requirements
        requires vcl::IsConst<T> || requires {
            { obj.operator()(n, n, n) } -> std::same_as<decltype(ref)>;
            { obj.data(n) } -> std::same_as<decltype(ptr)>;
            { obj.data(n, n) } -> std::same_as<decltype(ptr)>;
            { obj.resize(n, n, n) } -> std::same_as<void>;
            { obj.conservativeResize(n, n, n) } -> std::same_as<void>;
        };
    };

/**
 * @brief A concept representing a 4-dimensional array.
 *
 * This concept defines a set of requirements that must be satisfied by any type
 * that wants to model a 4-dimensional array.
 *
 * @tparam T: The type to be tested for conformity to the Array4Concept.
 */
template<typename T>
concept Array4Concept =
    ArrayConcept<T> && requires (
                           T&&                                   obj,
                           std::size_t                           n,
                           typename RemoveRef<T>::Reference      ref,
                           typename RemoveRef<T>::ConstReference cRef,
                           typename RemoveRef<T>::Pointer        ptr,
                           typename RemoveRef<T>::ConstPointer   cPtr) {
        requires RemoveRef<T>::DIM == 4;

        RemoveRef<T>(n, n, n, n);

        { obj.sizeX() } -> std::integral;
        { obj.sizeY() } -> std::integral;
        { obj.sizeZ() } -> std::integral;
        { obj.sizeW() } -> std::integral;

        { obj.operator()(n, n, n, n) } -> std::convertible_to<decltype(cRef)>;

        { obj.data(n) } -> std::convertible_to<decltype(cPtr)>;
        { obj.data(n, n) } -> std::convertible_to<decltype(cPtr)>;
        { obj.data(n, n, n) } -> std::convertible_to<decltype(cPtr)>;

        // non const requirements
        requires vcl::IsConst<T> || requires {
            { obj.operator()(n, n, n, n) } -> std::same_as<decltype(ref)>;

            { obj.data(n) } -> std::same_as<decltype(ptr)>;
            { obj.data(n, n) } -> std::same_as<decltype(ptr)>;
            { obj.data(n, n, n) } -> std::same_as<decltype(ptr)>;

            { obj.resize(n, n, n, n) } -> std::same_as<void>;
            { obj.conservativeResize(n, n, n, n) } -> std::same_as<void>;
        };
    };

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_ARRAY_H
