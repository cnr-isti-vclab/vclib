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

#include "internal/component_data.h"

namespace vcl::comp {

/**
 * @brief The VertexHalfEdgePointer class
 *
 * @ingroup components
 */
template<
	typename HalfEdge,
	typename ElementType = void,
	bool optional        = false>
class VertexHalfEdgePointer : public PointersComponentTriggerer<HalfEdge>
{
	using ThisType = VertexHalfEdgePointer<HalfEdge, ElementType, optional>;

	using Vertex = typename HalfEdge::VertexType;
	using Face   = typename HalfEdge::FaceType;

public:
	using DataValueType = HalfEdge*; // data that the component stores internally (or vertically)
	using VertexHalfEdgePointerComponent = ThisType; // expose the type to allow access to this component

	static const bool IS_VERTICAL = !std::is_same_v<ElementType, void>;
	static const bool IS_OPTIONAL = optional;

	using HalfEdgeType = HalfEdge;

	/* Iterator Types declaration */

	using AdjacentFaceIterator      = vcl::VertexAdjFaceIterator<HalfEdge>;
	using ConstAdjacentFaceIterator = vcl::ConstVertexAdjFaceIterator<HalfEdge>;
	using AdjacentFaceView          = vcl::View<AdjacentFaceIterator>;
	using ConstAdjacentFaceView     = vcl::View<ConstAdjacentFaceIterator>;

	using AdjacentVertexIterator      = vcl::VertexAdjVertexIterator<HalfEdge>;
	using ConstAdjacentVertexIterator = vcl::ConstVertexAdjVertexIterator<HalfEdge>;
	using AdjacentVertexView          = vcl::View<AdjacentVertexIterator>;
	using ConstAdjacentVertexView     = vcl::View<ConstAdjacentVertexIterator>;

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
	AdjacentFaceView          adjFaces();
	ConstAdjacentFaceView     adjFaces() const;

	AdjacentVertexIterator      adjVertexBegin();
	AdjacentVertexIterator      adjVertexEnd();
	ConstAdjacentVertexIterator adjVertexBegin() const;
	ConstAdjacentVertexIterator adjVertexEnd() const;
	AdjacentVertexView          adjVertices();
	ConstAdjacentVertexView     adjVertices() const;

protected:
	void updateReferences(const HalfEdge* oldBase, const HalfEdge* newBase);
	void updateReferencesAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

	template<typename Element>
	void importFrom(const Element& e);

	template<typename OtherVertex, typename OtherHEType>
	void importReferencesFrom(const OtherVertex& e, HalfEdge* base, const OtherHEType* ebase);

private:
	HalfEdge*& he();
	const HalfEdge* he() const;

	internal::ComponentData<HalfEdge*, IS_VERTICAL> data;
};

} // vcl::comp

#include "vertex_half_edge_pointer.cpp"

#endif // VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_POINTER_H
