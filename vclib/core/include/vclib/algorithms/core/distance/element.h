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

#ifndef VCL_ALGORITHMS_CORE_DISTANCE_ELEMENT_H
#define VCL_ALGORITHMS_CORE_DISTANCE_ELEMENT_H

#include "misc.h"

#include <vclib/algorithms/core/bounding_box.h>
#include <vclib/algorithms/core/polygon.h>
#include <vclib/concepts/mesh.h>
#include <vclib/math/min_max.h>
#include <vclib/space/core/triangle_wrapper.h>

namespace vcl {

/**
 * @brief Computes the distance between a Vertex and a 3D point.
 *
 * @tparam VertexType: The type of vertex. Must satisfy the VertexConcept.
 * @tparam PointType: The type of point. Must satisfy the Point3Concept.
 *
 * @param[in] v: The vertex to calculate the distance from.
 * @param[in] p: The point to calculate the distance to.
 *
 * @return The distance between the vertex and the point.
 *
 * @ingroup core_distance
 */
template<VertexConcept VertexType, Point3Concept PointType>
auto distance(const VertexType& v, const PointType& p)
{
    return v.coord().dist(p);
}

/**
 * @copydoc vcl::distance(const VertexType&, const PointType&)
 *
 * @ingroup core_distance
 */
template<Point3Concept PointType, VertexConcept VertexType>
auto distance(const PointType& p, const VertexType& v)
{
    return v.coord().dist(p);
}

/**
 * @brief Computes the distance between two vertices.
 *
 * @tparam VertexType1: The type of the first vertex. Must satisfy the
 * VertexConcept.
 * @tparam VertexType2: The type of the second vertex. Must satisfy the
 * VertexConcept.
 * @param[in] v1: The first vertex.
 * @param[in] v2: The second vertex.
 *
 * @return The distance between the two vertices.
 *
 * @ingroup core_distance
 */
template<VertexConcept VertexType1, VertexConcept VertexType2>
auto distance(const VertexType1& v1, const VertexType2& v2)
{
    return v1.coord().dist(v2.coord());
}

/**
 * @brief Compute the distance between a 3D point and a face.
 *
 * If the face is a triangle, the function will use the bounded distance
 * function between a point and a triangle. If the face is a polygon, the face
 * is first triangulated using an earcut algorithm, and then for each triangle,
 * the distance between the point and the triangle is computed.
 *
 * @tparam PointType: The type of point. Must satisfy the Point3Concept.
 * @tparam FaceType: The type of face. Must satisfy the FaceConcept.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] f: The face to calculate the distance to.
 * @param[in] maxDist: The maximum distance to consider. If the distance is
 * greater than this value, the function returns immediately. Default is
 * std::numeric_limits<ScalarType>::max().
 * @param[out] closest: The closest point on the triangle to the given point.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the point and the face.
 *
 * @ingroup core_distance
 */
template<Point3Concept PointType, FaceConcept FaceType, typename ScalarType>
auto boundedDistance(
    const PointType& p,
    const FaceType&  f,
    ScalarType       maxDist,
    PointType&       closest,
    bool             signedDist = false)
{
    if constexpr (TriangleFaceConcept<FaceType>) {
        TriangleWrapper tw(
            f.vertex(0)->coord(), f.vertex(1)->coord(), f.vertex(2)->coord());

        return boundedDistance(p, tw, maxDist, closest, signedDist);
    }
    else {
        if (f.vertexNumber() == 3) {
            TriangleWrapper tw(
                f.vertex(0)->coord(),
                f.vertex(1)->coord(),
                f.vertex(2)->coord());

            return boundedDistance(p, tw, maxDist, closest, signedDist);
        }

        ScalarType minDist = maxDist;

        std::vector<uint> tris = earCut(f);
        for (uint i = 0; i < tris.size(); i += 3) {
            PointType  w;
            ScalarType d = boundedDistance(
                p,
                TriangleWrapper(
                    f.vertex(tris[i])->coord(),
                    f.vertex(tris[i + 1])->coord(),
                    f.vertex(tris[i + 2])->coord()),
                minDist,
                w,
                signedDist);

            if (std::abs(d) < minDist) {
                minDist = std::abs(d);
                closest = w;
            }
        }

        return minDist;
    }
}

/**
 * @brief Compute the distance between a 3D point and a face.
 *
 * If the face is a triangle, the function will use the bounded distance
 * function between a point and a triangle. If the face is a polygon, the face
 * is first triangulated using an earcut algorithm, and then for each triangle,
 * the distance between the point and the triangle is computed.
 *
 * @tparam PointType: The type of point. Must satisfy the Point3Concept.
 * @tparam FaceType: The type of face. Must satisfy the FaceConcept.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] f: The face to calculate the distance to.
 * @param[in] maxDist: The maximum distance to consider. If the distance is
 * greater than this value, the function returns immediately. Default is
 * std::numeric_limits<ScalarType>::max().
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the point and the face.
 *
 * @ingroup core_distance
 */
template<Point3Concept PointType, FaceConcept FaceType, typename ScalarType>
auto boundedDistance(
    const PointType& p,
    const FaceType&  f,
    ScalarType       maxDist,
    bool             signedDist = false)
{
    PointType closest;
    return boundedDistance(p, f, maxDist, closest, signedDist);
}

/**
 * @copydoc vcl::boundedDistance(const PointType&, const FaceType&, ScalarType,
 * bool)
 *
 * @ingroup core_distance
 */
template<FaceConcept FaceType, Point3Concept PointType, typename ScalarType>
auto boundedDistance(
    const FaceType&  f,
    const PointType& p,
    ScalarType       maxDist,
    bool             signedDist = false)
{
    PointType closest;
    return boundedDistance(p, f, maxDist, closest, signedDist);
}

/**
 * @brief Compute the distance between a 3D point and a face.
 *
 * If the face is a triangle, the function will use the bounded distance
 * function between a point and a triangle. If the face is a polygon, the face
 * is first triangulated using an earcut algorithm, and then for each triangle,
 * the distance between the point and the triangle is computed.
 *
 * @tparam PointType: The type of point. Must satisfy the Point3Concept.
 * @tparam FaceType: The type of face. Must satisfy the FaceConcept.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] f: The face to calculate the distance to.
 * @param[out] closest: The closest point on the triangle to the given point.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the point and the face.
 *
 * @ingroup core_distance
 */
template<Point3Concept PointType, FaceConcept FaceType>
auto distance(
    const PointType& p,
    const FaceType&  f,
    PointType&       closest,
    bool             signedDist = false)
{
    using ScalarType = PointType::ScalarType;

    ScalarType maxDist = std::numeric_limits<ScalarType>::max();
    return boundedDistance(p, f, maxDist, closest, signedDist);
}

/**
 * @brief Compute the distance between a 3D point and a face.
 *
 * If the face is a triangle, the function will use the bounded distance
 * function between a point and a triangle. If the face is a polygon, the face
 * is first triangulated using an earcut algorithm, and then for each triangle,
 * the distance between the point and the triangle is computed.
 *
 * @tparam PointType: The type of point. Must satisfy the Point3Concept.
 * @tparam FaceType: The type of face. Must satisfy the FaceConcept.
 *
 * @param[in] p: The point to calculate the distance from.
 * @param[in] f: The face to calculate the distance to.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the point and the face.
 *
 * @ingroup core_distance
 */
template<Point3Concept PointType, FaceConcept FaceType>
auto distance(const PointType& p, const FaceType& f, bool signedDist = false)
{
    using ScalarType = PointType::ScalarType;

    PointType closest;

    ScalarType maxDist = std::numeric_limits<ScalarType>::max();
    return boundedDistance(p, f, maxDist, closest, signedDist);
}

/**
 * @copydoc vcl::distance(const PointType&, const FaceType&, bool)
 *
 * @ingroup core_distance
 */
template<FaceConcept FaceType, Point3Concept PointType>
auto distance(const FaceType& f, const PointType& p, bool signedDist = false)
{
    return distance(p, f, signedDist);
}

/**
 * @brief Compute the distance between a vertex and a face.
 *
 * If the face is a triangle, the function will use the bounded distance
 * function between a point and a triangle. If the face is a polygon, the face
 * is first triangulated using an earcut algorithm, and then for each triangle,
 * the distance between the point and the triangle is computed.
 *
 * @tparam VertexType: The type of Vertex. Must satisfy the VertexConcept.
 * @tparam FaceType: The type of face. Must satisfy the FaceConcept.
 *
 * @param[in] v: The vertex to calculate the distance from.
 * @param[in] f: The face to calculate the distance to.
 * @param[in] signedDist: Whether to calculate the signed distance. Default is
 * false.
 * @return The distance between the vertex and the face.
 *
 * @ingroup core_distance
 */
template<VertexConcept VertexType, FaceConcept FaceType>
auto distance(const VertexType& v, const FaceType& f, bool signedDist = false)
{
    return distance(v.coord(), f, signedDist);
}

/**
 * @copydoc vcl::distance(const VertexType&, const FaceType&, bool)
 *
 * @ingroup core_distance
 */
template<FaceConcept FaceType, VertexConcept VertexType>
auto distance(const FaceType& f, const VertexType& v, bool signedDist = false)
{
    return distance(v.coord(), f, signedDist);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_DISTANCE_ELEMENT_H
