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

#ifndef VCL_MESH_REQUIREMENTS_FACE_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_FACE_REQUIREMENTS_H

#include <vclib/concepts/mesh.h>

#include "element_requirements.h"

/**
 * @defgroup face_requirements Face Requirements
 * @ingroup mesh_requirements
 *
 * @brief List functions that allows to check, enable and assert the
 * availability of components in the Face Elements of a Mesh.
 */

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

// Face Compactness

template<FaceMeshConcept MeshType>
bool isFaceContainerCompact(const MeshType&);

// Face AdjacentEdges

template<FaceMeshConcept MeshType>
bool isPerFaceAdjacentEdgesAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceAdjacentEdgesOptional(MeshType&);

// Face AdjacentFaces

template<FaceMeshConcept MeshType>
bool isPerFaceAdjacentFacesAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType&);

// Face Color

template<FaceMeshConcept MeshType>
bool isPerFaceColorAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceColorOptional(MeshType&);

// Face Mark

template<FaceMeshConcept MeshType>
bool isPerFaceMarkAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceMarkOptional(MeshType&);

// Face Normal

template<FaceMeshConcept MeshType>
bool isPerFaceNormalAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceNormalOptional(MeshType&);

// Face PrincipalCurvature

template<FaceMeshConcept MeshType>
bool isPerFacePrincipalCurvatureAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFacePrincipalCurvatureOptional(MeshType&);

// Face Quality

template<FaceMeshConcept MeshType>
bool isPerFaceQualityAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceQualityOptional(MeshType&);

// Face WedgeColors

template<FaceMeshConcept MeshType>
bool isPerFaceWedgeColorsAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceWedgeColorsOptional(MeshType&);

// Face WedgeTexCoords

template<FaceMeshConcept MeshType>
bool isPerFaceWedgeTexCoordsAvailable(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceWedgeTexCoordsOptional(MeshType&);


/*********************
 * require functions *
 *********************/

// Face Compactness

template<FaceMeshConcept MeshType>
void requireFaceContainerCompactness(const MeshType& m);

// Face AdjacentEdges

template<FaceMeshConcept MeshType>
void requirePerFaceAdjacentEdges(const MeshType& m)
	requires HasPerFaceAdjacentEdges<MeshType>;

// Face AdjacentFaces

template<FaceMeshConcept MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m)
	requires HasPerFaceAdjacentFaces<MeshType>;

// Face Color

template<FaceMeshConcept MeshType>
void requirePerFaceColor(const MeshType& m)
	requires HasPerFaceColor<MeshType>;

// Face Mark

template<FaceMeshConcept MeshType>
void requirePerFaceMark(const MeshType& m)
	requires HasPerFaceMark<MeshType>;

// Face Normal

template<FaceMeshConcept MeshType>
void requirePerFaceNormal(const MeshType& m)
	requires HasPerFaceNormal<MeshType>;

// Face PrincipalCurvature

template<FaceMeshConcept MeshType>
void requirePerFacePrincipalCurvature(const MeshType& m)
	requires HasPerFacePrincipalCurvature<MeshType>;

// Face Quality

template<FaceMeshConcept MeshType>
void requirePerFaceQuality(const MeshType& m)
	requires HasPerFaceQuality<MeshType>;

// Face WedgeColors

template<FaceMeshConcept MeshType>
void requirePerFaceWedgeColors(const MeshType& m)
	requires HasPerFaceWedgeColors<MeshType>;

// Face WedgeTexCoords

template<FaceMeshConcept MeshType>
void requirePerFaceWedgeTexCoords(const MeshType& m)
	requires HasPerFaceWedgeTexCoords<MeshType>;


} // namespace vcl

#include "face_requirements.cpp"

#endif // VCL_MESH_REQUIREMENTS_FACE_REQUIREMENTS_H
