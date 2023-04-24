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

#include "bounding_box.h"

#include <vclib/space/box.h>

#include <vclib/concepts/iterators.h>

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
 * @ingroup bounding_box
 */
template<PointConcept PointType>
auto boundingBox(const PointType &p)
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
 * @tparam SegmentType: The type of the segment. It must satisfy the SegmentConcept.
 *
 * @param[in] s: The input line segment to compute the bounding box of
 * @return The bounding box of the input line segment
 *
 * @ingroup bounding_box
 */
template<SegmentConcept SegmentType>
auto boundingBox(const SegmentType& s)
{
	using PointType = typename SegmentType::PointType;
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
 * @tparam SphereType: The type of the sphere. It must satisfy the SphereConcept.
 *
 * @param[in] s: The input sphere to compute the bounding box of
 * @return The bounding box of the input sphere
 *
 * @ingroup bounding_box
 */
template<SphereConcept SphereType>
auto boundingBox(const SphereType& s)
{
	using ScalarType = typename SphereType::ScalarType;
	vcl::Box<Point3<ScalarType>> b;

	b.add(s.center() - s.radius());
	b.add(s.center() + s.radius());

	return b;
}

/**
 * @brief Compute the bounding box of a mesh
 *
 * Given a mesh `m`, this function computes and returns the bounding
 * box of the mesh. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam MeshType: The type of the mesh. It must satisfy the MeshConcept.
 *
 * @param[in] m: The input mesh to compute the bounding box of
 * @return The bounding box of the input mesh
 *
 * @ingroup bounding_box
 */
template<MeshConcept MeshType>
auto boundingBox(const MeshType& m)
{
	using VertexType = typename MeshType::VertexType;
	vcl::Box<typename VertexType::CoordType> b;

	for (const VertexType& v : m.vertices()) {
		b.add(v.coord());
	}

	return b;
}

/**
 * @brief Compute the bounding box of a vertex
 *
 * Given a vertex `v`, this function computes and returns the bounding
 * box of the vertex. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam VertexType: The type of the vertex. It must satisfy the VertexConcept.
 *
 * @param[in] v: The input vertex to compute the bounding box of
 * @return The bounding box of the input vertex
 *
 * @ingroup bounding_box
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
 * Given a pointer to a vertex `v`, this function computes and returns the bounding
 * box of the vertex. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam VertexType: The type of the vertex. It must satisfy the VertexConcept.
 *
 * @param[in] v: A pointer to the input vertex to compute the bounding box of
 * @return The bounding box of the input vertex
 *
 * @ingroup bounding_box
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
 * @ingroup bounding_box
 */
template<FaceConcept FaceType>
auto boundingBox(const FaceType& f)
{
	using VertexType = typename FaceType::VertexType;

	vcl::Box<typename VertexType::CoordType> b;
	for (const VertexType* v : f.vertices())
		b.add(v->coord());
	return b;
}

/**
 * @brief Compute the bounding box of a face pointer
 *
 * Given a pointer to a face `f`, this function computes and returns the bounding
 * box of the face. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam FaceType: The type of the face. It must satisfy the FaceConcept.
 *
 * @param[in] f: A pointer to the input face to compute the bounding box of
 * @return The bounding box of the input face
 *
 * @ingroup bounding_box
 */
template<FaceConcept FaceType>
auto boundingBox(const FaceType* f)
{
	using VertexType = typename FaceType::VertexType;

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
 * @ingroup bounding_box
 */
template<EdgeConcept EdgeType>
auto boundingBox(const EdgeType& e)
{
	using VertexType = typename EdgeType::VertexType;

	vcl::Box<typename VertexType::CoordType> b;
	for (const VertexType* v : e.vertices())
		b.add(v->coord());
	return b;
}

/**
 * @brief Compute the bounding box of an edge pointer
 *
 * Given a pointer to an edge `e`, this function computes and returns the bounding
 * box of the edge. The bounding box is represented by a `vcl::Box` object.
 *
 * @tparam EdgeType: The type of the edge. It must satisfy the EdgeConcept.
 * @param[in] e: A pointer to the input edge to compute the bounding box of
 *
 * @return The bounding box of the input edge
 *
 * @ingroup bounding_box
 */
template<EdgeConcept EdgeType>
auto boundingBox(const EdgeType* e)
{
	using VertexType = typename EdgeType::VertexType;

	vcl::Box<typename VertexType::CoordType> b;
	for (const VertexType* v : e->vertices())
		b.add(v->coord());
	return b;
}

/**
 * @brief Calculates the bounding box of a set of objects.
 *
 * Given a set of objects iterated in the range [begin, end), computes the bounding box by calling
 * the proper overload of the @ref boundingBox function for each object.
 *
 * If begin == end, the returned bounding box is invalid.
 *
 * @note A proper @ref boundingBox function overloaded on the iterated object type must exist.
 *
 * @tparam Iterator: the type of the iterator over the objects. It must satisfy the IteratorConcept.
 *
 * @param[in] begin: the iterator to the beginning of the range of objects.
 * @param[in] end: the iterator to the end of the range of objects.
 * @return the bounding box of the entire set of objects.
 *
 * @ingroup bounding_box
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
 * Given a set of objects iterated in the range, computes the bounding box by calling
 * the proper overload of the @ref boundingBox function for each object.
 *
 * If the range is empty, the returned bounding box is invalid.
 *
 * @note A proper @ref boundingBox function overloaded on the iterated object type must exist.
 *
 * @tparam Rng: the type of the range of objects. It must satisfy the Range concept.
 *
 * @param[in] r: the range of objects.
 * @return the bounding box of the entire range of objects.
 *
 * @ingroup bounding_box
 */
template<Range Rng>
auto boundingBox(Rng&& r)
{
	return boundingBox(std::ranges::begin(r), std::ranges::end(r));
}

} // namespace vcl
