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

#ifndef VCL_CONCEPTS_H
#define VCL_CONCEPTS_H

#include "concepts/iterators.h"
#include "concepts/logger.h"
#include "concepts/mesh.h"
#include "concepts/pointers.h"
#include "concepts/range.h"
#include "concepts/space.h"
#include "concepts/types.h"

/**
 * @defgroup concepts Concepts
 *
 * @brief List of concepts used in the library. They allow to discriminate
 * between all the defined types of the library, and to constrain the input
 * argument types of templated functions and classes.
 *
 * The library provides both generic utility concepts about iterators, pointers
 * or const correctness management, and concepts for types that have been
 * defined in this library. We separate these categories in two subgroups: @ref
 * uconcepts and @ref lconcepts.
 *
 * You can access all the concepts of VCLib by including `#include
 * <vclib/concepts.h>`
 */

/**
 * @defgroup uconcepts Utility Concepts
 * @ingroup concepts
 *
 * @brief List of utility concepts used in the library, that allows to check and
 * constrain iterators, pointers, const correctness management, ranges, etc.
 */

/**
 * @defgroup lconcepts Library Concepts
 * @ingroup concepts
 *
 * @brief List of concepts for types that have been defined in this library.
 * They are grouped in different categories depending on the involved types.
 */

#endif // VCL_CONCEPTS_H
