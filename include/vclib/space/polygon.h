/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_SPACE_POLYGON_H
#define VCL_SPACE_POLYGON_H

#include <ranges>
#include <vector>

#if __has_include(<mapbox/earcut.hpp>)
#include <mapbox/earcut.hpp>
#else
// inclusion for usage of vclib without CMake - not ideal but necessary for
// header only
#include "../../../../external/earcut.hpp-2.2.3/include/mapbox/earcut.hpp"
#endif

#include <vclib/concepts/range.h>
#include <vclib/concepts/space/polygon.h>
#include <vclib/space/point.h>

#include "triangle.h"

namespace vcl {

template<PointConcept PointT>
class Polygon
{
    std::vector<PointT> p;

public:
    using ScalarType = PointT::ScalarType;
    using PointType  = PointT;

    static const uint DIM = PointT::DIM;

    Polygon() {}

    template<typename Iterator>
    Polygon(Iterator begin, Iterator end)
        requires (std::is_same_v<typename Iterator::value_type, PointT>)
            : p(begin, end)
    {
    }

    uint size() const { return p.size(); }

    PointT& point(uint i) { return p[i]; }

    const PointT& point(uint i) const { return p[i]; }

    ScalarType sideLength(uint i) const
    {
        return p[i].dist(p[(i + 1) % p.size()]);
    }

    PointT normal() const requires (PointT::DIM == 3) { return normal(p); }

    PointT barycenter() const { return barycenter(p); }

    template<typename WIterator>
    PointT weightedBarycenter(WIterator wbegin) const
    {
        return weightedBarycenter(p.begin(), p.end(), wbegin);
    }

    ScalarType perimeter() const { return perimeter(p); }

    ScalarType area() const { return area(p); }

    std::vector<uint> earCut() const
        requires (PointT::DIM == 2 || PointT::DIM == 3)
    {
        return earCut(p);
    }

    // static member functions

    /**
     * @brief Computes the normal of a container of 3D points iterated between
     * the iterators begin and end, listed in counterclockwise order,
     * representing a polygon.
     *
     * @tparam Iterator: an iterator which points to a type that satiesfies the
     * PointConcept.
     * @param[in] begin: iterator pointing to the first point of the polygon.
     * @param[in] end: end iterator
     * @return The normal of the polygon.
     */
    template<typename Iterator>
    static PointT normal(Iterator begin, Iterator end) requires (
        std::is_same_v<typename Iterator::value_type, PointT> &&
        PointT::DIM == 3)
    {
        // compute the sum of normals for each triplet of consecutive points
        PointT sum;
        sum.setZero();
        for (auto i = begin; i != end; ++i) {
            auto i1 = i;
            ++i1;
            if (i1 == end)
                i1 = begin;

            auto i2 = i1;
            ++i2;
            if (i2 == end)
                i2 = begin;

            sum += (*i1 - *i).cross(*i2 - *i);
        }
        sum.normalize();
        return sum;
    }

    template<vcl::Range R>
    static PointT normal(R&& range)
    {
        return normal(std::ranges::begin(range), std::ranges::end(range));
    }

    /**
     * @brief Computes the barycenter of a container of points iterated between
     * the iterators begin and end, listed in counterclockwise order,
     * representing a polygon.
     *
     * @tparam Iterator: an iterator which points to a type that satiesfies the
     * PointConcept.
     *
     * @param[in] begin: iterator pointing to the first point of the polygon.
     * @param[in] end: end iterator
     * @return The barycenter of the polygon.
     */
    template<typename Iterator>
    static PointT barycenter(Iterator begin, Iterator end)
        requires (std::is_same_v<typename Iterator::value_type, PointT>)
    {
        PointT bar;
        bar.setZero();

        uint cnt = 0;
        for (; begin != end; ++begin) {
            bar += *begin;
            ++cnt;
        }

        assert(cnt);

        return bar / cnt;
    }

    template<vcl::Range R>
    static PointT barycenter(R&& range)
    {
        return barycenter(std::ranges::begin(range), std::ranges::end(range));
    }

    /**
     * @brief Computes the weighted barycenter of a container of 3D points
     * iterated between the iterators begin and end, listed in counterclockwise
     * order, representing a polygon. Weights are iterated by another iterator
     * wbegin, which iterates to a container of the same size of the polygon
     * container.
     *
     * @tparam Iterator: an iterator which points to a type that satiesfies the
     * PointConcept.
     * @tparam WIterator: an iterator which points to a scalar type.
     *
     * @param[in] begin: iterator pointing to the first point of the polygon.
     * @param[in] end: end iterator of the polygon container.
     * @param[in] wbegin: iterator pointing to the first weight associated to
     * the fist polygon point.
     * @return The weighted barycenter of the polygon.
     */
    template<typename Iterator, typename WIterator>
    static PointT weightedBarycenter(
        Iterator  begin,
        Iterator  end,
        WIterator wbegin)
        requires (std::is_same_v<typename Iterator::value_type, PointT>)
    {
        using ScalarType = WIterator::value_type;

        PointT bar;
        bar.setZero();
        ScalarType wsum = 0;

        for (; begin != end; ++begin, ++wbegin) {
            bar += *begin * *wbegin;
            wsum += *wbegin;
        }

        return bar / wsum;
    }

    template<vcl::Range Rp, vcl::Range Rw>
    static PointT weightedBarycenter(Rp&& rPolygon, Rw&& rWeights)
    {
        return weightedBarycenter(
            std::ranges::begin(rPolygon),
            std::ranges::end(rPolygon),
            std::ranges::begin(rWeights));
    }

    /**
     * @brief Calculates the perimeter of a polygon defined by a range of
     * points.
     *
     * The polygonPerimeter function calculates the perimeter of a polygon
     * defined by a range of points. The function takes two iterators as input,
     * representing the beginning and end of the range of points. The points in
     * the range must satisfy the PointConcept, which is a concept that requires
     * the point type to have a ScalarType and a dist() function that calculates
     * the distance between two points.
     *
     * @tparam Iterator: an iterator which points to a type that satiesfies the
     * PointConcept.
     *
     * @param[in] begin: An iterator pointing to the first point in the range.
     * @param[in] end: An iterator pointing to one past the last point in the
     * range.
     * @returns The perimeter of the polygon defined by the range of points.
     */
    template<typename Iterator>
    static ScalarType perimeter(Iterator begin, Iterator end)
        requires (std::is_same_v<typename Iterator::value_type, PointT>)
    {
        using Scalar = PointType::ScalarType;

        Scalar per = 0;
        for (auto i = begin; i != end; ++i) {
            const PointT& p0 = *i;
            auto          i1 = i;
            ++i;
            if (i1 == end)
                i1 = begin;
            const PointT& p1 = *i1;
            per += p0.dist(p1);
        }
        return per;
    }

    template<vcl::Range R>
    static ScalarType perimeter(R&& range)
    {
        return perimeter(std::ranges::begin(range), std::ranges::end(range));
    }

    /**
     * @brief Calculates the area of a polygon. This function works also with
     * simple triangles, but it is less efficient thant the function
     * "Triangle<PointT>::area()".
     *
     * @tparam Iterator: an iterator which points to a type that satiesfies the
     * PointConcept.
     *
     * @param[in] begin: an iterator pointing to the first point in the polygon
     * @param[in] end: an iterator pointing past the last point in the polygon
     * @return the area of the polygon
     *
     * @note The points in the polygon must have a scalar type, and must satisfy
     * the PointConcept concept.
     * @note The polygon must be simple, i.e., it cannot intersect itself.
     */
    template<typename Iterator>
    static ScalarType area(Iterator begin, Iterator end)
        requires (std::is_same_v<typename Iterator::value_type, PointT>)
    {
        using Scalar = PointType::ScalarType;

        PointType bar  = barycenter(begin, end);
        Scalar    area = 0;
        for (auto i = begin; i != end; ++i) {
            const PointT& p0 = *i;
            auto          i1 = i;
            ++i;
            if (i1 == end)
                i1 = begin;
            const PointT& p1 = *i1;

            area += Triangle<PointT>::area(p0, p1, bar);
        }
        return area;
    }

    template<vcl::Range R>
    static ScalarType area(R&& range)
    {
        return area(std::ranges::begin(range), std::ranges::end(range));
    }

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
     * type used to represent the coordinates of the point. If the polygon is
     * not simple or has holes, the function throws a std::logic_error.
     */
    template<typename Iterator>
    static std::vector<uint> earCut(Iterator begin, Iterator end) requires (
        std::is_same_v<typename Iterator::value_type, PointT> &&
        PointT::DIM == 2)
    {
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
     * type used to represent the coordinates of the point. If the polygon is
     * not simple or has holes, the function throws a std::logic_error.
     */
    template<typename Iterator>
    static std::vector<uint> earCut(Iterator begin, Iterator end) requires (
        std::is_same_v<typename Iterator::value_type, PointT> &&
        PointT::DIM == 3)
    {
        using Scalar = PointType::ScalarType;

        // Calculate the normal vector of the polygon and an orthonormal basis
        // for the plane containing the polygon.
        PointType n = normal(begin, end);
        PointType u, v;
        n.orthoBase(u, v);

        // Project each vertex onto the plane defined by the orthonormal basis.
        std::vector<Point2<Scalar>> poly2D;

        if constexpr (std::is_same_v<
                          typename Iterator::iterator_category,
                          std::random_access_iterator_tag>)
        {
            poly2D.reserve(std::distance(begin, end));
        }

        for (auto i = begin; i != end; ++i) {
            // project i-th polygon in a 2D plane
            poly2D.emplace_back(*i * u, *i * v);
        }

        // Use the ear-cut algorithm to triangulate the polygon in the 2D plane
        // and return the result.
        return Polygon<Point2<Scalar>>::earCut(poly2D.begin(), poly2D.end());
    }

    template<vcl::Range R>
    static std::vector<uint> earCut(R&& range)
    {
        return earCut(std::ranges::begin(range), std::ranges::end(range));
    }
};

/* Specialization Aliases */

template<typename Scalar>
using Polygon2 = Polygon<Point2<Scalar>>;

using Polygon2f = Polygon<Point2f>;
using Polygon2d = Polygon<Point2d>;

template<typename Scalar>
using Polygon3 = Polygon<Point3<Scalar>>;

using Polygon3f = Polygon<Point3f>;
using Polygon3d = Polygon<Point3d>;

} // namespace vcl

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

#endif // VCL_SPACE_POLYGON_H
