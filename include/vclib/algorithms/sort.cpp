/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#include "sort.h"

#include <algorithm>

#include <vclib/misc/parallel.h>

namespace vcl {

template<FaceMeshConcept MeshType>
std::vector<MeshEdgeUtil<MeshType>> fillAndSortMeshEdgeUtilVector(MeshType& m, bool includeFauxEdges)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<MeshEdgeUtil<MeshType>> vec;

	int n_edges = 0;
	for (const FaceType& f : m.faces())
		n_edges += f.vertexNumber();

	vec.reserve(n_edges);

	for (FaceType& f : m.faces()) { // Lo riempio con i dati delle facce
		for (uint j = 0; j < f.vertexNumber(); ++j) {
			if (includeFauxEdges || !f.isEdgeFaux(j)) {
				vec.push_back(MeshEdgeUtil<MeshType>(f, j));
			}
		}
	}
	std::sort(VCL_PARALLEL vec.begin(), vec.end()); // Lo ordino per vertici

	return vec;
}

template<FaceMeshConcept MeshType>
std::vector<ConstMeshEdgeUtil<MeshType>>
fillAndSortMeshEdgeUtilVector(const MeshType& m, bool includeFauxEdges)
{
	using FaceType = typename MeshType::FaceType;

	std::vector<ConstMeshEdgeUtil<MeshType>> vec;

	int n_edges = 0;
	for (const FaceType& f : m.faces())
		n_edges += f.vertexNumber();

	vec.reserve(n_edges);

	for (const FaceType& f : m.faces()) { // Lo riempio con i dati delle facce
		for (uint j = 0; j < f.vertexNumber(); ++j) {
			if (includeFauxEdges || !f.isEdgeFaux(j)) {
				vec.push_back(ConstMeshEdgeUtil<MeshType>(f, j));
			}
		}
	}

	std::sort(VCL_PARALLEL vec.begin(), vec.end()); // Lo ordino per vertici

	return vec;
}

} // namespace vcl
