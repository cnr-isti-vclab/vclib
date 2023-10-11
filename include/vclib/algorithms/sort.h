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

#ifndef VCL_ALGORITHMS_SORT_H
#define VCL_ALGORITHMS_SORT_H

#include <algorithm>

#include <vclib/mesh/utils/mesh_edge_util.h>
#include <vclib/misc/parallel.h>

namespace vcl {

template<FaceMeshConcept MeshType>
std::vector<MeshEdgeUtil<MeshType>>
fillAndSortMeshEdgeUtilVector(MeshType& m, bool includeFauxEdges = true)
{
    using FaceType = MeshType::FaceType;

    std::vector<MeshEdgeUtil<MeshType>> vec;

    int n_edges = 0;
    for (const FaceType& f : m.faces())
        n_edges += f.vertexNumber();

    vec.reserve(n_edges);

    for (FaceType& f : m.faces()) { // Lo riempio con i dati delle facce
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (includeFauxEdges || !f.edgeFaux(j)) {
                vec.emplace_back(f, j);
            }
        }
    }

    // Lo ordino per vertici
    std::sort(std::execution::par_unseq, vec.begin(), vec.end());

    return vec;
}

template<FaceMeshConcept MeshType>
std::vector<ConstMeshEdgeUtil<MeshType>>
fillAndSortMeshEdgeUtilVector(const MeshType& m, bool includeFauxEdges = true)
{
    using FaceType = MeshType::FaceType;

    std::vector<ConstMeshEdgeUtil<MeshType>> vec;

    int n_edges = 0;
    for (const FaceType& f : m.faces())
        n_edges += f.vertexNumber();

    vec.reserve(n_edges);

    for (const FaceType& f : m.faces()) { // Lo riempio con i dati delle facce
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (includeFauxEdges || !f.edgeFaux(j)) {
                vec.emplace_back(f, j);
            }
        }
    }

    // Lo ordino per vertici
    std::sort(std::execution::par_unseq, vec.begin(), vec.end());

    return vec;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_SORT_H
