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

#include <vclib/iterators/half_edge/face_half_edge_iterator.h>
#include <vclib/iterators/half_edge/face_vertex_iterator.h>
#include <vclib/iterators/range_iterator.h>

#include "../detection/face_half_edge_reference_detection.h"

#include "../detection/adjacent_faces_detection.h"
#include "../detection/vertex_references_detection.h"

namespace vcl::comp {

template<typename HalfEdge>
class FaceHalfEdgeReference :
		public FaceHalfEdgeReferenceTriggerer,
		public VertexReferencesTriggerer,
		public AdjacentFacesTriggerer
{
	using Vertex = typename HalfEdge::VertexType;
public:
	using HalfEdgeType = HalfEdge;
	using VertexType = typename HalfEdge::VertexType;

	using HalfEdgeIterator = vcl::FaceHalfEdgeIterator<HalfEdge>;
	using ConstHalfEdgeIterator = vcl::ConstFaceHalfEdgeIterator<HalfEdge>;
	using HalfEdgeRangeIterator = RangeIterator<FaceHalfEdgeReference, HalfEdgeIterator>;
	using ConstHalfEdgeRangeIterator =
		ConstRangeIterator<FaceHalfEdgeReference, ConstHalfEdgeIterator>;

	using VertexIterator = vcl::FaceVertexIterator<HalfEdge>;
	using ConstVertexIterator = vcl::ConstFaceVertexIterator<HalfEdge>;
	using VertexRangeIterator = RangeIterator<FaceHalfEdgeReference, VertexIterator>;
	using ConstVertexRangeIterator = ConstRangeIterator<FaceHalfEdgeReference, ConstVertexIterator>;

	// Vertex references can be accessed from a face using half edge reference, therefore this
	// component claims that it is the VertexReferences component. This is done just for
	// compatibility between mesh types.
	using VertexReferences = FaceHalfEdgeReference;
	static const int VERTEX_NUMBER = -1; // half edges support by design polygonal meshes

	/* Constructor */

	FaceHalfEdgeReference();

	const HalfEdge* outerHalfEdge() const;
	HalfEdge*& outerHalfEdge();

	uint numberHoles() const;
	const HalfEdge* innerHalfEdge(uint i) const;
	HalfEdge*& innerHalfEdge(uint i);

	uint vertexNumber() const;

	Vertex*&      vertex(uint i);
	const Vertex* vertex(uint i) const;
	Vertex*&      vertexMod(int i);
	const Vertex* vertexMod(int i) const;

	void setVertex(Vertex* v, uint i);
	void setVertices(const std::vector<Vertex*>& list);

	bool containsVertex(const Vertex* v) const;

	VertexIterator findVertex(const Vertex* v);
	ConstVertexIterator findVertex(const Vertex* v) const;

	int indexOfVertex(const Vertex* v) const;
	int indexOfEdge(const Vertex* v1, const Vertex* v2) const;

	/* Iterator Member functions */

	HalfEdgeIterator           haflEdgeBegin();
	ConstHalfEdgeIterator      halfEdgeBegin() const;
	HalfEdgeIterator           halfEdgeEnd();
	ConstHalfEdgeIterator      halfEdgeEnd() const;
	HalfEdgeRangeIterator      halfEdges();
	ConstHalfEdgeRangeIterator halfEdges() const;

	VertexIterator           vertexBegin();
	ConstVertexIterator      vertexBegin() const;
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexEnd() const;
	VertexRangeIterator      vertices();
	ConstVertexRangeIterator vertices() const;

private:
	HalfEdge* ohe = nullptr; // outer half edge
	std::vector<HalfEdge*> ihe; // inner half edges, one for each hole of the face
};

} // vcl::comp

#include "face_half_edge_reference.cpp"

#endif // VCL_MESH_COMPONENTS_FACE_HALF_EDGE_REFERENCE_H
