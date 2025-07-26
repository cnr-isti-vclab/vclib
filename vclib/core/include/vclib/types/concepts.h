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

#ifndef VCL_TYPES_CONCEPTS_H
#define VCL_TYPES_CONCEPTS_H

#include "concepts/const_correctness.h"
#include "concepts/iterators.h"
#include "concepts/pointers.h"
#include "concepts/range.h"
#include "concepts/types.h"

/**
 * @defgroup concepts Concepts
 * @ingroup types
 *
 * @brief List of concepts used in the library. They allow to discriminate
 * between all the defined types of the library, and to constrain the input
 * argument types of templated functions and classes.
 *
 * The library provides both generic utility concepts about iterators, pointers
 * or const correctness management, and concepts for types that have been
 * defined in this library. We separate these categories in two subgroups: @ref
 * util_concepts and @ref lib_concepts.
 *
 * This module depends on the @ref types module.
 *
 * You can access all the concepts of VCLib by including `#include
 * <vclib/concepts.h>`
 */

/**
 * @defgroup util_concepts Utility Concepts
 * @ingroup concepts
 *
 * @brief List of utility concepts used in the library, that allows to check and
 * constrain iterators, pointers, const correctness management, ranges, etc.
 */

/**
 * @defgroup lib_concepts Library Concepts
 * @ingroup concepts
 *
 * @brief List of concepts for types that have been defined in this library.
 * They are grouped in different categories depending on the involved types.
 */

#endif // VCL_TYPES_CONCEPTS_H
