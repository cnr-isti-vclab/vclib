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

#ifndef VCL_ALGORITHMS_CORE_POLYGON_GEOMETRY_H
#define VCL_ALGORITHMS_CORE_POLYGON_GEOMETRY_H

#include <vclib/concepts/mesh/elements/face.h>
#include <vclib/space/core/polygon.h>
#include <vclib/views/mesh.h>

namespace vcl {

/**
 * @brief Computes the collinearity test between three points. The test returns
 * a positive value if the points are counter-clockwise, a negative value if the
 * points are clockwise, and zero if the points are collinear.
 *
 * The function computes the z coordinate of the cross product between the
 * vectors p1 - p0 and p2 - p0:
 * - If the result is 0, the points are collinear; 
 * - if the result is positive, the three points constitute a "left turn" or
 *   counter-clockwise orientation (p0 is at the left of the line p1-p2);
 * - if the result is negative, a "right turn" or clockwise orientation
 *   (p0 is at the right of the line p1-p2).
 *
 * @tparam PointType: the type of the points that satisfies the Point2Concept.
 * @param[in] p0: the first point.
 * @param[in] p1: the second point.
 * @param[in] p2: the third point.
 * @return 0 if the points are collinear, a positive value if the points are
 * counter-clockwise, and a negative value if the points are clockwise.
 * 
 * @ingroup algorithms_core
 */
template<Point2Concept PointType>
auto collinearityTest(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2)
{
    return (
        (p1.x() - p0.x()) * (p2.y() - p0.y()) -
        (p2.x() - p0.x()) * (p1.y() - p0.y()));
}

/**
 * @brief Checks if the three points are counter-clockwise.
 *
 * @tparam PointType: the type of the points that satisfies the Point2Concept.
 * @param[in] p0: the first point.
 * @param[in] p1: the second point.
 * @param[in] p2: the third point.
 * @return true if the points are counter-clockwise, false otherwise.
 * 
 * @ingroup algorithms_core
 */
template<Point2Concept PointType>
bool arePointsCounterClockwise(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2)
{
    return collinearityTest(p0, p1, p2) > 0;
}

/**
 * @brief Computes the normal of a face, without modifying the face. Works both
 * for triangle and polygonal faces, and it is optimized in case of triangle
 * faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the normal of the face.
 *
 * @ingroup algorithms_core_polygon
 */
template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceNormal(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::normal(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::normal(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::normal(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Computes the barycenter of a face. Works both for triangle and
 * polygonal faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the barycenter of the face.
 *
 * @ingroup algorithms_core_polygon
 */
template<FaceConcept FaceType>
typename FaceType::VertexType::CoordType faceBarycenter(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::barycenter(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        return Polygon<CoordType>::barycenter(f.vertices() | views::coords);
    }
}

/**
 * @brief Computes the area of a face. Works both for triangle and polygonal
 * faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the area of the face.
 *
 * @ingroup algorithms_core_polygon
 */
template<FaceConcept FaceType>
auto faceArea(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::area(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::area(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::area(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Computes the perimeter of a face. Works both for triangle and
 * polygonal faces, and it is optimized in case of triangle faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @return the perimeter of the face.
 *
 * @ingroup algorithms_core_polygon
 */
template<FaceConcept FaceType>
auto facePerimeter(const FaceType& f)
{
    using CoordType = FaceType::VertexType::CoordType;
    if constexpr (TriangleFaceConcept<FaceType>) {
        return Triangle<CoordType>::perimeter(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());
    }
    else {
        if (f.vertexNumber() == 3) {
            return Triangle<CoordType>::perimeter(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());
        }
        else {
            return Polygon<CoordType>::perimeter(f.vertices() | views::coords);
        }
    }
}

/**
 * @brief Returns the internal angle (in radians) of the vi-th vertex of the
 * face.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the input face.
 * @param[in] vi: the index of the vertex in the face on which calculate the
 * angle
 * @return the angle in radians at the vi-th vertex.
 *
 * @ingroup algorithms_core_polygon
 */
template<FaceConcept FaceType>
auto faceAngleOnVertexRad(const FaceType& f, uint vi)
{
    const auto& p0 = f.vertex(vi)->coord();
    const auto& p1 = f.vertexMod((int) vi + 1)->coord();
    const auto& p2 = f.vertexMod((int) vi - 1)->coord();
    return (p2 - p0).angle(p1 - p0);
}

template<Point2IteratorConcept Iterator>
void sortConvexPolygonVertices(Iterator begin, Iterator end)
{
    using PointType = std::decay_t<decltype(*begin)>;

    // Find the point with the lowest x-coordinate
    PointType minPoint = *std::min_element(
        begin, end, [](const PointType& a, const PointType& b) {
            return a.y() < b.y();
        });

    // Sort the points based on the angle with respect to minPoint
    std::sort(begin, end, [minPoint](const PointType& a, const PointType& b) {
        double angleA = std::atan2(a.y() - minPoint.y(), a.x() - minPoint.x());
        double angleB = std::atan2(b.y() - minPoint.y(), b.x() - minPoint.x());
        return angleA < angleB;
    });
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_GEOMETRY_H
