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

#ifndef VCL_ALGORITHMS_INTERNAL_EDGE_SORTERER_H
#define VCL_ALGORITHMS_INTERNAL_EDGE_SORTERER_H

#include <algorithm>
#include <assert.h>
#include <vector>

#include <vclib/misc/vcl_types.h>

namespace vcl::internal {

template<typename MeshType>
class EdgeSorterer
{
public:
	using VertexType = typename MeshType::VertexType;
	using FaceType   = typename MeshType::FaceType;

	VertexType* v[2]; // Pointer to the two (ordered) vertices of the edge
	FaceType*   f;    // Pointer to the face of the edge
	int         e;    // Index of the edge inside the face

	EdgeSorterer() : v {nullptr, nullptr}, f(nullptr), e(-1) {}
	EdgeSorterer(FaceType& pf, uint ne)
	{
		v[0] = pf.vertex(ne);
		v[1] = pf.vertexMod(ne + 1);
		assert(v[0] != v[1]);

		if (v[0] > v[1])
			std::swap(v[0], v[1]);
		f = &pf;
		e = ne;
	}

	bool operator<(const EdgeSorterer& pe) const
	{
		if (v[0] < pe.v[0])
			return true;
		else if (v[0] > pe.v[0])
			return false;
		else
			return v[1] < pe.v[1];
	}

	bool operator==(const EdgeSorterer& pe) const { return v[0] == pe.v[0] && v[1] == pe.v[1]; }
	bool operator!=(const EdgeSorterer& pe) const { return v[0] != pe.v[0] || v[1] != pe.v[1]; }
};

template<typename MeshType>
std::vector<EdgeSorterer<MeshType>> fillAndSortEdgeVector(MeshType& m, bool includeFauxEdges = true)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<EdgeSorterer<MeshType>> vec;

	int n_edges = 0;
	for (const FaceType& f : m.faces())
		n_edges += f.vertexNumber();

	vec.reserve(n_edges);

	for (FaceType& f : m.faces()) { // Lo riempio con i dati delle facce
		for (uint j = 0; j < f.vertexNumber(); ++j) {
			if (includeFauxEdges || !f.isEdgeFaux(j)) {
				vec.push_back(EdgeSorterer<MeshType>(f, j));
			}
		}
	}

	std::sort(vec.begin(), vec.end()); // Lo ordino per vertici

	return vec;
}

} // namespace vcl::internal

#endif // VCL_ALGORITHMS_INTERNAL_EDGE_SORTERER_H
