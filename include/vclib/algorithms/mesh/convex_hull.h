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

#ifndef VCL_ALGORITHMS_MESH_CONVEX_HULL_H
#define VCL_ALGORITHMS_MESH_CONVEX_HULL_H

#include <vclib/algorithms/core/visibility.h>
#include <vclib/algorithms/mesh/create/tetrahedron.h>
#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/concepts/mesh.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/shuffle.h>

namespace vcl {

namespace detail {

/**
 * @brief Shuffle the input range of points such that the first four points are
 * not coplanar.
 * 
 * @tparam R 
 * @param points 
 * @param deterministic 
 */
template<vcl::Range R>
void shufflePoints(R&& points, bool deterministic = false)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    vcl::shuffle(points, deterministic);

    auto itP0 = std::ranges::begin(points);
    auto itP1 = std::next(itP0);
    auto itP2 = std::next(itP1);
    auto itP3 = std::next(itP2);
    auto itP = std::next(itP2);

    auto rEnd = std::ranges::end(points);

    while (itP != rEnd && vcl::arePointsCoplanar(*itP0, *itP1, *itP2, *itP)) {
        itP = std::next(itP);
    }

    if (itP == rEnd) {
        throw std::runtime_error("All points are coplanar.");
    }

    std::iter_swap(itP, itP3);
}

template<FaceMeshConcept MeshType, Point3Concept PointType>
MeshType makeTetrahedron(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2,
    const PointType& p3)
{
    using FaceType = MeshType::FaceType;

    MeshType result;

    if (vcl::trianglePointVisibility(p0, p1, p2, p3)) {
        result = vcl::createTetrahedron<MeshType>(p0, p2, p1, p3);
    }
    else {
        result = vcl::createTetrahedron<MeshType>(p0, p1, p2, p3);
    }

    if constexpr (vcl::face::HasOptionalAdjacentFaces<FaceType>) {
        result.enablePerFaceAdjacentFaces();
    }

    vcl::updatePerFaceAdjacentFaces(result);

    return result;
}

} // namespace detail

/**
 * @brief Compute the convex hull of a set of points.
 *
 * @tparam PointType: The type of the points.
 * @param[in] points: The set of points.
 * @param[in] deterministic: If true, the shuffle will be deterministic.
 * @return The convex hull of the points.
 * 
 * @ingroup algorithms_mesh
 */
template<FaceMeshConcept MeshType, Range R>
MeshType convexHull(R&& points, bool deterministic = false)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    using FaceType = MeshType::FaceType;

    static_assert(
        vcl::face::HasAdjacentFaces<FaceType>,
        "MeshType must have per-face adjacent faces.");

    detail::shufflePoints(points, deterministic);

    auto result = detail::makeTetrahedron<MeshType>(
        points[0], points[1], points[2], points[3]);

    // todo: implement the convex hull algorithm

    return result;
}

template<FaceMeshConcept MeshType, Range R>
MeshType convexHull(const R& points, bool deterministic = false)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    using PointType = std::ranges::range_value_t<R>;
    std::vector<PointType> pointsCopy(
        std::ranges::begin(points), std::ranges::end(points));
    return convexHull<MeshType>(pointsCopy, deterministic);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CONVEX_HULL_H