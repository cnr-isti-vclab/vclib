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
concept ArrayConcept = requires (T o, const T& co) {
    // clang-format off
    typename T::ValueType;
    typename T::Scalar;
    typename T::ConstReference;
    typename T::Reference;
    typename T::ConstPointer;
    typename T::Pointer;
    typename T::ConstIterator;
    typename T::Iterator;

    o.DIM;

    { o.size(std::size_t()) } -> std::convertible_to<std::size_t>;
    { o.empty() } -> std::convertible_to<bool>;

    { o.data() } -> std::same_as<typename T::Pointer>;
    { co.data() } -> std::same_as<typename T::ConstPointer>;

    { o.stdVector() } -> std::same_as<std::vector<typename T::ValueType>>;
    { co.stdVector() } ->
        std::same_as<const std::vector<typename T::ValueType>&>;

    o.fill(typename T::ValueType());
    o.clear();

    co.subArray(uint());

    { o.begin() } -> std::same_as<typename T::Iterator>;
    { o.end() } -> std::same_as<typename T::Iterator>;
    { co.begin() } -> std::same_as<typename T::ConstIterator>;
    { co.end() } -> std::same_as<typename T::ConstIterator>;
    // clang-format on
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
concept Array2Concept = ArrayConcept<T> && requires (T o, const T& co) {
    // clang-format off
    requires T::DIM == 2;

    T(std::size_t(), std::size_t());

    { co.rows() } -> std::convertible_to<std::size_t>;
    { co.cols() } -> std::convertible_to<std::size_t>;
    { co.sizeX() } -> std::convertible_to<std::size_t>;
    { co.sizeY() } -> std::convertible_to<std::size_t>;

    { o.operator()(std::size_t(), std::size_t()) } ->
        std::same_as<typename T::Reference>;
    { co.operator()(std::size_t(), std::size_t()) } ->
        std::same_as<typename T::ConstReference>;

    { o.data(std::size_t()) } -> std::same_as<typename T::Pointer>;
    { co.data(std::size_t()) } -> std::same_as<typename T::ConstPointer>;

    { o.resize(std::size_t(), std::size_t()) } -> std::same_as<void>;
    { o.conservativeResize(std::size_t(), std::size_t()) } ->
        std::same_as<void>;
    // clang-format on
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
concept Array3Concept = ArrayConcept<T> && requires (T o, const T& co) {
    // clang-format off
    requires T::DIM == 3;

    T(std::size_t(), std::size_t(), std::size_t());

    { co.sizeX() } -> std::convertible_to<std::size_t>;
    { co.sizeY() } -> std::convertible_to<std::size_t>;
    { co.sizeZ() } -> std::convertible_to<std::size_t>;

    { o.operator()(std::size_t(), std::size_t(), std::size_t()) } ->
        std::same_as<typename T::Reference>;
    { co.operator()(std::size_t(), std::size_t(), std::size_t()) } ->
        std::same_as<typename T::ConstReference>;

    { o.data(std::size_t()) } -> std::same_as<typename T::Pointer>;
    { co.data(std::size_t()) } -> std::same_as<typename T::ConstPointer>;
    { o.data(std::size_t(), std::size_t()) } ->
        std::same_as<typename T::Pointer>;
    { co.data(std::size_t(), std::size_t()) } ->
        std::same_as<typename T::ConstPointer>;

    { o.resize(std::size_t(), std::size_t(), std::size_t()) } ->
        std::same_as<void>;
    { o.conservativeResize(std::size_t(), std::size_t(), std::size_t()) } ->
        std::same_as<void>;
    // clang-format on
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
concept Array4Concept = ArrayConcept<T> && requires (T o, const T& co) {
    // clang-format off
    requires T::DIM == 4;

    T(std::size_t(), std::size_t(), std::size_t(), std::size_t());

    { co.sizeX() } -> std::convertible_to<std::size_t>;
    { co.sizeY() } -> std::convertible_to<std::size_t>;
    { co.sizeZ() } -> std::convertible_to<std::size_t>;
    { co.sizeW() } -> std::convertible_to<std::size_t>;

    {
        o.operator()(std::size_t(), std::size_t(), std::size_t(), std::size_t())
    } -> std::same_as<typename T::Reference>;
    {
        co.operator()(
            std::size_t(), std::size_t(), std::size_t(), std::size_t())
    } -> std::same_as<typename T::ConstReference>;

    { o.data(std::size_t()) } -> std::same_as<typename T::Pointer>;
    { co.data(std::size_t()) } -> std::same_as<typename T::ConstPointer>;
    { o.data(std::size_t(), std::size_t()) } ->
        std::same_as<typename T::Pointer>;
    { co.data(std::size_t(), std::size_t()) } ->
        std::same_as<typename T::ConstPointer>;
    { o.data(std::size_t(), std::size_t(), std::size_t()) } ->
        std::same_as<typename T::Pointer>;
    { co.data(std::size_t(), std::size_t(), std::size_t()) } ->
        std::same_as<typename T::ConstPointer>;

    { o.resize(std::size_t(), std::size_t(), std::size_t(), std::size_t()) } ->
        std::same_as<void>;
    {
        o.conservativeResize(
            std::size_t(), std::size_t(), std::size_t(), std::size_t())
    } -> std::same_as<void>;
    // clang-format on
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_ARRAY_H
