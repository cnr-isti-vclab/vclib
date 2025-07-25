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

#ifndef VCL_ALGORITHMS_CORE_DISTANCE_FUNCTIONS_H
#define VCL_ALGORITHMS_CORE_DISTANCE_FUNCTIONS_H

#include "element.h"
#include "misc.h"

#include <vclib/concepts.h>

namespace vcl {

namespace detail {

/**
 * @brief Concept that checks if exists a bounded distance function between two
 * objects of type Obj1 and Obj2.
 *
 * It is used by the boundedDistFunction() function to check if a proper
 * bounded distance function exists for the given types. If it does not exist,
 * the function will return the standard distance function, and the bound
 * parameter will be ignored.
 *
 * @tparam Obj1
 * @tparam Obj2
 * @tparam ST
 */
template<typename Obj1, typename Obj2, typename ST>
concept BoundedDistFunctionExists =
    requires (Obj1 o1, Obj2 o2, ST v) { boundedDistance(o1, o2, v); };

} // namespace detail

/**
 * @brief Return a proper dist function between a Obj1 object and an Obj2
 * object.
 *
 * The returned function is a lambda function that takes in input two const
 * references of the desired types and returns a scalar that is the distance
 * between the two objects.
 *
 * If the distance function for your types is not defined, you can write your
 * own vcl::distance overload that defines a proper distance function.
 *
 * @note The distFunction works only for non-pointer types, because generally
 * the overloads of vcl::distance do not take pointers as input. This means that
 * you cannot ask for the distance function between a vcl::Point3d and a
 * vcl::Vertex*. If you are working with template types that you don't know if
 * they are pointers or not, you can do the following:
 * @code{.cpp}
 * // don't know if T1 and T2 are pointers or non-pointers
 * auto f = distFunction<RemoveCVRefAndPointer<T1>,
 * RemoveCVRefAndPointer<T2>>();
 * // obj1 is of type T1, obj2 is of type T2 (may be pointers or non-pointers)
 * auto dist = f(dereferencePtr(obj1), dereferencePtr(obj2));
 * @endcode
 *
 * @ingroup core_distance
 */
template<typename Obj1, typename Obj2>
auto distFunction()
{
    auto f = [](const Obj1& o1, const Obj2& o2) {
        return distance(o1, o2);
    };

    return f;
}

/**
 * @brief Return a proper bounded distance function between a Obj1 object and an
 * Obj2 object.
 *
 * The returned function is a lambda function that takes in input two const
 * references of the desired types, a scalar that represents the bound, and
 * returns a scalar that is the distance between the two objects if it is less
 * than the bound. If the distance is greater than the bound, the distance was
 * not computed totally and should be discarded.
 *
 * This function is different w.r.t. the distFunction because the returned
 * function here takes three arguments: the two objects on which compute the
 * distance, and a scalar that represent the maximum distance that can be
 * returned. This value is used by some distance functions for performance
 * reasons: it allows to avoid several computation if a first distance does
 * already exceed the maximum value.
 *
 * If the bounded distance function is not defined, the standard distance
 * function will be used, and the bound parameter will be ignored.
 * If the distance function for your types is not defined, you can write your
 * own vcl::boundedDistance (or vcl::distance) overload that defines a proper
 * bounded distance function.
 *
 * @note The boundedDistFunction works only for non-pointer types, because
 * generally the overloads of vcl::distance and vcl::boundedDistance do not take
 * pointers as input. This means that you cannot ask for the distance function
 * between a vcl::Point3d and a vcl::Vertex*. If you are working with template
 * types that you don't know if they are pointers or not, you can do the
 * following:
 * @code{.cpp}
 * // don't know if T1 and T2 are pointers or non-pointers
 * auto f = boundedDistFunction<RemoveCVRefAndPointer<T1>,
 * RemoveCVRefAndPointer<T2>, double>();
 * // obj1 is of type T1, obj2 is of type T2 (may be pointers or non-pointers)
 * auto dist = f(dereferencePtr(obj1), dereferencePtr(obj2), maxDist);
 * @endcode
 *
 * @ingroup core_distance
 */
template<typename Obj1, typename Obj2, typename ScalarType = double>
auto boundedDistFunction()
{
    if constexpr (detail::BoundedDistFunctionExists<Obj1, Obj2, ScalarType>) {
        auto f = [](const Obj1& o1, const Obj2& o2, ScalarType s) {
            return boundedDistance(o1, o2, s);
        };

        return f;
    }
    else {
        auto f = [](const Obj1& o1, const Obj2& o2, ScalarType) {
            return distance(o1, o2);
        };

        return f;
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_DISTANCE_FUNCTIONS_H
