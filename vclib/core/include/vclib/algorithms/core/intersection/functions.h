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

#ifndef VCL_ALGORITHMS_CORE_INTERSECTION_FUNCTIONS_H
#define VCL_ALGORITHMS_CORE_INTERSECTION_FUNCTIONS_H

#include "intersect.h"

#include <vclib/concepts.h>

namespace vcl {

/**
 * @brief Return a proper intersect function between a Obj1 object and an Obj2
 * object.
 *
 * This function will return a function defined in a specialization of the
 * struct IntersFunctionStruct.
 *
 * The returned function is a std::function that takes in input two const
 * references of the desired types and returns a boolean that tells if the two
 * objects intersect.
 *
 * If the intersection function for your types is not defined, you can write
 * your own IntersFunctionStruct specialization that defines a proper `static
 * const inline` object called `intersFun` of `std::function` type.
 *
 * @note The intersectFunction works only for non-pointer types. This means that
 * you cannot ask for the intersect function between a vcl::Box3 and a
 * vcl::Face*. If you are working with template types that you don't know if
 * they are pointers or not, you can do the following:
 * @code{.cpp}
 * // don't know if T1 and T2 are pointers or non-pointers
 * auto f = vcl::intersectFunction<vcl::RemoveCVRefAndPointer<T1>,
 * vcl::RemoveCVRefAndPointer<T2>>();
 * // obj1 is of type T1, obj2 is of type T2 (may be pointers or non-pointers)
 * bool theyIntersect = f(dereferencePtr(obj1), dereferencePtr(obj2));
 * @endcode
 *
 * @ingroup core_intersection
 */
template<typename Obj1, typename Obj2>
auto intersectFunction()
{
    auto f = [](const Obj1& o1, const Obj2& o2) {
        return intersect(o1, o2);
    };

    return f;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_INTERSECTION_FUNCTIONS_H
