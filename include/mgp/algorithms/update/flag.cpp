/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

/**
 * This is a refactoring of VCGLib: https://github.com/cnr-isti-vclab/vcglib/
 * Copyright(C): Visual Computing Lab
 * ISTI - Italian National Research Council
 */

#include "flag.h"

#include <algorithm>
#include <assert.h>
#include <vector>

#include <mgp/mesh/requirements.h>

namespace mgp {

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
 * @brief mgp::updateBorder Computes per-face border flags without requiring any kind of
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
	mgp::requireVertices(m);
	mgp::requireFaces(m);

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

} // namespace mgp
