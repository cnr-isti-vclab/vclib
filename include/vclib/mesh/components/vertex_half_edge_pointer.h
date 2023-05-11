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

#ifndef VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_POINTER_H
#define VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_POINTER_H

#include <vector>

#include <vclib/concepts/mesh/components/vertex_half_edge_pointer.h>
#include <vclib/iterators/mesh/half_edge/vertex_adj_face_iterator.h>
#include <vclib/iterators/mesh/half_edge/vertex_adj_vertex_iterator.h>
#include <vclib/views/view.h>

#include "bases/component.h"

namespace vcl::comp {

/**
 * @brief The VertexHalfEdgePointer class
 *
 * @ingroup components
 */
template<
	typename HalfEdge,
	typename ElementType = void,
	bool OPT        = false>
class VertexHalfEdgePointer : public Component<VERTEX_HALF_EDGE_PTRS, HalfEdge*, ElementType, OPT, HalfEdge>
{
	using Base = Component<VERTEX_HALF_EDGE_PTRS, HalfEdge*, ElementType, OPT, HalfEdge>;

	using Vertex = typename HalfEdge::VertexType;
	using Face   = typename HalfEdge::FaceType;

public:
	using HalfEdgeType = HalfEdge;

	/* Iterator Types declaration */

	using AdjacentFaceIterator      = vcl::VertexAdjFaceIterator<HalfEdge>;
	using ConstAdjacentFaceIterator = vcl::ConstVertexAdjFaceIterator<HalfEdge>;

	using AdjacentVertexIterator      = vcl::VertexAdjVertexIterator<HalfEdge>;
	using ConstAdjacentVertexIterator = vcl::ConstVertexAdjVertexIterator<HalfEdge>;

	/* Constructor and isEnabled */

	VertexHalfEdgePointer();

	void init();

	bool isEnabled();

	/* Member functions */

	const HalfEdge* halfEdge() const;
	HalfEdge*& halfEdge();

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

	constexpr bool isAdjFacesEnabled() const { return true; }

	/* AdjacentVertices compatibility */

	uint adjVerticesNumber() const;

	Vertex*&      adjVertex(uint i);
	const Vertex* adjVertex(uint i) const;
	Vertex*&      adjVertexMod(int i);
	const Vertex* adjVertexMod(int i) const;

	void setAdjVertex(Vertex* v, uint i);
	void setAdjVertices(const std::vector<Vertex*>& list);

	bool containsAdjVertex(const Vertex* v) const;

	AdjacentVertexIterator      findAdjVertex(const Vertex* v);
	ConstAdjacentVertexIterator findAdjVertex(const Vertex* v) const;

	int indexOfAdjVertex(const Vertex* v) const;

	constexpr bool isAdjVerticesEnabled() const { return true; }

	/* Iterator Member functions */

	AdjacentFaceIterator      adjFaceBegin();
	AdjacentFaceIterator      adjFaceEnd();
	ConstAdjacentFaceIterator adjFaceBegin() const;
	ConstAdjacentFaceIterator adjFaceEnd() const;
	auto                      adjFaces();
	auto                      adjFaces() const;

	AdjacentVertexIterator      adjVertexBegin();
	AdjacentVertexIterator      adjVertexEnd();
	ConstAdjacentVertexIterator adjVertexBegin() const;
	ConstAdjacentVertexIterator adjVertexEnd() const;
	auto                        adjVertices();
	auto                        adjVertices() const;

protected:
	// Component interface function
	template<typename Element>
	void importFrom(const Element& e);

	// PointersComponent interface functions
	template<typename OtherVertex, typename OtherHEType>
	void importPointersFrom(const OtherVertex& e, HalfEdge* base, const OtherHEType* ebase);

	void updatePointers(const HalfEdge* oldBase, const HalfEdge* newBase);
	void updatePointersAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

private:
	HalfEdge*& he();
	const HalfEdge* he() const;
};

} // vcl::comp

#include "vertex_half_edge_pointer.cpp"

#endif // VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_POINTER_H
