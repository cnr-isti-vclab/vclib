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

#ifndef VCL_SPACE_CORE_PARALLEL_VECTOR_TUPLE_H
#define VCL_SPACE_CORE_PARALLEL_VECTOR_TUPLE_H

#include <vclib/base.h>

#include <array>
#include <span>

namespace vcl {

/**
 * @brief A class that holds multiple vectors of different types in a tuple,
 * constraind to the same size.
 *
 * This class is designed to hold multiple std::vector instances, each of a
 * different type, in a tuple-like structure. The size of all vectors is
 * constrained to be the same, and operations like resize, reserve, and
 * clear are applied to all vectors simultaneously.
 *
 * Each vector can be accessed, trough the index of its type in the template
 * parameter pack, using the `span()` member function. It will allow to access
 * to its elements.
 *
 * The class also supports enabling and disabling individual vectors, which
 * allows for selective operations on the contained vectors. Disabled vectors
 * are not resized or reserved, and their contents are cleared when disabled.
 *
 * The class stores the current size of the vectors, which changes only when
 * resize or clear members are called. The size is not changed when enabling or
 * disabling vectors (even when all vectors are disabled, the size remains
 * unchanged).
 */
template<typename... Types>
class ParallelVectorTuple
{
    using VecTuple = std::tuple<std::vector<Types>...>;

public:
    /**
     * @brief The number of vectors in the tuple.
     */
    static constexpr uint TUPLE_SIZE = std::tuple_size_v<VecTuple>;

private:
    VecTuple mVecTuple;

    std::array<bool, TUPLE_SIZE> mVecEnabled;
    std::size_t                  mSize = 0;

public:
    /**
     * @brief Constructs a ParallelVectorTuple with all vectors enabled.
     */
    ParallelVectorTuple()
    {
        // Initialize all vectors as enabled
        mVecEnabled.fill(true);
    }

    /**
     * @brief The number of vectors in the tuple.
     */
    static constexpr uint tupleSize() { return TUPLE_SIZE; }

    /**
     * @brief Returns the size of the parallel vector tuple.
     *
     * @note The size is the same for all vectors in the tuple and it does not
     * change if the vectors are enabled or disabled. This means that even if
     * all the vectors are disabled, this size could be greater than zero.
     *
     * @return The size of the parallel vector tuple.
     */
    std::size_t size() const { return mSize; }

    /**
     * @brief Returns whether a vector at a given index is enabled.
     * @tparam I: The index of the vector to check.
     * @return True if the vector at index I is enabled, false otherwise.
     */
    template<uint I>
    bool isVectorEnabled() const
    {
        static_assert(I < TUPLE_SIZE, "Index out of bounds");
        return mVecEnabled[I];
    }

    /**
     * @brief Enables the vector at the given index.
     *
     * At the end of the operation, the vector is resized to the current
     * size of the parallel vector tuple, and its elements are initialized
     * to their default values.
     *
     * @note If the vector was already enabled, this operation has no effect.
     * @tparam I: The index of the vector to enable.
     */
    template<uint I>
    void enableVector()
    {
        static_assert(I < TUPLE_SIZE, "Index out of bounds");
        mVecEnabled[I] = true;
        std::get<I>(mVecTuple).resize(mSize);
    }

    /**
     * @brief Disables the vector at the given index.
     *
     * At the end of the operation, the vectori si cleared and all the elements
     * are removed. The size of the parallel vector tuple remains unchanged.
     *
     * @tparam I: The index of the vector to disable.
     */
    template<uint I>
    void disableVector()
    {
        static_assert(I < TUPLE_SIZE, "Index out of bounds");
        mVecEnabled[I] = false;
        std::get<I>(mVecTuple).clear();
    }

    /**
     * @brief Enables all vectors in the tuple.
     *
     * All the vectors in the tuple are enabled, and their sizes are set
     * to the current size of the parallel vector tuple.
     */
    void enableAllVectors()
    {
        mVecEnabled.fill(true);
        if (mSize > 0) {
            auto func = [this](auto&& vec) {
                vec.resize(mSize);
            };
            applyToAllEnabledVectors<TUPLE_SIZE - 1>(func);
        }
    }

    /**
     * @brief Disables all vectors in the tuple.
     *
     * All the vectors in the tuple are disabled, and their contents are
     * cleared. The size of the parallel vector tuple remains unchanged.
     */
    void disableAllVectors()
    {
        mVecEnabled.fill(false);
        auto func = [](auto& vec) {
            vec.clear();
        };
        std::apply(func, mVecTuple);
        // don't change the size!
    }

    /**
     * @brief Returns a const std::span to the vector at the given index.
     *
     * This function allows access to the elements of the vector at the
     * specified index, throwing an exception if the vector is disabled.
     *
     * @throws std::runtime_error if the vector at index I is disabled.
     * @return A const std::span to the vector at index I.
     */
    template<uint I>
    auto span() const
    {
        if (!isVectorEnabled<I>()) {
            throw std::runtime_error(
                "Accessing disabled vector at index " + std::to_string(I));
        }
        using RT = TypeAtT<I, Types...>;
        return std::span<const RT>(std::get<I>(mVecTuple));
    }

    /**
     * @brief Returns a std::span to the vector at the given index.
     *
     * This function allows access to the elements of the vector at the
     * specified index, throwing an exception if the vector is disabled.
     *
     * @throws std::runtime_error if the vector at index I is disabled.
     * @return A std::span to the vector at index I.
     */
    template<uint I>
    auto span()
    {
        if (!isVectorEnabled<I>()) {
            throw std::runtime_error(
                "Accessing disabled vector at index " + std::to_string(I));
        }
        return std::span(std::get<I>(mVecTuple));
    }

    /**
     * @brief Resizes all enabled vectors to the given size, and sets the
     * current size of the parallel vector tuple.
     * @param[in] size: The new size to set for all enabled vectors.
     */
    void resize(std::size_t size)
    {
        if constexpr (TUPLE_SIZE > 0) {
            auto func = [this, size](auto&& vec) {
                vec.resize(size);
            };
            applyToAllEnabledVectors<TUPLE_SIZE - 1>(func);
        }
        mSize = size;
    }

    /**
     * @brief Reserves space for all enabled vectors to the given size.
     * @note This does not change the size of the vectors, it only reserves
     * space.
     * @param[in] size: The new size to reserve for all enabled vectors.
     */
    void reserve(std::size_t size)
    {
        if constexpr (TUPLE_SIZE > 0) {
            auto func = [this, size](auto&& vec) {
                vec.reserve(size);
            };
            applyToAllEnabledVectors<TUPLE_SIZE - 1>(func);
        }
    }

    /**
     * @brief Clears all enabled vectors and resets the size of the
     * parallel vector tuple to zero.
     */
    void clear()
    {
        if constexpr (TUPLE_SIZE > 0) {
            auto func = [](auto& vec) {
                vec.clear();
            };
            applyToAllEnabledVectors<TUPLE_SIZE - 1>(func);
        }
        mSize = 0;
    }

private:
    template<std::size_t N>
    void applyToAllEnabledVectors(auto&& func)
    {
        if (mVecEnabled[N]) {
            func(std::get<N>(mVecTuple));
        }
        if constexpr (N != 0)
            applyToAllEnabledVectors<N - 1>(func);
    }
};

template<typename... Types>
class ParallelVectorTuple<TypeWrapper<Types...>> :
        public ParallelVectorTuple<Types...>
{
};

} // namespace vcl

#endif // VCL_SPACE_CORE_PARALLEL_VECTOR_TUPLE_H
