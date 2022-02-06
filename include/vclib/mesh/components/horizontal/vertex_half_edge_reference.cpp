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

#include "vertex_half_edge_reference.h"

#include <cstddef>

namespace vcl::comp {

template<typename HalfEdge>
VertexHalfEdgeReference<HalfEdge>::VertexHalfEdgeReference()
{
}

template<typename HalfEdge>
const HalfEdge *VertexHalfEdgeReference<HalfEdge>::halfEdge() const
{
	return he;
}

template<typename HalfEdge>
HalfEdge *&VertexHalfEdgeReference<HalfEdge>::halfEdge()
{
	return he;
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::updateHalfEdgeReferences(
	const HalfEdge* oldBase,
	const HalfEdge* newBase)
{
	if (he != nullptr) {
		size_t diff = he - oldBase;
		he = newBase + diff;
	}
}

template<typename HalfEdge>
void VertexHalfEdgeReference<HalfEdge>::updateHalfEdgeReferencesAfterCompact(
	const HalfEdge*         base,
	const std::vector<int>& newIndices)
{
	if (he != nullptr) {
		size_t diff = he - base;
		if (newIndices[diff] < 0)
			he = nullptr;
		else
			he = base + newIndices[diff];
	}
}

template<typename HalfEdge>
template<typename Element>
void VertexHalfEdgeReference<HalfEdge>::importFrom(const Element &e)
{
}

template<typename HalfEdge>
template<typename Vertex, typename VertHEType>
void VertexHalfEdgeReference<HalfEdge>::importHalfEdgeReferencesFrom(
	const Vertex&  e,
	HalfEdge*       base,
	const VertHEType* ebase)
{
	if constexpr (hasVertexHalfEdgeReference<Vertex>()) {
		if (base != nullptr && ebase != nullptr) {
			if (e.halfEdge() != nullptr) {
				he = base + (e.halfEdge() - ebase);
			}
		}
	}

}

} // namespace vcl::comp
