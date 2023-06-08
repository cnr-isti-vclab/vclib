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

#include <vclib/exceptions/mesh_exceptions.h>
#include <vclib/iterators/mesh.h>
#include <vclib/mesh/utils/mesh_pos.h>
#include <vclib/views/mesh.h>
#include <vclib/misc/comparators.h>
#include <vclib/space/polygon.h>

namespace vcl {

/**
 * @brief Check if an edge in the given face is manifold.
 *
 * An edge is considered manifold if it is either a boundary edge or if it is shared by exactly two
 * faces.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to check.
 * @param[in] edge: The index of the edge to check.
 *
 * @return true if the edge is manifold, false otherwise.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is not enabled on \p f.
 */
template<FaceConcept FaceType>
bool isFaceManifoldOnEdge(const FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>
{
	// Check if the AdjacentFaces component is enabled for the given face.
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	// Check if the edge is a boundary edge.
	if (f.adjFace(edge) == nullptr) {
		return true;
	}
	else { // Check if the edge is shared by exactly two faces.
		return f.adjFace(edge)->indexOfAdjFace(&f) >= 0;
	}
}

/**
 * @brief Check if an edge in the given face is on the border.
 *
 * An edge is considered to be on the border if it is not shared by another face.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to check.
 * @param[in] edge: The index of the edge to check.
 *
 * @return true if the edge is on the border, false otherwise.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is not enabled on \p f.
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
 * @brief Returns a boolean value indicating whether the edge flip operation is allowed or not.
 *
 * This function requires AdjacentFaces component, that must be enabled and computed before calling
 * this function.
 *
 * The function first checks if the specified edge is a boundary edge, in which case the flip
 * operation is not allowed. If the edge is not a boundary edge, the function checks whether the
 * mesh is well-oriented by verifying that the vertices of the edge to be flipped are the same in
 * the adjacent face.
 *
 * Next, the function checks if the flipped edge already exists in the mesh. To do this, the
 * function performs a depth-first search starting from the current face, following the edges that
 * share the opposite vertex to the edge being flipped. The search checks if the vertex on the other
 * end of the flipped edge is already connected to any other face in the mesh. If it is, the flip
 * operation is not allowed.
 *
 * The depth-first search is limited to the faces that share the opposite vertex to the edge being
 * flipped, so it does not perform an exhaustive search of the entire mesh. However, it is
 * sufficient to detect non-manifoldness caused by the flipped edge.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face that contains the edge to flip.
 * @param[in] edge: The index of the edge to flip.
 *
 * @return true if the edge flip is allowed, false otherwise.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is not enabled on \p f.
 */
template<FaceConcept FaceType>
bool checkFlipEdge(const FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>
{
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	using VertexType = typename FaceType::VertexType;

	if (f.vertexNumber() > 3)
		return false;

	if (vcl::isFaceEdgeOnBorder(f, edge))
		return false;

	const VertexType* v0 = f.vertex(edge);
	const VertexType* v1 = f.vertexMod(edge+1);

	const FaceType* of = f.adjFace(edge);
	int oe = of->indexOfAdjFace(&f);
	assert(oe);

	// check if the vertices of the edge are the same
	// e.g. the mesh has to be well oriented
	if (of->vertex(oe) != v1 || of->vertexMod(oe+1) != v0)
		return false;

	// check if the flipped edge is already present in the mesh
	// f_v2 and of_v2 are the vertices of the new edge
	const VertexType* f_v2 = f.vertexMod(edge+2);
	const VertexType* of_v2 = of->vertexMod(oe+2);

	MeshPos<FaceType> pos(&f, f_v2);
	MeshPos<FaceType> startPos = pos;
	// loop in the one ring of f_v2
	do {
		pos.nextEdgeAdjacentToV();
		if (pos.adjVertex() == of_v2)
			return false;
	} while(pos != startPos);

	return true;
}

/**
 * @brief Returns the number of adjacent faces to the given edge of the face f.
 *
 * If the given edge is manifold, the returned number will be 1 (if the edge is on border - just one
 * face) or 2 (two adjacent faces on the edge). If the edge is non manifold, the number of faces
 * adjacent to the given face will be counted.
 *
 * This function requires AdjacentFaces component, that must be enabled and computed before calling
 * this function.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: the face containing the edge
 * @param[in] edge: the index of the edge of the face
 * @return the number of adjacent faces to the given edge of the face \p f
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is not enabled on \p f.
 */
template<FaceConcept FaceType>
uint edgeAdjacentFacesNumber(const FaceType& f, uint edge)
	requires comp::HasAdjacentFaces<FaceType>
{
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	ConstEdgeAdjFaceIterator<FaceType> begin(f, edge), end;
	uint cnt = 0;
	for (auto it = begin ; it != end; ++it) {
		++cnt;
	}

	return cnt;
}

/**
 * @brief Returns the number of edges that are on border (no adjacent faces) on the given face.
 *
 * This function requires AdjacentFaces component, that must be enabled and computed before calling
 * this function.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to check for border edges.
 * @return The number of edges on the border of the face.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is not enabled on \p f.
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
 * @brief Detaches the face \p f on the given edge, which consists on updating adjacent faces such
 * that any face that was linking the face \p f won't link it anymore. It manages also non-manifold
 * edges.
 *
 * If the given pair face-edge is on border, nothing is done.
 * If the given pair face-edge is a normal manifold edge, this operation will set nullptr as
 * adjacent face of f (making edge a border) and vice versa on the adjacent face.
 * If the given pair face-edge is a non-manifold edge, the function will "remove" the current face
 * from the ring of faces incident on the edge. The given face f will have the given edge set as a
 * border (nullptr).
 *
 * This function is designed to work with faces that have an enabled "AdjacentFaces" component.
 * If the component is not enabled, a MissingComponentException is thrown.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to detach on the given edge.
 * @param[in] edge: The index of the edge to detach the face from.
 *
 * @throws vcl::MissingComponentException If the "AdjacentFaces" component is not enabled on \p f.
 */
template <FaceConcept FaceType>
void detachAdjacentFacesOnEdge(FaceType& f, uint edge) requires comp::HasAdjacentFaces<FaceType>
{
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	FaceType* nextFace = f.adjFace(edge);

	// if nextFace == nullptr there is nothing to do
	// the face is already detached on the edge
	if (nextFace != nullptr) {

		FaceType* prevFace;
		ConstEdgeAdjFaceIterator<FaceType> begin(f, edge), end;
		for (auto it = begin ; it != end; ++it) {
			prevFace = *it;
		}

		if (nextFace == prevFace) { // manifold case
			int en = nextFace->indexOfAdjFace(&f);
			assert(en >= 0);
			nextFace->adjFace(en) = nullptr;
		}
		else { // non manifold case
			// the prev face does not have to point to f anymore, but to nextFace
			int pn = prevFace->indexOfAdjFace(&f);
			assert(pn >= 0);
			prevFace->adjFace(pn) = nextFace;
		}
		f.adjFace(edge) = nullptr;
	}
}

/**
 * @brief Detaches the given face from all its adjacent vertices and adjacent faces.
 *
 * The detachFace function detaches the given face from all its adjacent vertices and adjacent
 * faces. This means that every vertex and face will no longer link to the face \p f as an adjacent
 * face, and \p f will no longer have any adjacent faces (all their values are set to nullptr). The
 * vertices of the face \p f are unchanged.
 *
 * This function is designed to work with faces that have an enabled "Adjacent Faces" component.
 * If the component is not enabled, a MissingComponentException is thrown.
 *
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] f: The face to detach from its vertices and adjacent faces.
 *
 * @throws MissingComponentException if the adjacent faces component is not enabled on the face.
 */
template <FaceConcept FaceType>
void detachFace(FaceType& f) requires comp::HasAdjacentFaces<FaceType>
{
	if (! comp::isAdjacentFacesEnabledOn(f)) {
		throw vcl::MissingComponentException("Face has no Adjacent Faces component.");
	}

	using VertexType = typename FaceType::VertexType;

	for (uint e = 0; e < f.vertexNumber(); ++e) {
		detachAdjacentFacesOnEdge(f, e);

		// if the vertices have adjacent faces
		if constexpr (comp::HasAdjacentFaces<VertexType>) {
			if (comp::isAdjacentFacesEnabledOn(f.vertex(e))) {
				VertexType* v = f.vertex(e);
				int vpos = v->indexOfAdjFace(&f);
				if (vpos >= 0) { // may happen if vertex adj faces are not initialized / updated
						v->eraseAdjFace(vpos); // the vertex v has not anymore the adjacent face f
				}
			}
		}
	}
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
 * @tparam Face: the type of the face that satisfies the FaceConcept.
 *
 * @param[in] polygon: A (polygonal) face of a vcl::Mesh.
 *
 * @return A vector of indices, representing the triplets of the triangulation of the polygon.
 */
template <FaceConcept Face>
std::vector<uint> earCut(const Face& polygon)
{
	using CoordType = typename Face::VertexType::CoordType;
	return Polygon<CoordType>::earCut(polygon.vertices() | views::coords);
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
 * @tparam MeshType: the type of the face that satisfies the FaceMeshConcept.
 * @tparam FaceType: the type of the face that satisfies the FaceConcept.
 *
 * @param[in,out] m: the mesh on which add the triangulation of the polygon.
 * @param[in,out] f: the first face of the triangulation, that will be filled.
 * @param[in] polygon: the vertex indices in the mesh representing the polygon.
 */
template <FaceMeshConcept MeshType, FaceConcept FaceType>
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
	std::vector<uint> tris = Polygon<CoordType>::earCut(polCoords.begin(), polCoords.end());

	// faux edges management: create a set of unordered edges of the polygon
	// note: we use indices from 0 to polygon.size() because that are the output indices given by
	// the earcut algorithm
	std::set<std::pair<uint, uint>, UnorderedPairComparator<uint>> unorderedEdges;
	for (uint i = 0; i < polygon.size(); ++i)
		unorderedEdges.emplace(i, (i+1) % (uint)polygon.size());

	if constexpr (FaceType::VERTEX_NUMBER < 0) {
		f.resizeVertices(3);
	}

	// set the first triangle of the loaded polygon
	uint i = 0; // from 0 to 2, vertices indices of the triangle
	for (auto& v : f.vertices()) {
		v = &m.vertex(polygon[tris[i]]);
		++i;
	}

	if constexpr(face::HasFaceBitFlags<FaceType>) {
		if (unorderedEdges.find(std::make_pair(tris[0], tris[1])) == unorderedEdges.end())
			f.edgeFaux(0) = true;
		if (unorderedEdges.find(std::make_pair(tris[1], tris[2])) == unorderedEdges.end())
			f.edgeFaux(1) = true;
		if (unorderedEdges.find(std::make_pair(tris[2], tris[0])) == unorderedEdges.end())
			f.edgeFaux(2) = true;
	}

	// remaining triangles, need to create more faces in the mesh
	for (uint i = 3; i < tris.size(); i += 3) {
		uint ff              = m.addFace();

		if constexpr (FaceType::VERTEX_NUMBER < 0) {
			m.face(ff).resizeVertices(3);
		}

		uint j = 0; // from 0 to 2, vertices indices of the triangle
		for (auto& v : m.face(ff).vertices()) {
			v = &m.vertex(polygon[tris[i + j]]);
			++j;
		}

		if constexpr(face::HasFaceBitFlags<FaceType>) {
			if (unorderedEdges.find(std::make_pair(tris[i], tris[i+1])) == unorderedEdges.end())
				m.face(ff).edgeFaux(0) = true;
			if (unorderedEdges.find(std::make_pair(tris[i+1], tris[i+2])) == unorderedEdges.end())
				m.face(ff).edgeFaux(1) = true;
			if (unorderedEdges.find(std::make_pair(tris[i+2], tris[i+0])) == unorderedEdges.end())
				m.face(ff).edgeFaux(2) = true;
		}
	}
}

/**
 * @brief Given a vector of indices of vertices in a mesh representing a polyon, this function adds
 * N triangular faces to the mesh, that are the triangulation of the input polygon. Triangle edges
 * that are internal in the polygon are marked as faux. This function returns the index of the first
 * added triangle.
 *
 * @tparam MeshType: the type of the face that satisfies the FaceMeshConcept.
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
