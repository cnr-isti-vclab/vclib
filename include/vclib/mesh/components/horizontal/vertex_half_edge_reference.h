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

#ifndef VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_REFERENCE_H
#define VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_REFERENCE_H

#include <vector>

#include "../detection/vertex_half_edge_reference_detection.h"

namespace vcl::comp {

template<typename HalfEdge>
class VertexHalfEdgeReference : public VertexHalfEdgeReferenceTriggerer
{
public:
	VertexHalfEdgeReference();

	const HalfEdge* halfEdge() const;
	HalfEdge*& halfEdge();

protected:
	void updateHalfEdgeReferences(const HalfEdge* oldBase, const HalfEdge* newBase);
	void
	updateHalfEdgeReferencesAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

	template<typename Element>
	void importFrom(const Element& e);

	template<typename Vertex, typename VertHEType>
	void importHalfEdgeReferencesFrom(const Vertex& e, HalfEdge* base, const VertHEType* ebase);

private:
	HalfEdge* he = nullptr; // outgoing half edge
};

} // vcl::comp

#include "vertex_half_edge_reference.cpp"

#endif // VCL_MESH_COMPONENTS_VERTEX_HALF_EDGE_REFERENCE_H
