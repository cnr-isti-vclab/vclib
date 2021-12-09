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
#include <vclib/misc/comparators.h>

#include "update/normal.h"

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
 * @brief Computes the EarCut algorithm of a 2D polygon, that returns a triangulation of the
 * polygon.
 *
 * Returns a list of indices in which each index is the index of a point of the 2D input polgon,
 * organized in triplets, each one of these is a triangle of the resulting triangulation.
 *
 * @param[in] polygon: Container of 2D points forming a polygon.
 * @return A vector of indices, representing the triplets of the triangulation of the polygon.
 */
template<template<typename, typename...> typename Container, typename Scalar>
std::vector<uint> earCut(const Container<Point2<Scalar>>& polygon)
{
	std::vector<Container<Point2<Scalar>>> poly;
	poly.push_back(polygon);

	return mapbox::earcut<uint>(poly);
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
template<template<typename, typename...> typename Container, typename Scalar>
std::vector<uint> earCut(const Container<Point3<Scalar>>& polygon)
{
	Point3<Scalar> n = polygonNormal<Scalar, Point3<Scalar>>(polygon);
	Point3<Scalar> u, v;
	getOrthoBase(n, u, v);

	std::vector<Point2<Scalar>> poly2D(polygon.size());
	for (uint i = 0; i < polygon.size(); ++i){
		// project i-th polygon in a 2D plane
		poly2D[i] = Point2<Scalar>(polygon[i]*u, polygon[i]*v);
	}
	return earCut(poly2D);
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
	using VertexType = typename Polygon::VertexType;
	using CoordType = typename VertexType::CoordType;

	std::vector<CoordType> pol; pol.reserve(polygon.vertexNumber());
	for (const VertexType* v : polygon.vertices()){
		pol.push_back(v->coord());
	}
	return earCut(pol);
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
template <typename MeshType, typename FaceType, template <typename,typename...> typename Container>
void addTriangleFacesFromPolygon(MeshType& m, FaceType& f, const Container<uint>& polygon)
{
	vcl::requireVertices<MeshType>();
	vcl::requireFaces<MeshType>();

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
		unorderedEdges.insert(std::make_pair(i, (i+1) % polygon.size()));

	if constexpr (FaceType::VERTEX_NUMBER < 0)
		f.resizeVertices(3);

	// set the first triangle of the loaded polygon
	f.vertex(0) = &m.vertex(polygon[tris[0]]);
	f.vertex(1) = &m.vertex(polygon[tris[1]]);
	f.vertex(2) = &m.vertex(polygon[tris[2]]);

	if (unorderedEdges.find(std::make_pair(tris[0], tris[1])) == unorderedEdges.end())
		f.setEdgeFaux(0);
	if (unorderedEdges.find(std::make_pair(tris[1], tris[2])) == unorderedEdges.end())
		f.setEdgeFaux(1);
	if (unorderedEdges.find(std::make_pair(tris[2], tris[0])) == unorderedEdges.end())
		f.setEdgeFaux(2);

	// remaining triangles, need to create more faces in the mesh
	for (uint i = 3; i < tris.size(); i += 3) {
		uint ff              = m.addFace();

		if constexpr (FaceType::VERTEX_NUMBER < 0)
			m.face(ff).resizeVertices(3);

		m.face(ff).vertex(0) = &m.vertex(polygon[tris[i]]);
		m.face(ff).vertex(1) = &m.vertex(polygon[tris[i + 1]]);
		m.face(ff).vertex(2) = &m.vertex(polygon[tris[i + 2]]);

		if (unorderedEdges.find(std::make_pair(tris[i], tris[i+1])) == unorderedEdges.end())
			m.face(ff).setEdgeFaux(0);
		if (unorderedEdges.find(std::make_pair(tris[i+1], tris[i+2])) == unorderedEdges.end())
			m.face(ff).setEdgeFaux(1);
		if (unorderedEdges.find(std::make_pair(tris[i+2], tris[i+0])) == unorderedEdges.end())
			m.face(ff).setEdgeFaux(2);
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
template <typename MeshType, template <typename,typename...> typename Container>
uint addTriangleFacesFromPolygon(MeshType& m, const Container<uint>& polygon)
{
	uint fid = m.addFace();
	addTriangleFacesFromPolygon(m, m.face(fid), polygon);
	return fid;
}

} // namespace vcl
