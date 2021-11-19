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

#include "flag.h"

#include <algorithm>
#include <assert.h>
#include <vector>

#include <vclib/mesh/requirements.h>

namespace vcl {

namespace internal {

template<typename MeshType>
class EdgeSorter
{
public:
	using VertexType    = typename MeshType::Vertex;
	using FaceType      = typename MeshType::Face;
	using VertexPointer = VertexType*;
	using FacePointer   = FaceType*;

	VertexPointer v[2]; // Puntatore ai due vertici (Ordinati)
	FacePointer   f;    // Puntatore alla faccia generatrice
	int           z;    // Indice dell'edge nella faccia

	EdgeSorter() {} // Nothing to do

	void set(const FacePointer pf, const int nz)
	{
		assert(pf != 0);
		assert(nz >= 0);
		assert(nz < 3);

		v[0] = pf->vertex(nz);
		v[1] = pf->vertexMod(nz + 1);
		assert(v[0] != v[1]);

		if (v[0] > v[1])
			std::swap(v[0], v[1]);
		f = pf;
		z = nz;
	}

	bool operator<(const EdgeSorter& pe) const
	{
		if (v[0] < pe.v[0])
			return true;
		else if (v[0] > pe.v[0])
			return false;
		else
			return v[1] < pe.v[1];
	}

	bool operator==(const EdgeSorter& pe) const { return v[0] == pe.v[0] && v[1] == pe.v[1]; }
	bool operator!=(const EdgeSorter& pe) const { return v[0] != pe.v[0] || v[1] != pe.v[1]; }
};

} // namespace internal

/**
 * @brief Computes per-face border flags without requiring any kind of
 * topology info.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * Complexity: O(NF log (NF))
 *
 * @param m: the mesh on which the border flags will be updated
 */
template<typename MeshType>
void updateBorder(MeshType& m)
{
	vcl::requireVertices(m);
	vcl::requireFaces(m);

	using VertexType    = typename MeshType::Vertex;
	using FaceType      = typename MeshType::Face;
	using VertexPointer = VertexType*;
	using FacePointer   = FaceType*;

	std::vector<internal::EdgeSorter<MeshType>>                    e;
	typename std::vector<internal::EdgeSorter<MeshType>>::iterator edgeIterator;

	for (FaceType& f : m.faces())
		f.clearAllEdgeOnBorder();

	if (m.faceNumber() == 0)
		return;

	// FaceIterator fi;
	int n_edges = 0;
	for (const FaceType& f : m.faces())
		n_edges += f.vertexNumber();

	e.resize(n_edges);

	edgeIterator = e.begin();
	for (FaceType& f : m.faces()) { // Lo riempio con i dati delle facce
		f.clearAllEdgeOnBorder();
		for (int j = 0; j < f.vertexNumber(); ++j) {
			(*edgeIterator).set(&f, j);
			++edgeIterator;
		}
	}
	assert(edgeIterator == e.end());
	std::sort(e.begin(), e.end()); // Lo ordino per vertici

	typename std::vector<internal::EdgeSorter<MeshType>>::iterator pe, ps;
	ps = e.begin();
	pe = e.begin();
	do {
		if (pe == e.end() || *pe != *ps) { // Trovo blocco di edge uguali
			if (pe - ps == 1) {
				ps->f->setEdgeOnBorder(ps->z);
			}
			ps = pe;
		}
		if (pe != e.end())
			++pe;
	} while (pe != e.end());
}

} // namespace vcl
