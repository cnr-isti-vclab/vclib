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

#include <vector>

#include <vclib/types.h>

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
    T obj,
    const T& cObj,
    typename T::Iterator it,
    typename T::ConstIterator cIt,
    typename T::ValueType v,
    std::vector<typename T::ValueType> vecV,
    const std::vector<typename T::ValueType>& cVecVR) {
    typename T::ValueType;
    typename T::Scalar;
    typename T::ConstReference;
    typename T::Reference;
    typename T::ConstPointer;
    typename T::Pointer;
    typename T::ConstIterator;
    typename T::Iterator;

    obj.DIM;

    { obj.size(std::size_t()) } -> std::integral;
    { obj.empty() } -> std::convertible_to<bool>;

    { obj.data() } -> std::same_as<typename T::Pointer>;
    { cObj.data() } -> std::same_as<typename T::ConstPointer>;

    { obj.stdVector() } -> std::same_as<decltype(vecV)>;
    { cObj.stdVector() } -> std::same_as<decltype(cVecVR)>;

    { obj.fill(v) } -> std::same_as<void>;
    { obj.clear() } -> std::same_as<void>;

    cObj.subArray(uint());

    { obj.begin() } -> std::same_as<decltype(it)>;
    { obj.end() } -> std::same_as<decltype(it)>;
    { cObj.begin() } -> std::same_as<decltype(cIt)>;
    { cObj.end() } -> std::same_as<decltype(cIt)>;
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
concept Array2Concept = ArrayConcept<T> && requires (
                                               T                          obj,
                                               const T&                   cObj,
                                               std::size_t                n,
                                               typename T::Reference      ref,
                                               typename T::ConstReference cRef,
                                               typename T::Pointer        ptr,
                                               typename T::ConstPointer cPtr) {
    requires T::DIM == 2;

    T(n, n);

    { cObj.rows() } -> std::integral;
    { cObj.cols() } -> std::integral;
    { cObj.sizeX() } -> std::integral;
    { cObj.sizeY() } -> std::integral;

    { obj.operator()(n, n) } -> std::same_as<decltype(ref)>;
    { cObj.operator()(n, n) } -> std::same_as<decltype(cRef)>;

    { obj.data(n) } -> std::same_as<decltype(ptr)>;
    { cObj.data(n) } -> std::same_as<decltype(cPtr)>;

    { obj.resize(n, n) } -> std::same_as<void>;
    { obj.conservativeResize(n, n) } -> std::same_as<void>;
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
concept Array3Concept = ArrayConcept<T> && requires (
                                               T                          obj,
                                               const T&                   cObj,
                                               std::size_t                n,
                                               typename T::Reference      ref,
                                               typename T::ConstReference cRef,
                                               typename T::Pointer        ptr,
                                               typename T::ConstPointer cPtr) {
    requires T::DIM == 3;

    T(n, n, n);

    { cObj.sizeX() } -> std::integral;
    { cObj.sizeY() } -> std::integral;
    { cObj.sizeZ() } -> std::integral;

    { obj.operator()(n, n, n) } -> std::same_as<decltype(ref)>;
    { cObj.operator()(n, n, n) } -> std::same_as<decltype(cRef)>;

    { obj.data(n) } -> std::same_as<decltype(ptr)>;
    { cObj.data(n) } -> std::same_as<decltype(cPtr)>;
    { obj.data(n, n) } -> std::same_as<decltype(ptr)>;
    { cObj.data(n, n) } -> std::same_as<decltype(cPtr)>;

    { obj.resize(n, n, n) } -> std::same_as<void>;
    { obj.conservativeResize(n, n, n) } -> std::same_as<void>;
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
concept Array4Concept = ArrayConcept<T> && requires (
                                               T                          obj,
                                               const T&                   cObj,
                                               std::size_t                n,
                                               typename T::Reference      ref,
                                               typename T::ConstReference cRef,
                                               typename T::Pointer        ptr,
                                               typename T::ConstPointer cPtr) {
    requires T::DIM == 4;

    T(n, n, n, n);

    { cObj.sizeX() } -> std::integral;
    { cObj.sizeY() } -> std::integral;
    { cObj.sizeZ() } -> std::integral;
    { cObj.sizeW() } -> std::integral;

    { obj.operator()(n, n, n, n) } -> std::same_as<decltype(ref)>;
    { cObj.operator()(n, n, n, n) } -> std::same_as<decltype(cRef)>;

    { obj.data(n) } -> std::same_as<decltype(ptr)>;
    { cObj.data(n) } -> std::same_as<decltype(cPtr)>;
    { obj.data(n, n) } -> std::same_as<decltype(ptr)>;
    { cObj.data(n, n) } -> std::same_as<decltype(cPtr)>;
    { obj.data(n, n, n) } -> std::same_as<decltype(ptr)>;
    { cObj.data(n, n, n) } -> std::same_as<decltype(cPtr)>;

    { obj.resize(n, n, n, n) } -> std::same_as<void>;
    { obj.conservativeResize(n, n, n, n) } -> std::same_as<void>;
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_ARRAY_H
