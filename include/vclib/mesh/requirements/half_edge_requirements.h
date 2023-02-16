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

// HalfEdge Mark

template<DcelMeshConcept MeshType>
bool isPerHalfEdgeMarkEnabled(const MeshType&);

template<DcelMeshConcept MeshType>
bool enableIfPerHalfEdgeMarkOptional(MeshType&);

// HalfEdge Scalar

template<DcelMeshConcept MeshType>
bool isPerHalfEdgeScalarEnabled(const MeshType&);

template<DcelMeshConcept MeshType>
bool enableIfPerHalfEdgeScalarOptional(MeshType&);

// HalfEdge TexCoord

template<DcelMeshConcept MeshType>
bool isPerHalfEdgeTexCoordEnabled(const MeshType&);

template<DcelMeshConcept MeshType>
bool enableIfPerHalfEdgeTexCoordOptional(MeshType&);

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

// HalfEdge Mark

template<DcelMeshConcept MeshType>
void requirePerHalfEdgeMark(const MeshType& m)
	requires HasPerHalfEdgeMark<MeshType>;

// HalfEdge Scalar

template<DcelMeshConcept MeshType>
void requirePerHalfEdgeScalar(const MeshType& m)
	requires HasPerHalfEdgeScalar<MeshType>;

// HalfEdge TexCoord

template<DcelMeshConcept MeshType>
void requirePerHalfEdgeTexCoord(const MeshType& m)
	requires HasPerHalfEdgeTexCoord<MeshType>;

} // namespace vcl

#include "half_edge_requirements.cpp"

#endif // VCL_MESH_HALF_EDGE_REQUIREMENTS_H
