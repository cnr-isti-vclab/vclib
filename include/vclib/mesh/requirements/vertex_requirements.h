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

#ifndef VCL_MESH_REQUIREMENTS_VERTEX_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_VERTEX_REQUIREMENTS_H

#include <vclib/concepts/mesh.h>

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

// Vertex Compactness

template<MeshConcept MeshType>
bool isVertexContainerCompact(const MeshType&);

// Vertex Adjacent Faces

template<MeshConcept MeshType>
bool isPerVertexAdjacentFacesEnabled(const MeshType&);

template<MeshConcept MeshType>
bool enableIfPerVertexAdjacentFacesOptional(MeshType&);

// Vertex Adjacent Vertices

template<MeshConcept MeshType>
bool isPerVertexAdjacentVerticesEnabled(const MeshType&);

template<MeshConcept MeshType>
bool enableIfPerVertexAdjacentVerticesOptional(MeshType&);

// Vertex Color

template<MeshConcept MeshType>
bool isPerVertexColorEnabled(const MeshType&);

template<MeshConcept MeshType>
bool enableIfPerVertexColorOptional(MeshType&);

// Vertex Mark

template<MeshConcept MeshType>
bool isPerVertexMarkEnabled(const MeshType&);

template<MeshConcept MeshType>
bool enableIfPerVertexMarkOptional(MeshType&);

// Vertex Normal

template<MeshConcept MeshType>
bool isPerVertexNormalEnabled(const MeshType&);

template<MeshConcept MeshType>
bool enableIfPerVertexNormalOptional(MeshType&);

// Vertex PrincipalCurvature

template<MeshConcept MeshType>
bool isPerVertexPrincipalCurvatureEnabled(const MeshType&);

template<MeshConcept MeshType>
bool enableIfPerVertexPrincipalCurvatureOptional(MeshType&);

// Vertex Quality

template<MeshConcept MeshType>
bool isPerVertexQualityEnabled(const MeshType&);

template<MeshConcept MeshType>
bool enableIfPerVertexQualityOptional(MeshType&);

// Vertex TexCoord

template<MeshConcept MeshType>
bool isPerVertexTexCoordEnabled(const MeshType&);

template<MeshConcept MeshType>
bool enableIfPerVertexTexCoordOptional(MeshType&);

/*********************
 * require functions *
 *********************/

// Vertex Compactness

template<MeshConcept MeshType>
void requireVertexContainerCompactness(const MeshType& m);

// Vertex Adjacent Faces

template<typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m)
	requires HasPerVertexAdjacentFaces<MeshType>;

// Vertex Adjacent Vertices

template<typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m)
	requires HasPerVertexAdjacentVertices<MeshType>;

// Vertex Color

template<typename MeshType>
void requirePerVertexColor(const MeshType& m)
	requires HasPerVertexColor<MeshType>;

// Vertex Mark

template<typename MeshType>
void requirePerVertexMark(const MeshType& m)
	requires HasPerVertexMark<MeshType>;

// Vertex Normal

template<typename MeshType>
void requirePerVertexNormal(const MeshType& m)
	requires HasPerVertexNormal<MeshType>;

// Vertex PrincipalCurvature

template<typename MeshType>
void requirePerVertexPrincipalCurvature(const MeshType& m)
	requires HasPerVertexPrincipalCurvature<MeshType>;

// Vertex Quality

template<typename MeshType>
void requirePerVertexQuality(const MeshType& m)
	requires HasPerVertexQuality<MeshType>;

// Vertex TexCoord

template<typename MeshType>
void requirePerVertexTexCoord(const MeshType& m)
	requires HasPerVertexTexCoord<MeshType>;

} // namespace vcl

#include "vertex_requirements.cpp"

#endif // VCL_MESH_REQUIREMENTS_VERTEX_REQUIREMENTS_H
