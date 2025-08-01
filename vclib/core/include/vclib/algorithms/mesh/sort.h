/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_ALGORITHMS_MESH_SORT_H
#define VCL_ALGORITHMS_MESH_SORT_H

#include <vclib/space/complex.h>

#include <algorithm>

namespace vcl {

template<FaceMeshConcept MeshType>
std::vector<MeshEdgeUtil<MeshType>> fillAndSortMeshEdgeUtilVector(
    MeshType& m,
    bool      includeFauxEdges = true)
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
std::vector<ConstMeshEdgeUtil<MeshType>> fillAndSortMeshEdgeUtilVector(
    const MeshType& m,
    bool            includeFauxEdges = true)
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

#endif // VCL_ALGORITHMS_MESH_SORT_H
