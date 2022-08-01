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

#ifndef VCL_MESH_HALF_EDGE_REQUIREMENTS_H
#define VCL_MESH_HALF_EDGE_REQUIREMENTS_H

#include "mesh_concepts.h"

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

// HalfEdge Compactness

template<DcelMeshConcept MeshType>
bool isHalfEdgeContainerCompact(const MeshType&);

// HalfEdge Color

template<DcelMeshConcept MeshType>
bool isPerHalfEdgeColorEnabled(const MeshType&);

template<DcelMeshConcept MeshType>
bool enableIfPerHalfEdgeColorOptional(MeshType&);

/*********************
 * require functions *
 *********************/

// HalfEdge Compactness

template<DcelMeshConcept MeshType>
void requireHalfEdgeContainerCompactness(const MeshType& m);

// HalfEdge Color

template<DcelMeshConcept MeshType>
void requirePerHalfEdgeColor(const MeshType& m)
	requires HasPerHalfEdgeColor<MeshType>;

} // namespace vcl

#include "half_edge_requirements.cpp"

#endif // VCL_MESH_HALF_EDGE_REQUIREMENTS_H
