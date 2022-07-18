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

#ifndef VCL_MESH_COMPONENTS_HALF_EDGE_REFERENCES_H
#define VCL_MESH_COMPONENTS_HALF_EDGE_REFERENCES_H

#include <vector>

#include "../concepts/half_edge_references.h"

namespace vcl::comp {

template<typename HalfEdge, typename Vertex, typename Face>
class HalfEdgeReferences
{
public:
	using HalfEdgeType = HalfEdge;
	using VertexType   = Vertex;
	using FaceType     = Face;

	HalfEdgeReferences();

	const HalfEdge* next() const;
	HalfEdge*&      next();

	const HalfEdge* prev() const;
	HalfEdge*&      prev();

	const HalfEdge* twin() const;
	HalfEdge*&      twin();

	const Vertex* fromVertex() const;
	Vertex*&      fromVertex();

	const Vertex* toVertex() const;
	Vertex*&      toVertex();

	const Vertex* vertex(uint i) const;
	Vertex*&      vertex(uint i);

	const Face* face() const;
	Face*&      face();

protected:
	void updateHalfEdgeReferences(const HalfEdge* oldBase, const HalfEdge* newBase);
	void
	updateHalfEdgeReferencesAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

	void updateFaceReferences(const Face* oldBase, const Face* newBase);
	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);

	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);
	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	template<typename Element>
	void importFrom(const Element& e);

	template<typename HE, typename HEType>
	void importHalfEdgeReferencesFrom(const HE& e, HalfEdge* base, const HEType* ebase);

	template<typename HE, typename VType>
	void importVertexReferencesFrom(const HE& e, Vertex* base, const VType* ebase);

	template<typename HE, typename FType>
	void importFaceReferencesFrom(const HE& e, Face* base, const FType* ebase);

private:
	HalfEdge* n = nullptr; // next half edge
	HalfEdge* p = nullptr; // prev half edge
	HalfEdge* t = nullptr; // twin half edge
	Vertex*   v = nullptr; // from vertex
	Face*     f = nullptr; // incident face
};

} // namespace vcl::comp

#include "half_edge_references.cpp"

#endif // VCL_MESH_COMPONENTS_HALF_EDGE_REFERENCES_H
