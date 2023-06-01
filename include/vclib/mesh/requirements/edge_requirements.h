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

#ifndef VCL_MESH_REQUIREMENTS_EDGE_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_EDGE_REQUIREMENTS_H

#include <vclib/concepts/mesh.h>

#include "element_requirements.h"

/**
 * @defgroup edge_requirements Edge Requirements
 * @ingroup mesh_requirements
 *
 * @brief List functions that allows to check, enable and assert the availability of components
 * in the Edge Elements of a Mesh.
 */

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

// Edge Compactness

template<EdgeMeshConcept MeshType>
bool isEdgeContainerCompact(const MeshType&);

// AdjacentEdges Edges

template<EdgeMeshConcept MeshType>
bool isPerEdgeAdjacentEdgesEnabled(const MeshType&);

template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeAdjacentEdgesOptional(MeshType&);

// AdjacentFaces Edges

template<EdgeMeshConcept MeshType>
bool isPerEdgeAdjacentFacesEnabled(const MeshType&);

template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeAdjacentFacesOptional(MeshType&);

// Color Edges

template<EdgeMeshConcept MeshType>
bool isPerEdgeColorEnabled(const MeshType&);

template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeColorOptional(MeshType&);

// Mark Edges

template<EdgeMeshConcept MeshType>
bool isPerEdgeMarkEnabled(const MeshType&);

template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeMarkOptional(MeshType&);

// Quality Edges

template<EdgeMeshConcept MeshType>
bool isPerEdgeQualityEnabled(const MeshType&);

template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeQualityOptional(MeshType&);


/*********************
 * require functions *
 *********************/

// Edge Compactness

template<EdgeMeshConcept MeshType>
void requireEdgeContainerCompactness(const MeshType& m);

// AdjacentEdges Edges

template<EdgeMeshConcept MeshType>
void requirePerEdgeAdjacentEdges(const MeshType& m)
	requires HasPerEdgeAdjacentEdges<MeshType>;

// AdjacentFaces Edges

template<EdgeMeshConcept MeshType>
void requirePerEdgeAdjacentFaces(const MeshType& m)
	requires HasPerEdgeAdjacentFaces<MeshType>;

// Color Edges

template<EdgeMeshConcept MeshType>
void requirePerEdgeColor(const MeshType& m)
	requires HasPerEdgeColor<MeshType>;

// Mark Edges

template<EdgeMeshConcept MeshType>
void requirePerEdgeMark(const MeshType& m)
	requires HasPerEdgeMark<MeshType>;

// Quality Edges

template<EdgeMeshConcept MeshType>
void requirePerEdgeQuality(const MeshType& m)
	requires HasPerEdgeQuality<MeshType>;


} // namespace vcl

#include "edge_requirements.cpp"

#endif // VCL_MESH_REQUIREMENTS_EDGE_REQUIREMENTS_H
