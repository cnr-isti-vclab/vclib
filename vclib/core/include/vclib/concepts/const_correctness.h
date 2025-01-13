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

#ifndef VCL_CONCEPTS_CONST_CORRECTNESS_H
#define VCL_CONCEPTS_CONST_CORRECTNESS_H

#include <vclib/types.h>

#include <type_traits>

namespace vcl {

/**
 * @brief The IsConst concept is satisfied if T satisfies one of the following
 * conditions:
 *
 * - T is const;
 * - T is a pointer to const;
 * - T is a reference to const;
 *
 * @ingroup util_concepts
 */
template<typename T>
concept IsConst =
    std::is_const_v<RemovePtr<T>> || std::is_const_v<RemoveRef<T>>;

} // namespace vcl

#endif // VCL_CONCEPTS_CONST_CORRECTNESS_H
