/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "polygon.h"

#include <set>

#include <mapbox/earcut.hpp>
#include <vclib/exception/mesh_exception.h>
#include <vclib/mesh/mesh_algorithms.h>
#include <vclib/misc/comparators.h>

// structs to make working the mapbox earcut algorithm on vcl::Point2
namespace mapbox {
namespace util {

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

} // namespace util
} // namespace mapbox

namespace vcl {

/**
 * @brief Computes the normal of the triangle composed by the points p0, p1, and p2, considering
 * that these three points are ordered in counterclockwise order.
 *
 * @param[in] p0: first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @return The normal of the triangle composed by p0, p1 and p2.
 */
template<typename PointType>
PointType triangleNormal(const PointType& p0, const PointType& p1, const PointType& p2)
{
	return (p1 - p0).cross(p2 - p0);
}

/**
 * @brief Computes the normal of a Face that is a triangle. Does not modify the triangle.
 * This function could accept also polygonal faces, but it will take into account only the first 3
 * vertices of the polygon. In this case, the resulting normal could be flipped w.r.yt. the actual
 * normal of the polygon.
 *
 * @param[in] t: input triangle of type Face
 * @return The normal of t.
 */
template<typename Triangle, typename NormalType>
NormalType triangleNormal(const Triangle& t)
{
	return triangleNormal(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the barycenter of the triangle composed by the points p0, p1, and p2.
 *
 * @param[in] p0: first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @return The barycenter of the triangle composed by p0, p1 and p2.
 */
template <typename PointType>
PointType triangleBarycenter(const PointType& p0, const PointType& p1, const PointType& p2)
{
	return (p0 + p1 + p2) / 3;
}

/**
 * @brief Computes the barycenter of a Face that is a triangle. This function could accept also
 * polygonal faces, but it will take into account only the first 3 vertices of the polygon. In this
 * case, the resulting barycenter won't be accurate.
 *
 * @param[in] t: input triangle of type Face
 * @return The barycenter of t.
 */
template<typename Triangle, typename PointType>
PointType triangleBarycenter(const Triangle& t)
{
	return triangleBarycenter(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the weighted barycenter of the triangle composed by the points p0, p1, and p2.
 *
 * @param[in] p0: first point of the triangle.
 * @param[in] w0: weight of the first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] w1: weight of the second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @param[in] w2: weight of the third point of the triangle.
 * @return The weighted barycenter of the triangle composed by p0, p1 and p2.
 */
template <typename PointType>
PointType triangleWeightedBarycenter(
	const PointType& p0, typename PointType::ScalarType w0,
	const PointType& p1, typename PointType::ScalarType w1,
	const PointType& p2, typename PointType::ScalarType w2)
{
	return (p0 * w0 + p1 * w1 + p2 * w2) / (w0 + w1 + w2);
}

/**
 * @brief Computes the area of the triangle composed by the points p0, p1, and p2, considering
 * that these three points are ordered in counterclockwise order.
 *
 * @param[in] p0: first point of the triangle.
 * @param[in] p1: second point of the triangle.
 * @param[in] p2: third point of the triangle.
 * @return The area of the triangle composed by p0, p1 and p2.
 */
template<typename PointType>
typename PointType::ScalarType
triangleArea(const PointType& p0, const PointType& p1, const PointType& p2)
{
	return triangleNormal(p0, p1, p2).norm() / 2;
}

/**
 * @brief Computes the area of a Face that is a triangle. This function could accept also
 * polygonal faces, but it will take into account only the first 3 vertices of the polygon. In this
 * case, the resulting area won't be accurate.
 *
 * @param[in] t: input triangle of type Face
 * @return The area of t.
 */
template<typename Triangle, typename ScalarType>
ScalarType triangleArea(const Triangle& t)
{
	return triangleArea(t.vertex(0)->coord(), t.vertex(1)->coord(), t.vertex(2)->coord());
}

/**
 * @brief Computes the normal of a std::vector of 3D points listed in counterclockwise order,
 * representing a polygon.
 *
 * @param[in] p: input container of 3D points representing a polygon.
 * @return The normal of p.
 */
template<typename PointType>
PointType polygonNormal(const std::vector<PointType>& p)
{
	// compute the sum of normals for each triplet of consecutive points
	PointType sum;
	sum.setZero();
	for (uint i = 0; i < p.size(); ++i) {
		sum += triangleNormal(
			p[i], p[(i+1)%p.size()], p[(i+2)%p.size()]);
	}
	sum.normalize();
	return sum;
}

/**
 * @brief Computes the normal of a Face that is a generic polygon. Does not modify the
 * polygon. This function works also with simple triangles, but it is less efficient thant the
 * function "triangleNormal".
 *
 * @param[in] p: input polygonal Face
 * @return The normal of p.
 */
template<typename Polygon, typename NormalType>
NormalType polygonNormal(const Polygon& p)
{
	// compute the sum of normals for each triplet of consecutive points
	NormalType sum;
	sum.setZero();
	for (uint i = 0; i < p.vertexNumber(); ++i) {
		sum += triangleNormal(
			p.vertexMod(i)->coord(), p.vertexMod(i + 1)->coord(), p.vertexMod(i + 2)->coord());
	}
	sum.normalize();
	return sum;
}

/**
 * @brief Computes the barycenter of a std::vector of points representing a polygon.
 *
 * @param[in] p: input container of points representing a polygon.
 * @return The barycenter of p.
 */
template<typename PointType>
PointType polygonBarycenter(const std::vector<PointType>& p)
{
	PointType bar;
	bar.setZero();

	for (const PointType& pp : p)
		bar += pp;
	return bar / p.size();
}

/**
 * @brief Computes the barycenter of a Face that is a generic polygon. This function works also with
 * simple triangles, but it is less efficient thant the function "triangleBarycenter".
 *
 * @param[in] p: input polygonal Face
 * @return The barycenter of p.
 */
template<typename Polygon, typename PointType>
PointType polygonBarycenter(const Polygon& p)
{
	PointType bar;
	bar.setZero();

	for (uint i = 0; i < p.vertexNumber(); ++i)
		bar += p.vertex(i)->coord();
	return bar / p.vertexNumber();
}

/**
 * @brief Computes the weighted barycenter of a std::vector of points representing a polygon.
 *
 * @param[in] p: input container of points representing a polygon.
 * @param[in] w: the weights for each point of the polygon.
 * @return The weighted barycenter of p.
 */
template<typename PointType>
PointType polygonWeighedBarycenter(
	const std::vector<PointType>&                      p,
	const std::vector<typename PointType::ScalarType>& w)
{
	PointType bar;
	bar.setZero();
	typename PointType::ScalarType wsum = 0;

	for (uint i = 0; i < p.size(); ++i){
		bar += p[i] * w[i];
		wsum += w[i];
	}

	return bar / wsum;
}

/**
 * @brief Computes the area of a std::vector of 3D points listed in counterclockwise order,
 * representing a polygon.
 *
 * @param[in] p: input container of 3D points representing a polygon.
 * @return The area of p.
 */
template<typename PointType>
typename PointType::Scalar polygonArea(const std::vector<PointType>& p)
{
	using Scalar = typename PointType::ScalarType;
	PointType bar = polygonBarycenter(p);
	Scalar area = 0;
	for (uint i = 0; i < p.size(); ++i){
		const PointType& p0 = p[i];
		const PointType& p1 = p[(i+1)%p.size()];
		area += triangleArea(p0, p1, bar);
	}
	return area;
}

/**
 * @brief Computes the area of a Face that is a generic polygon. This function works also with
 * simple triangles, but it is less efficient thant the function "triangleArea".
 *
 * @param[in] p: input polygonal Face
 * @return The area of p.
 */
template<typename Polygon, typename ScalarType>
ScalarType polygonArea(const Polygon& p)
{
	using PointType = typename Polygon::VertexType::CoordType;

	PointType bar = polygonBarycenter(p);
	ScalarType area = 0;
	for (uint i = 0; i < p.vertexNumber(); ++i){
		const PointType& p0 = p.vertex(i)->coord();
		const PointType& p1 = p.vertexMod(i+1)->coord();
		area += triangleArea(p0, p1, bar);
	}
	return area;
}

/**
 * @brief Computes the EarCut algorithm of a 2D polygon, that returns a triangulation of the
 * polygon.
 *
 * Returns a list of indices in which each index is the index of a point of the 2D input polgon,
 * organized in triplets, each one of these is a triangle of the resulting triangulation.
 *
 * @param[in] polygon: Container of 2D points forming a polygon.
 * @return A vector of indices, representing the triplets of the triangulation of the polygon.
 */
template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point2<Scalar>>& polygon)
{
	return mesh::earCut(polygon);
}

/**
 * @brief Computes the earcut algorithm of a 3D *planar* polygon, that returns a triangulation of
 * the polygon.
 *
 * Returns a list of indices in which each index is the index of a point of the 3D input polgon,
 * organized in triplets, each one of these is a triangle of the resulting triangulation.
 *
 * This algorithm first computes the normal of the given polygon, then projects it in a 2D plane
 * and executes the classic 2D EarCut algorithm.
 *
 * @param[in] polygon: Container of 3D points forming a polygon.
 * @return A vector of indices, representing the triplets of the triangulation of the polygon.
 */
template<typename Scalar>
std::vector<uint> earCut(const std::vector<Point3<Scalar>>& polygon)
{
	return mesh::earCut(polygon);
}

/**
 * @brief Computes the earcut algorithm of a 3D *planar* polygon, that returns a triangulation of
 * the polygon.
 *
 * Returns a list of indices in which each index is the index of a point of the 3D input polgon,
 * organized in triplets, each one of these is a triangle of the resulting triangulation.
 *
 * This algorithm first computes the normal of the given polygon, then projects it in a 2D plane
 * and executes the classic 2D EarCut algorithm.
 *
 * @param[in] polygon: A (polygonal) face of a vcl::Mesh.
 * @return A vector of indices, representing the triplets of the triangulation of the polygon.
 */
template <typename Polygon>
std::vector<uint> earCut(const Polygon& polygon)
{
	return mesh::earCut(polygon);
}

/**
 * @brief Given a vector of indices of vertices in a mesh representing a polyon, this function adds
 * N triangular faces to the mesh, that are the triangulation of the input polygon. Triangle edges
 * that are internal in the polygon are marked as faux.
 *
 * This function assumes that the first (triangular) face has been already added to the mesh and
 * just needs to be filled with vertex references. This is useful in some cases (e.g. when reading
 * from file and you realize just at some point that you need to manage a polygon). In all the other
 * cases, you should use the function that does not take a face in input, that is:
 *
 * @code{.cpp}
 * uint fid = addTriangleFacesFromPolygon(mesh, polygon);
 * @endcode
 *
 * @param[in/out] m: the mesh on which add the triangulation of the polygon.
 * @param[in/out] f: the first face of the triangulation, that will be filled.
 * @param[in] polygon: the vertex indices in the mesh representing the polygon.
 */
template <FaceMeshConcept MeshType, typename FaceType>
void addTriangleFacesFromPolygon(MeshType& m, FaceType& f, const std::vector<uint>& polygon)
{
	using VertexType = typename MeshType::VertexType;
	using CoordType = typename VertexType::CoordType;

	// from the ids, create a polygon of coordinates
	std::vector<CoordType> polCoords(polygon.size());
	for (uint i = 0; i < polygon.size(); ++i) {
		if (polygon[i] >= m.vertexContainerSize()){
			throw BadVertexIndexException(std::to_string(polygon[i]));
		}
		polCoords[i] = m.vertex(polygon[i]).coord();
	}

	// compute earcut of the polygons
	std::vector<uint> tris = earCut(polCoords);

	// faux edges management: create a set of unordered edges of the polygon
	// note: we use indices from 0 to polygon.size() because that are the output indices given by
	// the earcut algorithm
	std::set<std::pair<uint, uint>, UnorderedPairComparator<uint>> unorderedEdges;
	for (uint i = 0; i < polygon.size(); ++i)
		unorderedEdges.insert(std::make_pair(i, (i+1) % (uint)polygon.size()));

	if constexpr (FaceType::VERTEX_NUMBER < 0) {
		if constexpr( mesh::hasHalfEdges<MeshType>()) {
			m.addHalfEdgesToFace(3, f);
		}
		else {
			f.resizeVertices(3);
		}
	}

	// set the first triangle of the loaded polygon
	uint i = 0; // from 0 to 2, vertices indices of the triangle
	for (auto& v : f.vertices()) {
		v = &m.vertex(polygon[tris[i]]);
		++i;
	}

	if constexpr(face::HasFaceBitFlags<FaceType>) {
		if (unorderedEdges.find(std::make_pair(tris[0], tris[1])) == unorderedEdges.end())
			f.setEdgeFaux(0);
		if (unorderedEdges.find(std::make_pair(tris[1], tris[2])) == unorderedEdges.end())
			f.setEdgeFaux(1);
		if (unorderedEdges.find(std::make_pair(tris[2], tris[0])) == unorderedEdges.end())
			f.setEdgeFaux(2);
	}

	// remaining triangles, need to create more faces in the mesh
	for (uint i = 3; i < tris.size(); i += 3) {
		uint ff              = m.addFace();

		if constexpr (FaceType::VERTEX_NUMBER < 0) {
			if constexpr( mesh::hasHalfEdges<MeshType>()) {
				m.addHalfEdgesToFace(3, f);
			}
			else {
				m.face(ff).resizeVertices(3);
			}
		}

		uint j = 0; // from 0 to 2, vertices indices of the triangle
		for (auto& v : m.face(ff).vertices()) {
			v = &m.vertex(polygon[tris[i + j]]);
			++j;
		}

		if constexpr(face::HasFaceBitFlags<FaceType>) {
			if (unorderedEdges.find(std::make_pair(tris[i], tris[i+1])) == unorderedEdges.end())
				m.face(ff).setEdgeFaux(0);
			if (unorderedEdges.find(std::make_pair(tris[i+1], tris[i+2])) == unorderedEdges.end())
				m.face(ff).setEdgeFaux(1);
			if (unorderedEdges.find(std::make_pair(tris[i+2], tris[i+0])) == unorderedEdges.end())
				m.face(ff).setEdgeFaux(2);
		}
	}
}

/**
 * @brief Given a vector of indices of vertices in a mesh representing a polyon, this function adds
 * N triangular faces to the mesh, that are the triangulation of the input polygon. Triangle edges
 * that are internal in the polygon are marked as faux. This function returns the index of the first
 * added triangle.
 *
 * @param[in/out] m: the mesh on which add the triangulation of the polygon.
 * @param[in] polygon: the vertex indices in the mesh representing the polygon.
 * @return The index of the first triangle added to the mesh.
 */
template <FaceMeshConcept MeshType>
uint addTriangleFacesFromPolygon(MeshType& m, const std::vector<uint>& polygon)
{
	uint fid = m.addFace();
	addTriangleFacesFromPolygon(m, m.face(fid), polygon);
	return fid;
}

} // namespace vcl
