/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_ALGORITHMS_INTERSECTION_FUNCTIONS_H
#define VCL_ALGORITHMS_INTERSECTION_FUNCTIONS_H

#include "element.h"
#include "misc.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

// generic case - nothing is done here. It will fail because a specialization
// must exist
template<typename Obj1, typename Obj2>
struct IntersFunctionStruct
{
    static_assert(
        sizeof(Obj1) != sizeof(Obj1), // always fail
        "There is no a IntersFunctionStruct specialization for the given "
        "types.");
};

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
 * auto f = intersectFunction<RemoveCVRefAndPointer<T1>,
 * RemoveCVRefAndPointer<T2>>();
 * // obj1 is of type T1, obj2 is of type T2 (may be pointers or non-pointers)
 * bool theyIntersect = f(dereferencePtr(obj1), dereferencePtr(obj2));
 * @endcode
 *
 * @ingroup intersection
 */
template<typename Obj1, typename Obj2>
auto intersectFunction()
{
    return IntersFunctionStruct<Obj1, Obj2>::intersFun;
}

/********* IntersFunctionStruct Specializations *********/

// Note: for every specialization there is also its commutative;
//
// List of Specializations:
// - Box3-Face
// - Sphere-Face
// - Plane-Box3
// - Plane-Segment
// - Sphere-Box3

// Specialization for intersection between Box and Face
template<Box3Concept Obj1, FaceConcept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return faceBoxIntersect(o2, o1);
    };
};

// Specialization for intersection between Face and Box
template<FaceConcept Obj1, Box3Concept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return faceBoxIntersect(o1, o2);
    };
};

// Specialization for intersection between Sphere and Face
template<SphereConcept Obj1, FaceConcept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return faceSphereIntersect(o2, o1);
    };
};

// Specialization for intersection between Face and Sphere
template<FaceConcept Obj1, SphereConcept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return faceSphereIntersect(o1, o2);
    };
};

// Specialization for intersection between Plane and Box3
template<PlaneConcept Obj1, Box3Concept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return planeBoxIntersect(o1, o2);
    };
};

// Specialization for intersection between Box3 and Plane
template<Box3Concept Obj1, PlaneConcept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return planeBoxIntersect(o2, o1);
    };
};

// Specialization for intersection between Plane and Segment3
template<PlaneConcept Obj1, Segment3Concept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return planeSegmentIntersect(o1, o2);
    };
};

// Specialization for intersection between Segment3 and Plane
template<Segment3Concept Obj1, PlaneConcept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return planeSegmentIntersect(o2, o1);
    };
};

// Specialization for intersection between Sphere and Box3
template<SphereConcept Obj1, Box3Concept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return sphereBoxIntersect(o1, o2);
    };
};

// Specialization for intersection between Box3 and Sphere
template<Box3Concept Obj1, SphereConcept Obj2>
struct IntersFunctionStruct<Obj1, Obj2>
{
    static inline const auto intersFun = [](const Obj1& o1, const Obj2& o2) {
        return sphereBoxIntersect(o2, o1);
    };
};

} // namespace vcl

#endif // VCL_ALGORITHMS_INTERSECTION_FUNCTIONS_H
