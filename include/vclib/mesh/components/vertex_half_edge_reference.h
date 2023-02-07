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

#ifndef VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_REFERENCE_H
#define VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_REFERENCE_H

#include <vector>

#include <vclib/iterators/mesh/half_edge/vertex_adj_face_iterator.h>
#include <vclib/iterators/mesh/half_edge/vertex_adj_vertex_iterator.h>
#include <vclib/iterators/range_iterator.h>

#include "concepts/vertex_half_edge_reference.h"
#include "internal/component_data.h"

namespace vcl::comp {

template<
	typename HalfEdge,
	typename ElementType = void,
	bool horizontal      = true,
	bool optional        = false>
class VertexHalfEdgeReference
{
	using ThisType = VertexHalfEdgeReference<HalfEdge, ElementType, horizontal, optional>;

	using Vertex = typename HalfEdge::VertexType;
	using Face   = typename HalfEdge::FaceType;

public:
	using DataValueType = HalfEdge*; // data that the component stores internally (or vertically)
	using VertexHalfEdgeReferencesComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !horizontal;
	static const bool IS_OPTIONAL = optional;

	using HalfEdgeType = HalfEdge;

	/* Iterator Types declaration */

	using AdjacentFaceIterator      = vcl::VertexAdjFaceIterator<HalfEdge>;
	using ConstAdjacentFaceIterator = vcl::ConstVertexAdjFaceIterator<HalfEdge>;
	using AdjacentFaceRangeIterator = RangeIterator<VertexHalfEdgeReference, AdjacentFaceIterator>;
	using ConstAdjacentFaceRangeIterator =
		ConstRangeIterator<VertexHalfEdgeReference, ConstAdjacentFaceIterator>;

	using AdjacentVertexIterator      = vcl::VertexAdjVertexIterator<HalfEdge>;
	using ConstAdjacentVertexIterator = vcl::ConstVertexAdjVertexIterator<HalfEdge>;
	using AdjacentVertexRangeIterator =
		RangeIterator<VertexHalfEdgeReference, AdjacentVertexIterator>;
	using ConstAdjacentVertexRangeIterator =
		ConstRangeIterator<VertexHalfEdgeReference, ConstAdjacentVertexIterator>;

	/* Constructor */

	void init();

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

	AdjacentFaceIterator           adjFaceBegin();
	AdjacentFaceIterator           adjFaceEnd();
	ConstAdjacentFaceIterator      adjFaceBegin() const;
	ConstAdjacentFaceIterator      adjFaceEnd() const;
	AdjacentFaceRangeIterator      adjFaces();
	ConstAdjacentFaceRangeIterator adjFaces() const;

	AdjacentVertexIterator           adjVertexBegin();
	AdjacentVertexIterator           adjVertexEnd();
	ConstAdjacentVertexIterator      adjVertexBegin() const;
	ConstAdjacentVertexIterator      adjVertexEnd() const;
	AdjacentVertexRangeIterator      adjVertices();
	ConstAdjacentVertexRangeIterator adjVertices() const;

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

	template<typename OtherVertex, typename OtherHEType>
	void importHalfEdgeReferencesFrom(const OtherVertex& e, HalfEdge* base, const OtherHEType* ebase);

private:
	HalfEdge*& he();
	const HalfEdge* he() const;

	internal::ComponentData<HalfEdge*, true> data;
};

} // vcl::comp

#include "vertex_half_edge_reference.cpp"

#endif // VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_REFERENCE_H
