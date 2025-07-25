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

#ifndef VCL_SPACE_CORE_POLYGON_H
#define VCL_SPACE_CORE_POLYGON_H

#include "point.h"
#include "triangle.h"

#include <vclib/concepts.h>

#include <ranges>
#include <vector>

namespace vcl {

/**
 * @brief The Polygon class represents a polygon in a N-dimensional Euclidean
 * space.
 *
 * The class allows to store a sequence of points that define a polygon, and
 * provides a set of member functions to compute properties of the polygon, such
 * as the normal, the barycenter, the perimeter, the area... The class also
 * provides a series of static member functions to compute these properties for
 * a polygon defined by a range of points.
 *
 * @tparam PointT: The type of the points that define the polygon.
 *
 * @ingroup space_core
 */
template<PointConcept PointT>
class Polygon
{
    std::vector<PointT> mPoints;

public:
    /** @brief The scalar type of the points that define the polygon. */
    using ScalarType = PointT::ScalarType;

    /** @brief The type of the points that define the polygon. */
    using PointType = PointT;

    /** @brief The iterator type of the polygon. */
    using Iterator = typename std::vector<PointT>::iterator;

    /** @brief The const iterator type of the polygon. */
    using ConstIterator = typename std::vector<PointT>::const_iterator;

    /** @brief The dimension of the points that define the polygon. */
    static const uint DIM = PointT::DIM;

    /**
     * @brief Construct a new empty Polygon object.
     */
    Polygon() {}

    /**
     * @brief Construct a new Polygon object from an initializer list of points.
     *
     * @param[in] points: the list of points that define the polygon.
     */
    Polygon(std::initializer_list<PointT> points) : mPoints(points) {}

    /**
     * @brief Construct a new Polygon object from a range of points.
     *
     * @tparam Iterator: an iterator which points to a type that satisfies the
     * PointConcept.
     * @param[in] begin: iterator pointing to the first point of the polygon.
     * @param[in] end: end iterator.
     */
    template<typename Iterator>
    Polygon(Iterator begin, Iterator end)
        requires (std::is_same_v<typename Iterator::value_type, PointT>)
            : mPoints(begin, end)
    {
    }

    /**
     * @brief Construct a new Polygon object from a range of points.
     *
     * @tparam R: a range of points that satisfy the PointConcept.
     * @param[in] range: the range of points that define the polygon.
     */
    template<Range R>
    Polygon(R&& range) requires (InputRange<R, PointT>)
            : mPoints(std::ranges::begin(range), std::ranges::end(range))
    {
    }

    /**
     * @brief Returns the number of points that define the polygon.
     *
     * @return The number of points that define the polygon.
     */
    uint size() const { return mPoints.size(); }

    /**
     * @brief Resizes the polygon to contain n points.
     *
     * @param[in] n: the number of points that the polygon should contain.
     */
    void resize(uint n) { mPoints.resize(n); }

    /**
     * @brief Reserves space for n points in the polygon.
     *
     * @param[in] n: the number of points for which to reserve space.
     */
    void reserve(uint n) { mPoints.reserve(n); }

    /**
     * @brief Clears the polygon, removing all points.
     */
    void clear() { mPoints.clear(); }

    /**
     * @brief Adds a point to the back of the polygon.
     *
     * @param[in] point: the point to add to the polygon.
     */
    void pushBack(const PointT& point) { mPoints.push_back(point); }

    /**
     * @brief Adds a point to the back of the polygon.
     *
     * @param[in] point: the point to add to the polygon.
     */
    void pushBack(PointT&& point) { mPoints.push_back(std::move(point)); }

    /**
     * @brief Returns the point at index i in the polygon.
     *
     * @param[in] i: the index of the point to return.
     */
    PointT& point(uint i) { return mPoints.at(i); }

    /**
     * @brief Returns the point at index i in the polygon.
     *
     * @param[in] i: the index of the point to return.
     */
    const PointT& point(uint i) const { return mPoints.at(i); }

    /**
     * @brief Returns the side length of the i-th side of the polygon.
     *
     * The i-th side is the side that connects the i-th point with the (i+1)-th
     * point, where the indices are taken modulo the number of points in the
     * polygon.
     *
     * @param[in] i: the index of the side for which to compute the length.
     */
    ScalarType sideLength(uint i) const
    {
        return mPoints.at(i).dist(mPoints[(i + 1) % mPoints.size()]);
    }

    /**
     * @brief Computes the normal of the polygon.
     *
     * The normal is computed as the normalized sum of the cross products of
     * each triplet of consecutive points.
     *
     * @note This function requires that the dimension of the points that define
     * the polygon is 3.
     *
     * @return The normal of the polygon.
     */
    PointT normal() const requires (PointT::DIM == 3)
    {
        return normal(mPoints);
    }

    /**
     * @brief Computes the barycenter of the polygon.
     *
     * The barycenter is computed as the average of the points that define the
     * polygon.
     *
     * @return The barycenter of the polygon.
     */
    PointT barycenter() const { return barycenter(mPoints); }

    /**
     * @brief Computes the weighted barycenter of the polygon.
     *
     * The weighted barycenter is computed as the weighted average of the points
     * that define the polygon. The weights are provided by a container of the
     * same size as the polygon, iterated by the iterator wBegin.
     *
     * @tparam WIterator: an iterator which points to a scalar type.
     * @param[in] wBegin: iterator pointing to the first weight associated to
     * the fist polygon point.
     * @return The weighted barycenter of the polygon.
     */
    template<typename WIterator>
    PointT weightedBarycenter(WIterator wBegin) const
    {
        return weightedBarycenter(mPoints.begin(), mPoints.end(), wBegin);
    }

    /**
     * @brief Returns the perimeter of the polygon.
     *
     * @return The perimeter of the polygon.
     */
    ScalarType perimeter() const { return perimeter(mPoints); }

    /**
     * @brief Returns the area of the polygon.
     *
     * @return The area of the polygon.
     */
    ScalarType area() const { return area(mPoints); }

    /**
     * @brief Returns the i-th point of the polygon.
     *
     * @param i: the index of the point to return.
     * @return The i-th point of the polygon.
     */
    PointType& operator[](uint i) { return mPoints[i]; }

    /**
     * @brief Returns the i-th point of the polygon.
     *
     * @param i: the index of the point to return.
     * @return The i-th point of the polygon.
     */
    const PointType& operator[](uint i) const { return mPoints[i]; }

    /**
     * @brief Returns an iterator pointing to the first point of the polygon.
     */
    Iterator begin() { return mPoints.begin(); }

    /**
     * @brief Returns an iterator pointing past the last point of the polygon.
     */
    Iterator end() { return mPoints.end(); }

    /**
     * @brief Returns a const iterator pointing to the first point of the
     * polygon.
     */
    ConstIterator begin() const { return mPoints.begin(); }

    /**
     * @brief Returns a const iterator pointing past the last point of the
     * polygon.
     */
    ConstIterator end() const { return mPoints.end(); }

    // static member functions

    /**
     * @brief Computes the normal of a container of 3D points iterated between
     * the iterators begin and end, listed in counterclockwise order,
     * representing a polygon.
     *
     * The normal is computed as the normalized sum of the cross products of
     * each triplet of consecutive points.
     *
     * @tparam Iterator: an iterator which points to a type that satisfies the
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

    /**
     * @copybrief normal(Iterator, Iterator)
     *
     * @tparam R: a range of points that satisfy the PointConcept.
     * @param[in] range: the range of points that define the polygon.
     * @return The normal of the polygon.
     */
    template<Range R>
    static PointT normal(R&& range)
    {
        return normal(std::ranges::begin(range), std::ranges::end(range));
    }

    /**
     * @brief Computes the barycenter of a container of points iterated between
     * the iterators begin and end, listed in counterclockwise order,
     * representing a polygon.
     *
     * @tparam Iterator: an iterator which points to a type that satisfies the
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

    /**
     * @copybrief barycenter(Iterator, Iterator)
     *
     * @tparam R: a range of points that satisfy the PointConcept.
     * @param[in] range: the range of points that define the polygon.
     * @return The barycenter of the polygon.
     */
    template<Range R>
    static PointT barycenter(R&& range)
    {
        return barycenter(std::ranges::begin(range), std::ranges::end(range));
    }

    /**
     * @brief Computes the weighted barycenter of a container of 3D points
     * iterated between the iterators begin and end, listed in counterclockwise
     * order, representing a polygon. Weights are iterated by another iterator
     * wBegin, which iterates to a container of the same size of the polygon
     * container.
     *
     * @tparam Iterator: an iterator which points to a type that satisfies the
     * PointConcept.
     * @tparam WIterator: an iterator which points to a scalar type.
     *
     * @param[in] begin: iterator pointing to the first point of the polygon.
     * @param[in] end: end iterator of the polygon container.
     * @param[in] wBegin: iterator pointing to the first weight associated to
     * the fist polygon point.
     * @return The weighted barycenter of the polygon.
     */
    template<typename Iterator, typename WIterator>
    static PointT weightedBarycenter(
        Iterator  begin,
        Iterator  end,
        WIterator wBegin)
        requires (std::is_same_v<typename Iterator::value_type, PointT>)
    {
        using ScalarType = WIterator::value_type;

        PointT bar;
        bar.setZero();
        ScalarType wSum = 0;

        for (; begin != end; ++begin, ++wBegin) {
            bar += *begin * *wBegin;
            wSum += *wBegin;
        }

        return bar / wSum;
    }

    /**
     * @copybrief weightedBarycenter(Iterator, Iterator, WIterator)
     *
     * @tparam Rp: a range of points that satisfy the PointConcept.
     * @tparam Rw: a range of weights.
     * @param[in] rPolygon: the range of points that define the polygon.
     * @param[in] rWeights: the range of weights associated to the points.
     * @return The weighted barycenter of the polygon.
     */
    template<Range Rp, Range Rw>
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
     * The function calculates the perimeter of a polygon defined by a range of
     * points. The function takes two iterators as input, representing the
     * beginning and end of the range of points. The points in the range must
     * satisfy the PointConcept, which is a concept that requires the point type
     * to have a ScalarType and a dist() function that calculates the distance
     * between two points.
     *
     * @tparam Iterator: an iterator which points to a type that satisfies the
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

    /**
     * @copybrief perimeter(Iterator, Iterator)
     *
     * @tparam R: a range of points that satisfy the PointConcept.
     * @param[in] range: the range of points that define the polygon.
     * @return The perimeter of the polygon.
     */
    template<Range R>
    static ScalarType perimeter(R&& range)
    {
        return perimeter(std::ranges::begin(range), std::ranges::end(range));
    }

    /**
     * @brief Calculates the area of a polygon. This function works also with
     * simple triangles, but it is less efficient thant the function
     * "Triangle<PointT>::area()".
     *
     * @tparam Iterator: an iterator which points to a type that satisfies the
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
            ++i1;
            if (i1 == end)
                i1 = begin;
            const PointT& p1 = *i1;

            area += Triangle<PointT>::area(p0, p1, bar);
        }
        return area;
    }

    /**
     * @copybrief area(Iterator, Iterator)
     *
     * @tparam R: a range of points that satisfy the PointConcept.
     * @param[in] range: the range of points that define the polygon.
     * @return The area of the polygon.
     */
    template<Range R>
    static ScalarType area(R&& range)
    {
        return area(std::ranges::begin(range), std::ranges::end(range));
    }
};

/* Specialization Aliases */

/**
 * @brief A convenience alias for a 2D polygon.
 *
 * @tparam Scalar: the scalar type used to represent the coordinates of the
 * points that define the polygon.
 *
 * @ingroup space_core
 */
template<typename Scalar>
using Polygon2 = Polygon<Point2<Scalar>>;

/**
 * @brief A convenience alias for a 2D polygon with single precision floating
 * point coordinates.
 *
 * @ingroup space_core
 */
using Polygon2f = Polygon<Point2f>;

/**
 * @brief A convenience alias for a 2D polygon with double precision floating
 * point coordinates.
 *
 * @ingroup space_core
 */
using Polygon2d = Polygon<Point2d>;

/**
 * @brief A convenience alias for a 3D polygon.
 *
 * @tparam Scalar: the scalar type used to represent the coordinates of the
 * points that define the polygon.
 *
 * @ingroup space_core
 */
template<typename Scalar>
using Polygon3 = Polygon<Point3<Scalar>>;

/**
 * @brief A convenience alias for a 3D polygon with single precision floating
 * point coordinates.
 *
 * @ingroup space_core
 */
using Polygon3f = Polygon<Point3f>;

/**
 * @brief A convenience alias for a 3D polygon with double precision floating
 * point coordinates.
 *
 * @ingroup space_core
 */
using Polygon3d = Polygon<Point3d>;

} // namespace vcl

#endif // VCL_SPACE_CORE_POLYGON_H
