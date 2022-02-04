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

#include "../detection/half_edge_references_detection.h"

namespace vcl::comp {

template<typename HalfEdge, typename Vertex, typename Face>
class HalfEdgeReferences : public HalfEdgeReferencesTriggerer
{
public:
	using HalfEdgeType = HalfEdge;

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
	Face*       face();

private:
	HalfEdge* n  = nullptr; // next
	HalfEdge* p  = nullptr; // prev
	HalfEdge* t  = nullptr; // twin
	Vertex*   fv = nullptr; // from vertex
	Vertex*   tv = nullptr; // to vertex
	Face*     f  = nullptr; // face
};

} // namespace vcl::comp

#include "half_edge_references.cpp"

#endif // VCL_MESH_COMPONENTS_HALF_EDGE_REFERENCES_H
