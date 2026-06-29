// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BASE_CONCEPTS_CONST_CORRECTNESS_H
#define VCL_BASE_CONCEPTS_CONST_CORRECTNESS_H

#include <vclib/base/pointers.h>

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

#endif // VCL_BASE_CONCEPTS_CONST_CORRECTNESS_H
