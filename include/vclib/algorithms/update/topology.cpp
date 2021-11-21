/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#include "topology.h"

#include <vclib/algorithms/internal/edge_sorterer.h>

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief updatePerVertexAdjacentFaces
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - AdjacentFaces
 *   - Faces
 *
 * @param m
 */
template<typename MeshType>
void updatePerVertexAdjacentFaces(MeshType& m)
{
	vcl::requirePerVertexAdjacentFaces(m);
	vcl::requireFaces<MeshType>();

	using VertexType = typename MeshType::Vertex;
	using FaceType   = typename MeshType::Face;

	for (VertexType& v : m.vertices()) {
		v.clearAdjFaces();
	}

	for (FaceType& f : m.faces()) {
		for (VertexType* v : f.vertices()){
			v->pushAdjFace(&f);
		}
	}
}

/**
 * @brief Updates the per face adjacent face component.
 *
 * Requirements:
 * - Mesh:
 *   - Faces:
 *     - AdjacentFaces
 *
 * @param m
 */
template<typename MeshType>
void updatePerFaceAdjacentFaces(MeshType& m)
{
	vcl::requirePerFaceAdjacentFaces(m);

	// vector that contains edges sorted trough unordered vertex pointers
	// it contains clusters of "same" edges, but each one of them has its face pointer
	// note that in case on non-manifold mesh, clusters may be of size >= 2
	std::vector<internal::EdgeSorterer<MeshType>> vec = internal::fillAndSortEdgeVector(m);

	if (vec.size() > 0) {

		// in this loop, base will point to the first element of a cluster of edges
		for (auto base = vec.begin(); base != vec.end(); /* increment of clusters into loop*/) {
			auto first = base; // remember the first to set adj to the last

			// i and j will increment together, and if i == j, i will be adj to j, but j will not be
			// adj to i (to manage non manifold edges and make cyclic adj on the same edge)
			auto i = base;
			auto j = i+1;

			if (*i != *j) { // case of cluster composed of one element. adj of i is nullptr
				i->f->adjFace(i->z) = nullptr;
			}
			else { // at least two edges in the cluster
				while (*i == *j && j != vec.end()) {
					i->f->adjFace(i->z) = j->f;
					++i;
					++j;
				}
				// i now is the last element that was equal to first
				i->f->adjFace(i->z) = first->f;
			}

			// j is the first different edge from first (or it is vec.end()!)
			base = j;
		}
	}
}

}

