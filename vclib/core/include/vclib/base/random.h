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

#ifndef VCL_BASE_RANDOM_H
#define VCL_BASE_RANDOM_H

#include <optional>
#include <random>

namespace vcl {

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
    } else {
        std::random_device rd;
        gen.seed(rd()); // random seed
    }

    return gen;
}

} // namespace vcl

#endif // VCL_BASE_RANDOM_H
