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

#ifndef VCL_CONCEPTS_SPACE_SAMPLER_H
#define VCL_CONCEPTS_SPACE_SAMPLER_H

#include <vclib/concepts/const_correctness.h>
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
concept SamplerConcept = requires (T&& obj) {
    // TODO: make this concept more meaningful: add more requirements
    // on return types and parameters.
    typename RemoveRef<T>::PointType;

    obj.samples();

    { obj.size() } -> std::same_as<std::size_t>;
    obj.sample(uint());

    obj.begin();
    obj.end();

    // non const requirements
    requires IsConst<T> || requires {
        obj.clear();
        obj.reserve(uint());
        obj.resize(uint());
    };
};

} // namespace vcl

#endif // VCL_CONCEPTS_SPACE_SAMPLER_H
