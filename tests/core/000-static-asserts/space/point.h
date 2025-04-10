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

#ifndef POINT_H
#define POINT_H

#include <vclib/space.h>

void pointStaticAsserts()
{
    using namespace vcl;

    using PointType  = Point<uint16_t, 5>;
    using Point2Type = Point2i;
    using Point3Type = Point3f;
    using Point4Type = Point4d;

    PointType p = Eigen::Matrix<uint16_t, 1, 5>();

    // point
    static_assert(
        PointConcept<PointType>, "Point does not satisfy the PointConcept");
    static_assert(
        PointConcept<const PointType>,
        "const Point does not satisfy the PointConcept");
    static_assert(
        PointConcept<PointType&>, "Point& does not satisfy the PointConcept");
    static_assert(
        PointConcept<const PointType&>,
        "const Point& does not satisfy the PointConcept");
    static_assert(
        PointConcept<PointType&&>, "Point&& does not satisfy the PointConcept");
    static_assert(Serializable<PointType>, "Point is not serializable");

    // point2
    static_assert(
        Point2Concept<Point2Type>, "Point2 does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<const Point2Type>,
        "const Point2 does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<Point2Type&>,
        "Point2& does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<const Point2Type&>,
        "const Point2& does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<Point2Type&&>,
        "Point2&& does not satisfy the Point2Concept");
    static_assert(Serializable<Point2Type>, "Point2 is not serializable");

    // point3
    static_assert(
        Point3Concept<Point3Type>, "Point3 does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<const Point3Type>,
        "const Point3 does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<Point3Type&>,
        "Point3& does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<const Point3Type&>,
        "const Point3& does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<Point3Type&&>,
        "Point3&& does not satisfy the Point3Concept");
    static_assert(Serializable<Point3Type>, "Point3 is not serializable");

    // point4
    static_assert(
        Point4Concept<Point4Type>, "Point4 does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<const Point4Type>,
        "const Point4 does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<Point4Type&>,
        "Point4& does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<const Point4Type&>,
        "const Point4& does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<Point4Type&&>,
        "Point4&& does not satisfy the Point4Concept");
    static_assert(Serializable<Point4Type>, "Point4 is not serializable");

    static_assert(
        PointIteratorConcept<std::vector<Point2d>::iterator>,
        "Vector of Point2 iterator does not satisfy the PointIteratorConcept");
    static_assert(
        PointIteratorConcept<std::vector<Point3d>::const_iterator>,
        "Vector of Point3 const iterator does not satisfy the "
        "PointIteratorConcept");
}

#endif // POINT_H
