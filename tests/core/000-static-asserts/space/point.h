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

#ifndef POINT_H
#define POINT_H

#include <vclib/space.h>

void pointStaticAsserts()
{
    using namespace vcl;

    // point
    static_assert(
        PointConcept<Point<uint16_t, 5>>,
        "Point does not satisfy the PointConcept");

    // point2
    static_assert(
        Point2Concept<Point2i>, "Point2i does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<Point2f>, "Point2f does not satisfy the Point2Concept");
    static_assert(
        Point2Concept<Point2d>, "Point2d does not satisfy the Point2Concept");
    static_assert(Serializable<Point2d>, "Point2d is not serializable");

    // point3
    static_assert(
        Point3Concept<Point3i>, "Point3i does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<Point3f>, "Point3f does not satisfy the Point3Concept");
    static_assert(
        Point3Concept<Point3d>, "Point3d does not satisfy the Point3Concept");
    static_assert(Serializable<Point3d>, "Point3d is not serializable");

    // point4
    static_assert(
        Point4Concept<Point4i>, "Point4i does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<Point4f>, "Point4f does not satisfy the Point4Concept");
    static_assert(
        Point4Concept<Point4d>, "Point4d does not satisfy the Point4Concept");
    static_assert(Serializable<Point4d>, "Point4d is not serializable");
}

#endif // POINT_H
