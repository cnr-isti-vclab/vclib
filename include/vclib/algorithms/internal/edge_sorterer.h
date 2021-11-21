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

#ifndef VCL_ALGORITHMS_INTERNAL_EDGE_SORTERER_H
#define VCL_ALGORITHMS_INTERNAL_EDGE_SORTERER_H

#include <algorithm>
#include <assert.h>
#include <vector>

namespace vcl::internal {

template<typename MeshType>
class EdgeSorterer
{
public:
	using VertexType = typename MeshType::Vertex;
	using FaceType   = typename MeshType::Face;

	VertexType* v[2]; // Puntatore ai due vertici (Ordinati)
	FaceType*   f;    // Puntatore alla faccia generatrice
	int         z;    // Indice dell'edge nella faccia

	EdgeSorterer() {} // Nothing to do

	void set(const FaceType& pf, const int nz)
	{
		assert(pf != 0);
		assert(nz >= 0);
		assert(nz < 3);

		v[0] = pf.vertex(nz);
		v[1] = pf.vertexMod(nz + 1);
		assert(v[0] != v[1]);

		if (v[0] > v[1])
			std::swap(v[0], v[1]);
		f = &pf;
		z = nz;
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
std::vector<EdgeSorterer<MeshType>> fillAndSortEdgeVector(
	const MeshType& m,
	bool includeFauxEdges = true)
{
	using FaceType = typename MeshType::Face;

	std::vector<EdgeSorterer<MeshType>> vec;

	int n_edges = 0;
	for (const FaceType& f : m.faces())
		n_edges += f.vertexNumber();

	vec.reserve(n_edges);

	auto edgeIterator = vec.begin();
	for (FaceType& f : m.faces()) { // Lo riempio con i dati delle facce
		for (int j = 0; j < f.vertexNumber(); ++j) {
			if (includeFauxEdges || !f.isEdgeFaux(j)) {
				(*edgeIterator).set(f, j);
				++edgeIterator;
			}
		}
	}

	std::sort(vec.begin(), vec.end()); // Lo ordino per vertici

	return vec;
}

} // namespace vcl::internal

#endif // VCL_ALGORITHMS_INTERNAL_EDGE_SORTERER_H
