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

#ifndef VCL_MESH_COMPONENTS_FACE_HALF_EDGE_REFERENCE_H
#define VCL_MESH_COMPONENTS_FACE_HALF_EDGE_REFERENCE_H

#include <vector>

#include <vclib/iterators/half_edge/face_adj_face_iterator.h>
#include <vclib/iterators/half_edge/face_half_edge_iterator.h>
#include <vclib/iterators/half_edge/face_vertex_iterator.h>
#include <vclib/iterators/range_iterator.h>

namespace vcl::comp {

template<typename HalfEdge>
class FaceHalfEdgeReference
{
	using Vertex = typename HalfEdge::VertexType;
	using Face   = typename HalfEdge::FaceType;

public:
	using HalfEdgeType = HalfEdge;
	using VertexType   = typename HalfEdge::VertexType;

	/* Iterator Types declaration */

	using AdjacentFaceIterator      = vcl::FaceAdjFaceIterator<HalfEdge>;
	using ConstAdjacentFaceIterator = vcl::ConstFaceAdjFaceIterator<HalfEdge>;
	using AdjacentFaceRangeIterator = RangeIterator<FaceHalfEdgeReference, AdjacentFaceIterator>;
	using ConstAdjacentFaceRangeIterator =
		ConstRangeIterator<FaceHalfEdgeReference, ConstAdjacentFaceIterator>;

	using InnerHalfEdgeIterator      = typename std::vector<HalfEdge*>::iterator;
	using ConstInnerHalfEdgeIterator = typename std::vector<HalfEdge*>::const_iterator;
	using InnerHalfEdgeRangeIterator = RangeIterator<FaceHalfEdgeReference, InnerHalfEdgeIterator>;
	using ConstInnerHalfEdgeRangeIterator =
		RangeIterator<FaceHalfEdgeReference, ConstInnerHalfEdgeIterator>;

	using HalfEdgeIterator      = vcl::FaceHalfEdgeIterator<HalfEdge>;
	using ConstHalfEdgeIterator = vcl::ConstFaceHalfEdgeIterator<HalfEdge>;
	using HalfEdgeRangeIterator = RangeIterator<FaceHalfEdgeReference, HalfEdgeIterator>;
	using ConstHalfEdgeRangeIterator =
		ConstRangeIterator<FaceHalfEdgeReference, ConstHalfEdgeIterator>;

	using VertexIterator           = vcl::FaceVertexIterator<HalfEdge>;
	using ConstVertexIterator      = vcl::ConstFaceVertexIterator<HalfEdge>;
	using VertexRangeIterator      = RangeIterator<FaceHalfEdgeReference, VertexIterator>;
	using ConstVertexRangeIterator = ConstRangeIterator<FaceHalfEdgeReference, ConstVertexIterator>;

	// Vertex references can be accessed from a face using half edge reference, therefore this
	// component claims that it is the VertexReferences component. This is done just for
	// compatibility between mesh types.
	using VertexReferences         = FaceHalfEdgeReference;
	static const int VERTEX_NUMBER = -1; // half edges support by design polygonal meshes
	static const int ADJ_FACE_NUMBER = -1;

	/* Constructor */

	FaceHalfEdgeReference();

	/* Member functions */

	const HalfEdge* outerHalfEdge() const;
	HalfEdge*&      outerHalfEdge();

	uint            numberHoles() const;
	const HalfEdge* innerHalfEdge(uint i) const;
	HalfEdge*&      innerHalfEdge(uint i);

	void resizeInnerHalfEdges(uint n);
	void pushInnerHalfEdge(HalfEdge* he);
	void insertInnerHalfEdge(uint i, HalfEdge* he);
	void eraseInnerHalfEdge(uint i);
	void clearInnerHalfEdges();

	/* VertexReferences compatibility */

	uint vertexNumber() const;

	Vertex*&      vertex(uint i);
	const Vertex* vertex(uint i) const;
	Vertex*&      vertexMod(int i);
	const Vertex* vertexMod(int i) const;

	void setVertex(Vertex* v, uint i);
	void setVertices(const std::vector<Vertex*>& list);

	bool containsVertex(const Vertex* v) const;

	VertexIterator      findVertex(const Vertex* v);
	ConstVertexIterator findVertex(const Vertex* v) const;

	int indexOfVertex(const Vertex* v) const;
	int indexOfEdge(const Vertex* v1, const Vertex* v2) const;

	/* AdjacentFaces compatibility */

	uint adjFacesNumber() const;

	Face*&      adjFace(uint i);
	const Face* adjFace(uint i) const;
	Face*&      adjFaceMod(int i);
	const Face* adjFaceMod(int i) const;

	void setAdjFace(Face* f, uint i);
	void setAdjFaces(const std::vector<Face*>& list);

	bool containsAdjFace(const Face* f) const;

	AdjacentFaceIterator      findAdjFace(const Face* f);
	ConstAdjacentFaceIterator findAdjFace(const Face* f) const;

	int indexOfAdjFace(const Face* f) const;

	/* Iterator Member functions */

	AdjacentFaceIterator           adjFaceBegin();
	AdjacentFaceIterator           adjFaceBegin(HalfEdge* he);
	AdjacentFaceIterator           adjFaceEnd();
	ConstAdjacentFaceIterator      adjFaceBegin() const;
	ConstAdjacentFaceIterator      adjFaceBegin(const HalfEdge* he) const;
	ConstAdjacentFaceIterator      adjFaceEnd() const;
	AdjacentFaceRangeIterator      adjFaces();
	ConstAdjacentFaceRangeIterator adjFaces() const;

	HalfEdgeIterator           haflEdgeBegin();
	HalfEdgeIterator           haflEdgeBegin(HalfEdge* he);
	ConstHalfEdgeIterator      halfEdgeBegin() const;
	ConstHalfEdgeIterator      halfEdgeBegin(const HalfEdge* he) const;
	HalfEdgeIterator           halfEdgeEnd();
	ConstHalfEdgeIterator      halfEdgeEnd() const;
	HalfEdgeRangeIterator      halfEdges();
	ConstHalfEdgeRangeIterator halfEdges() const;

	InnerHalfEdgeIterator           innerHalfEdgeBegin();
	ConstInnerHalfEdgeIterator      innerHalfEdgeBegin() const;
	InnerHalfEdgeIterator           innerHalfEdgeEnd();
	ConstInnerHalfEdgeIterator      innerHalfEdgeEnd() const;
	InnerHalfEdgeRangeIterator      innerHalfEdges();
	ConstInnerHalfEdgeRangeIterator innerHalfEdges() const;

	VertexIterator           vertexBegin();
	VertexIterator           vertexBegin(HalfEdge* he);
	ConstVertexIterator      vertexBegin() const;
	ConstVertexIterator      vertexBegin(const HalfEdge* he) const;
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexEnd() const;
	VertexRangeIterator      vertices();
	ConstVertexRangeIterator vertices() const;

protected:
	void updateHalfEdgeReferences(const HalfEdge* oldBase, const HalfEdge* newBase);

	void
	updateHalfEdgeReferencesAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);
	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	void updateFaceReferences(const Face* oldBase, const Face* newBase);
	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);

	template<typename Element>
	void importFrom(const Element& e);

	template<typename OtherFace, typename OtherHEdge>
	void importHalfEdgeReferencesFrom(const OtherFace& e, HalfEdge* base, const OtherHEdge* ebase);

private:
	HalfEdge*              ohe = nullptr; // outer half edge
	std::vector<HalfEdge*> ihe;           // inner half edges, one for each hole of the face
};

} // namespace vcl::comp

#include "face_half_edge_reference.cpp"

#endif // VCL_MESH_COMPONENTS_FACE_HALF_EDGE_REFERENCE_H
