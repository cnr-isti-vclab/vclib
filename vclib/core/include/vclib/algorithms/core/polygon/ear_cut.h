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

#ifndef VCL_ALGORITHMS_CORE_POLYGON_EAR_CUT_H
#define VCL_ALGORITHMS_CORE_POLYGON_EAR_CUT_H

#include "projection.h"

#include <vclib/concepts.h>
#include <vclib/space/core.h>
#include <vclib/views.h>

#if __has_include(<mapbox/earcut.hpp>)
#include <mapbox/earcut.hpp>
#else
// inclusion for usage of vclib without CMake - not ideal but necessary for
// header only
#include "../../../../../external/earcut.hpp-2.2.3/include/mapbox/earcut.hpp"
#endif

/* Structs to make working the mapbox earcut algorithm on vcl::Point2 */

namespace mapbox::util {

template<typename Scalar>
struct nth<0, vcl::Point2<Scalar>>
{
    inline static auto get(const vcl::Point2<Scalar>& t) { return t.x(); };
};

template<typename Scalar>
struct nth<1, vcl::Point2<Scalar>>
{
    inline static auto get(const vcl::Point2<Scalar>& t) { return t.y(); };
};

} // namespace mapbox::util

namespace vcl {

/**
 * @brief Triangulates a simple polygon with no holes using the ear-cutting
 * algorithm.
 *
 * @tparam Iterator: The type of iterator used to represent the vertices of
 * the polygon. It must satisfy the Point2Concept requirement.
 * @param[in] begin: An iterator pointing to the first vertex of the
 * polygon.
 * @param[in] end: An iterator pointing to one past the last vertex of the
 * polygon.
 * @return A vector containing the indices of the vertices that form
 * triangles in the triangulated polygon. Each group of three indices
 * represents the vertices of a single triangle, and the indices are ordered
 * in a counter-clockwise direction.
 * @throws std::logic_error If the polygon is not simple or has holes.
 *
 * @remarks This function uses the ear-cutting algorithm to triangulate a
 * simple polygon with no holes. The polygon is represented as a sequence of
 * vertices, where each vertex is a two-dimensional point. The function
 * returns a vector containing the indices of the vertices that form
 * triangles in the triangulated polygon. The indices are ordered in a
 * counter-clockwise direction, and each group of three indices represents
 * the vertices of a single triangle. The function requires that the type of
 * iterator used to represent the vertices of the polygon satisfies the
 * Point2Concept requirement, which means that it must have a value_type
 * that is a Point2 object with a ScalarType member representing the scalar
 * type used to represent the position of the point. If the polygon is
 * not simple or has holes, the function throws a std::logic_error.
 *
 * @ingroup core_polygon
 */
template<Point2IteratorConcept Iterator>
std::vector<uint> earCut(Iterator begin, Iterator end)
{
    using PointT = Iterator::value_type;
    using Scalar = PointT::ScalarType;

    // Convert the polygon represented as a sequence of vertices to a vector
    // of vectors of points. This is necessary because the earcut library
    // expects the polygon to be represented as a vector of contours, where
    // each contour is a vector of points. In this case, there is only one
    // contour, which is represented as a vector of points.
    std::vector<std::vector<Point2<Scalar>>> poly;
    poly.emplace_back(begin, end);

    // Use the earcut library to triangulate the polygon and return the
    // result.
    return mapbox::earcut<uint>(poly);
}

/**
 * @brief Triangulates a simple polygon with no holes in 3D space by
 * projecting it onto a 2D plane and applying the ear-cutting algorithm.
 *
 * @tparam Iterator: The type of iterator used to represent the vertices of
 * the polygon. It must satisfy the Point3Concept requirement.
 * @param[in] begin: An iterator pointing to the first vertex of the
 * polygon.
 * @param[in] end: An iterator pointing to one past the last vertex of the
 * polygon.
 * @return A vector containing the indices of the vertices that form
 * triangles in the triangulated polygon. Each group of three indices
 * represents the vertices of a single triangle, and the indices are ordered
 * in a counter-clockwise direction.
 * @throws std::logic_error If the polygon is not simple or has holes.
 *
 * @remarks This function triangulates a simple polygon with no holes in 3D
 * space by projecting it onto a 2D plane and applying the ear-cutting
 * algorithm. The polygon is represented as a sequence of vertices, where
 * each vertex is a three-dimensional point. The function first calculates
 * the normal vector of the polygon and an orthonormal basis for the plane
 * containing the polygon. It then projects each vertex onto the plane and
 * triangulates the resulting 2D polygon using the ear-cutting algorithm.
 * The function returns a vector containing the indices of the vertices that
 * form triangles in the triangulated polygon. The indices are ordered in a
 * counter-clockwise direction, and each group of three indices represents
 * the vertices of a single triangle. The function requires that the type of
 * iterator used to represent the vertices of the polygon satisfies the
 * Point3Concept requirement, which means that it must have a value_type
 * that is a Point3 object with a ScalarType member representing the scalar
 * type used to represent the position of the point. If the polygon is
 * not simple or has holes, the function throws a std::logic_error.
 *
 * @ingroup core_polygon
 */
template<Point3IteratorConcept Iterator>
std::vector<uint> earCut(Iterator begin, Iterator end)
{
    // Project the 3D polygon onto a 2D plane defined by its normal.
    auto poly2D = project(begin, end);

    // Use the ear-cut algorithm to triangulate the polygon in the 2D plane
    // and return the result.
    return earCut(poly2D.begin(), poly2D.end());
}

/**
 * @copybrief earCut(Iterator, Iterator)
 *
 * @tparam R: a range of points that satisfy the PointConcept.
 * @param[in] range: the range of points that define the polygon.
 * @return A vector containing the indices of the vertices that form
 * triangles in the triangulated polygon.
 *
 * @ingroup core_polygon
 */
template<Range R>
std::vector<uint> earCut(R&& range)
{
    return earCut(std::ranges::begin(range), std::ranges::end(range));
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_POLYGON_EAR_CUT_H
