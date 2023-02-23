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

#include "topology.h"

#include <set>

#include <vclib/exception/mesh_exception.h>
#include <vclib/mesh/mesh/mesh_algorithms.h>
#include <vclib/misc/comparators.h>

namespace vcl {

/**
 * @brief Returns true if the edge in the given face is manifold.
 *
 * This function requires AdjacentFaces component, that must be enabled and computed before calling
 * this function.
 */
template<FaceConcept FaceType>
bool isFaceManifoldOnEdge(const FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>
{
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	if (f.adjFace(edge) == nullptr) {
		return true;
	}
	else {
		return f.adjFace(edge)->indexOfAdjFace(&f) >= 0;
	}
}

/**
 * @brief Returns true if the edge in the given face is on border.
 *
 * This function requires AdjacentFaces component, that must be enabled and computed before calling
 * this function.
 */
template<FaceConcept FaceType>
bool isFaceEdgeOnBorder(const FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>
{
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	return f->adjFace(edge) == nullptr;
}

/**
 * @brief Returns the number of adjacent faces to the given face f on the given edge.
 *
 * If the given edge is manifold, the returned number will be 0 (if the edge is on border) or 1
 * (there is only one adjacent face). If the edge is non manifold, the number of faces adjacent to
 * the given face will be counted.
 */
template<FaceConcept FaceType>
uint adjacentFacesNumberOnEdge(const FaceType& f, uint edge)
	requires comp::HasAdjacentFaces<FaceType>
{
	using VertexType = typename FaceType::VertexType;

	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	if (isFaceEdgeOnBorder(f, edge))
		return 0;
	if (isFaceManifoldOnEdge(f, edge))
		return 1;

	const VertexType* v0 = f.vertex(edge);
	const VertexType* v1 = f.vertexMod(edge+1);

	uint cnt = 0;

	const FaceType* ff = f.adjFace(edge);
	int e = ff->indexOfEdge(v0, v1);

	do {
		cnt++;

		ff = ff->adjFace(e);
		e = ff->indexOfEdge(v0, v1);
		assert(e != -1);
	} while(ff != &f);

	return cnt;
}

/**
 * @brief Returns the number of edges that are on border (no adjacent faces) on the given faces.
 *
 * This function requires AdjacentFaces component, that must be enabled and computed before calling
 * this function.
 */
template <FaceConcept FaceType>
uint faceEdgesOnBorderNumber(const FaceType& f) requires comp::HasAdjacentFaces<FaceType>
{
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	uint cnt = 0;
	for (uint i = 0; i < f.vertexNumber(); ++i)
		if (isFaceEdgeOnBorder(f, i))
			cnt++;

	return cnt;
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
template <FaceConcept Face>
std::vector<uint> earCut(const Face& polygon)
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
 * @param[in,out] m: the mesh on which add the triangulation of the polygon.
 * @param[in,out] f: the first face of the triangulation, that will be filled.
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
		if constexpr(HasHalfEdges<MeshType>) {
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
			if constexpr( HasHalfEdges<MeshType>) {
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
 * @param[in,out] m: the mesh on which add the triangulation of the polygon.
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
