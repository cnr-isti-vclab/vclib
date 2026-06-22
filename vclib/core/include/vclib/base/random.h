/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_BASE_RANDOM_H
#define VCL_BASE_RANDOM_H

#include <vclib/base/concepts/range.h>

#include <algorithm>
#include <optional>
#include <random>
#include <variant>

namespace vcl {

/**
 * @brief Configuration type for random number generation behavior.
 *
 * This variant abstracts the source of randomness, providing maximum
 * flexibility without the need to write multiple overloads for every function.
 *
 * All functions in the VCLib codebase that require random generation take an
 * instance of this type as an optional parameter, defaulting to
 * `std::monostate`.
 *
 * Depending on the passed argument, the behavior adapts as follows:
 * - **std::monostate** (Default): The function will use a purely random,
 * persistent internal generator (typically a `thread_local` instance).
 * - **uint** (Seed): The function will instantiate a temporary deterministic
 * generator initialized with the provided seed for a single-use sequence.
 * - **std::reference_wrapper<std::mt19937>**: The function will use a
 * user-provided external generator, advancing its state. Thanks to implicit
 * conversions, callers can simply pass the generator directly without wrapping
 * it in `std::ref()`.
 *
 * @note You do not need to construct the `std::variant` manually. The compiler
 * will automatically deduce the correct state based on the argument provided at
 * the call site.
 *
 * @ingroup base
 */
using RandomConfig =
    std::variant<std::monostate, uint, std::reference_wrapper<std::mt19937>>;

/**
 * @brief Creates a random number generator with an optional seed.
 *
 * If a seed is provided, the generator is seeded with that value, otherwise
 * it uses a random device to generate a random seed.
 *
 * @param seed: Optional seed value for the random number generator.
 * @return A std::mt19937 random number generator.
 *
 * @ingroup base
 */
inline std::mt19937 randomGenerator(std::optional<uint> seed = std::nullopt)
{
    std::mt19937 gen;

    if (seed.has_value()) {
        gen.seed(seed.value());
    }
    else {
        std::random_device rd;
        gen.seed(rd()); // random seed
    }

    return gen;
}

/**
 * @brief Calls a function with a random number generator based on the provided
 * RandomConfig.
 *
 * Depending on the type of RandomConfig, the function will be called with:
 * - A reference to a user-provided std::mt19937 generator.
 * - A temporary std::mt19937 generator seeded with the provided seed.
 * - A thread-local std::mt19937 generator if no config is provided.
 *
 * @tparam Func: The type of the function to call. It should accept a
 * std::mt19937& as its first parameter.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 * @param[in] func: The function to call with the appropriate random number
 * generator.
 *
 * @return The result of calling func with the appropriate random number
 * generator.
 *
 * @ingroup base
 */
template<typename Func>
decltype(auto) callWithRandomGenerator(RandomConfig config, Func&& func)
{
    // Case 1: the user provides a reference to a std::mt19937 generator
    if (auto* userGen =
            std::get_if<std::reference_wrapper<std::mt19937>>(&config)) {
        return func(userGen->get());
    }
    // Case 2: the user provides a seed for a temporary generator
    else if (auto* seed = std::get_if<uint>(&config)) {
        std::mt19937 tempGen = randomGenerator(*seed);
        return func(tempGen);
    }
    // Case 3: the user provides std::monostate or no config, use a thread-local
    // generator
    else {
        thread_local std::mt19937 internal_gen = randomGenerator();
        return func(internal_gen);
    }
}

/**
 * @brief Shuffle the elements of a range.
 *
 * @tparam R: Type of the range.
 * @param[in] range: Range to shuffle.
 * @param[in] config: RandomConfig that determines how to provide the random
 * number generator.
 *
 * @ingroup base
 */
template<Range R>
void shuffle(R&& range, RandomConfig config = std::monostate())
{
    callWithRandomGenerator(config, [&](std::mt19937& gen) {
        std::shuffle(range.begin(), range.end(), gen);
    });
}

} // namespace vcl

#endif // VCL_BASE_RANDOM_H
