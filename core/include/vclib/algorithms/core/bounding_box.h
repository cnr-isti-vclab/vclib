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

#ifndef VCL_ALGORITHMS_CORE_BOUNDING_BOX_H
#define VCL_ALGORITHMS_CORE_BOUNDING_BOX_H

#include <vclib/concepts/iterators.h>
#include <vclib/concepts/mesh/elements/edge.h>
#include <vclib/concepts/range.h>
#include <vclib/concepts/space/triangle.h>
#include <vclib/space/core/box.h>
#include <vclib/space/core/segment.h>
#include <vclib/space/core/sphere.h>

/**
 * @defgroup core_bounding_box Core Bounding Box Algorithms
 *
 * @ingroup algorithms_core
 *
 * @brief List of overloaded boundingBox functions that take in input an object
 * (or a Range of objects) and return its/their bounding box.
 *
 * You can access these algorithms by including
 * `#include <vclib/algorithms/core/bounding_box.h>`
 *
 * @note To compute the bounding box of a Mesh, use the `vcl::boundingBox`
 * function defined in `vclib/algorithms/mesh/bounding_box.h`.
 */

namespace vcl {

/**
 * @brief Compute the bounding box of a single point
 *
 * Given a point `p`, this function computes and returns the bounding box
 * of the point. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam PointType: The type of the point. It must satisfy the PointConcept.
 *
 * @param[in] p: The input point to compute the bounding box of
 * @return The bounding box of the input point
 *
 * @ingroup core_bounding_box
 */
template<PointConcept PointType>
auto boundingBox(const PointType& p)
{
    vcl::Box<PointType> b;
    b.add(p);
    return b;
}

/**
 * @brief Compute the bounding box of a line segment
 *
 * Given a line segment `s`, this function computes and returns the bounding
 * box of the segment. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam SegmentType: The type of the segment. It must satisfy the
 * SegmentConcept.
 *
 * @param[in] s: The input line segment to compute the bounding box of
 * @return The bounding box of the input line segment
 *
 * @ingroup core_bounding_box
 */
template<SegmentConcept SegmentType>
auto boundingBox(const SegmentType& s)
{
    using PointType = SegmentType::PointType;
    vcl::Box<PointType> b;

    b.add(s.p0());
    b.add(s.p1());

    return b;
}

/**
 * @brief Compute the bounding box of a sphere
 *
 * Given a sphere `s`, this function computes and returns the bounding box
 * of the sphere. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam SphereType: The type of the sphere. It must satisfy the
 * SphereConcept.
 *
 * @param[in] s: The input sphere to compute the bounding box of
 * @return The bounding box of the input sphere
 *
 * @ingroup core_bounding_box
 */
template<SphereConcept SphereType>
auto boundingBox(const SphereType& s)
{
    using ScalarType = SphereType::ScalarType;
    vcl::Box<Point3<ScalarType>> b;

    b.add(s.center() - s.radius());
    b.add(s.center() + s.radius());

    return b;
}

/**
 * @brief Compute the bounding box of a triangle
 *
 * Given a triangle `t`, this function computes and returns the bounding box
 * of the triangle. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam TriangleType: The type of the triangle.
 *
 * @param[in] t: The input triangle to compute the bounding box of
 * @return The bounding box of the input triangle
 *
 * @ingroup core_bounding_box
 */
template<ConstTriangleConcept TriangleType>
auto boundingBox(const TriangleType& t)
{
    using PointType = TriangleType::PointType;
    vcl::Box<PointType> b;

    b.add(t.point(0));
    b.add(t.point(1));
    b.add(t.point(2));

    return b;
}

/**
 * @brief Compute the bounding box of a vertex
 *
 * Given a vertex `v`, this function computes and returns the bounding
 * box of the vertex. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam VertexType: The type of the vertex. It must satisfy the
 * VertexConcept.
 *
 * @param[in] v: The input vertex to compute the bounding box of
 * @return The bounding box of the input vertex
 *
 * @ingroup core_bounding_box
 */
template<VertexConcept VertexType>
auto boundingBox(const VertexType& v)
{
    vcl::Box<typename VertexType::CoordType> b;
    b.add(v.coord());
    return b;
}

/**
 * @brief Compute the bounding box of a vertex pointer
 *
 * Given a pointer to a vertex `v`, this function computes and returns the
 * bounding box of the vertex. The bounding box is represented by a `vcl::Box`
 * object.
 *
 * @tparam VertexType: The type of the vertex. It must satisfy the
 * VertexConcept.
 *
 * @param[in] v: A pointer to the input vertex to compute the bounding box of
 * @return The bounding box of the input vertex
 *
 * @ingroup core_bounding_box
 */
template<VertexConcept VertexType>
auto boundingBox(const VertexType* v)
{
    vcl::Box<typename VertexType::CoordType> b;
    b.add(v->coord());
    return b;
}

/**
 * @brief Compute the bounding box of a face
 *
 * Given a face `f`, this function computes and returns the bounding box
 * of the face. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam FaceType: The type of the face. It must satisfy the FaceConcept.
 *
 * @param[in] f The input face to compute the bounding box of
 * @return The bounding box of the input face
 *
 * @ingroup core_bounding_box
 */
template<FaceConcept FaceType>
auto boundingBox(const FaceType& f)
{
    using VertexType = FaceType::VertexType;

    vcl::Box<typename VertexType::CoordType> b;
    for (const VertexType* v : f.vertices())
        b.add(v->coord());
    return b;
}

/**
 * @brief Compute the bounding box of a face pointer
 *
 * Given a pointer to a face `f`, this function computes and returns the
 * bounding box of the face. The bounding box is represented by a `vcl::Box`
 * object.
 *
 * @tparam FaceType: The type of the face. It must satisfy the FaceConcept.
 *
 * @param[in] f: A pointer to the input face to compute the bounding box of
 * @return The bounding box of the input face
 *
 * @ingroup core_bounding_box
 */
template<FaceConcept FaceType>
auto boundingBox(const FaceType* f)
{
    using VertexType = FaceType::VertexType;

    vcl::Box<typename VertexType::CoordType> b;
    for (const VertexType* v : f->vertices())
        b.add(v->coord());
    return b;
}

/**
 * @brief Compute the bounding box of an edge
 *
 * Given an edge `e`, this function computes and returns the bounding box
 * of the edge. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam EdgeType: The type of the edge. It must satisfy the EdgeConcept.
 *
 * @param[in] e: The input edge to compute the bounding box of
 * @return The bounding box of the input edge
 *
 * @ingroup core_bounding_box
 */
template<EdgeConcept EdgeType>
auto boundingBox(const EdgeType& e)
{
    using VertexType = EdgeType::VertexType;

    vcl::Box<typename VertexType::CoordType> b;
    for (const VertexType* v : e.vertices())
        b.add(v->coord());
    return b;
}

/**
 * @brief Compute the bounding box of an edge pointer
 *
 * Given a pointer to an edge `e`, this function computes and returns the
 * bounding box of the edge. The bounding box is represented by a `vcl::Box`
 * object.
 *
 * @tparam EdgeType: The type of the edge. It must satisfy the EdgeConcept.
 * @param[in] e: A pointer to the input edge to compute the bounding box of
 *
 * @return The bounding box of the input edge
 *
 * @ingroup core_bounding_box
 */
template<EdgeConcept EdgeType>
auto boundingBox(const EdgeType* e)
{
    using VertexType = EdgeType::VertexType;

    vcl::Box<typename VertexType::CoordType> b;
    for (const VertexType* v : e->vertices())
        b.add(v->coord());
    return b;
}

/**
 * @brief Calculates the bounding box of a set of objects.
 *
 * Given a set of objects iterated in the range [begin, end), computes the
 * bounding box by calling the proper overload of the @ref boundingBox function
 * for each object.
 *
 * If begin == end, the returned bounding box is invalid.
 *
 * @note A proper @ref boundingBox function overloaded on the iterated object
 * type must exist.
 *
 * @tparam Iterator: the type of the iterator over the objects. It must satisfy
 * the IteratorConcept.
 *
 * @param[in] begin: the iterator to the beginning of the range of objects.
 * @param[in] end: the iterator to the end of the range of objects.
 * @return the bounding box of the entire set of objects.
 *
 * @ingroup core_bounding_box
 */
template<IteratorConcept Iterator>
auto boundingBox(Iterator begin, Iterator end)
{
    using BB = decltype(boundingBox(typename Iterator::value_type()));

    BB b;

    for (; begin != end; ++begin)
        b.add(boundingBox(*begin));

    return b;
}

/**
 * @brief Calculates the bounding box of a range of objects.
 *
 * Given a set of objects iterated in the range, computes the bounding box by
 * calling the proper overload of the @ref boundingBox function for each object.
 *
 * If the range is empty, the returned bounding box is invalid.
 *
 * @note A proper @ref boundingBox function overloaded on the iterated object
 * type must exist.
 *
 * @tparam Rng: the type of the range of objects. It must satisfy the Range
 * concept.
 *
 * @param[in] r: the range of objects.
 * @return the bounding box of the entire range of objects.
 *
 * @ingroup core_bounding_box
 */
template<Range Rng>
auto boundingBox(Rng&& r)
{
    return boundingBox(std::ranges::begin(r), std::ranges::end(r));
}

} // namespace vcl

#endif // VCL_ALGORITHMS_CORE_BOUNDING_BOX_H
