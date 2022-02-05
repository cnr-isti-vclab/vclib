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

#include "half_edge_references.h"

namespace vcl::comp {

template<typename HalfEdge, typename Vertex, typename Face>
HalfEdgeReferences<HalfEdge, Vertex, Face>::HalfEdgeReferences()
{
}

template<typename HalfEdge, typename Vertex, typename Face>
const HalfEdge* HalfEdgeReferences<HalfEdge, Vertex, Face>::next() const
{
	return n;
}

template<typename HalfEdge, typename Vertex, typename Face>
HalfEdge*& HalfEdgeReferences<HalfEdge, Vertex, Face>::next()
{
	return n;
}

template<typename HalfEdge, typename Vertex, typename Face>
const HalfEdge* HalfEdgeReferences<HalfEdge, Vertex, Face>::prev() const
{
	return p;
}

template<typename HalfEdge, typename Vertex, typename Face>
HalfEdge*& HalfEdgeReferences<HalfEdge, Vertex, Face>::prev()
{
	return p;
}

template<typename HalfEdge, typename Vertex, typename Face>
const HalfEdge* HalfEdgeReferences<HalfEdge, Vertex, Face>::twin() const
{
	return t;
}

template<typename HalfEdge, typename Vertex, typename Face>
HalfEdge*& HalfEdgeReferences<HalfEdge, Vertex, Face>::twin()
{
	return t;
}

template<typename HalfEdge, typename Vertex, typename Face>
const Vertex* HalfEdgeReferences<HalfEdge, Vertex, Face>::fromVertex() const
{
	return v;
}

template<typename HalfEdge, typename Vertex, typename Face>
Vertex*& HalfEdgeReferences<HalfEdge, Vertex, Face>::fromVertex()
{
	return v;
}

template<typename HalfEdge, typename Vertex, typename Face>
const Vertex* HalfEdgeReferences<HalfEdge, Vertex, Face>::toVertex() const
{
	return n->fromVertex();
}

template<typename HalfEdge, typename Vertex, typename Face>
Vertex*& HalfEdgeReferences<HalfEdge, Vertex, Face>::toVertex()
{
	return n->fromVertex();
}

template<typename HalfEdge, typename Vertex, typename Face>
const Vertex* HalfEdgeReferences<HalfEdge, Vertex, Face>::vertex(uint i) const
{
	switch(i) {
	case 0: return v;
	case 1: return toVertex();
	default: assert(0); return nullptr;
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
Vertex*& HalfEdgeReferences<HalfEdge, Vertex, Face>::vertex(uint i)
{
	switch(i) {
	case 0: return v;
	case 1: return toVertex();
	default: assert(0); return nullptr;
	}
}

template<typename HalfEdge, typename Vertex, typename Face>
const Face* HalfEdgeReferences<HalfEdge, Vertex, Face>::face() const
{
	return f;
}

template<typename HalfEdge, typename Vertex, typename Face>
Face* HalfEdgeReferences<HalfEdge, Vertex, Face>::face()
{
	return f;
}

} // namespace vcl::comp
