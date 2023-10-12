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

#ifndef VCL_CONCEPTS_SPACE_SAMPLER_H
#define VCL_CONCEPTS_SPACE_SAMPLER_H

#include <vclib/types/base.h>

namespace vcl {

/**
 * @brief A concept for a container that stores Point samples.
 *
 * A type satisfies this concept if it provides the following interface:
 * - `o.samples()`: returns a container of samples in the sampler.
 * - `o.clear()`: clears all previously contained samples.
 * - `o.reserve(uint())`: reserves memory for the given number of samples.
 * - `o.resize(uint())`: resizes the sampler to the given number of samples.
 *
 * @tparam T: The type to be tested for conformity to the SamplerConcept.
 */
template<typename T>
concept SamplerConcept = requires (T o, const T& co) {
    // clang-format off
    typename T::PointType;

    o.samples();

    { co.size() } -> std::same_as<std::size_t>;
    co.sample(uint());

    o.clear();
    o.reserve(uint());
    o.resize(uint());
    
    co.begin();
    co.end();
    // clang-format on
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_SAMPLER_H
