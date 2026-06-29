// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_CORE_INTERSECTION_FUNCTIONS_H
#define VCL_ALGORITHMS_CORE_INTERSECTION_FUNCTIONS_H

#include "intersect.h"

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
