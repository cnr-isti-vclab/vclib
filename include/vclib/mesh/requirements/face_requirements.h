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

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

// Face Compactness

template<FaceMeshConcept MeshType>
bool isFaceContainerCompact(const MeshType&);

// Face Adjacent Faces

template<FaceMeshConcept MeshType>
bool isPerFaceAdjacentFacesEnabled(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType&);

// Face Color

template<FaceMeshConcept MeshType>
bool isPerFaceColorEnabled(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceColorOptional(MeshType&);

// Face Mark

template<FaceMeshConcept MeshType>
bool isPerFaceMarkEnabled(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceMarkOptional(MeshType&);

// Face Normal

template<FaceMeshConcept MeshType>
bool isPerFaceNormalEnabled(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceNormalOptional(MeshType&);

// Face Principal Curvature

template<FaceMeshConcept MeshType>
bool isPerFacePrincipalCurvatureEnabled(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFacePrincipalCurvatureOptional(MeshType&);

// Face Quality

template<FaceMeshConcept MeshType>
bool isPerFaceQualityEnabled(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceQualityOptional(MeshType&);

// Face Wedge Colors

template<FaceMeshConcept MeshType>
bool isPerFaceWedgeColorsEnabled(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceWedgeColorsOptional(MeshType&);

// Face Wedge TexCoords

template<FaceMeshConcept MeshType>
bool isPerFaceWedgeTexCoordsEnabled(const MeshType&);

template<FaceMeshConcept MeshType>
bool enableIfPerFaceWedgeTexCoordsOptional(MeshType&);

/*********************
 * require functions *
 *********************/

// Face Compactness

template<FaceMeshConcept MeshType>
void requireFaceContainerCompactness(const MeshType& m);

// Face Adjacent Faces

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

// Face Principal Curvature

template<FaceMeshConcept MeshType>
void requirePerFacePrincipalCurvature(const MeshType& m)
	requires HasPerFacePrincipalCurvature<MeshType>;

// Face Quality

template<FaceMeshConcept MeshType>
void requirePerFaceQuality(const MeshType& m)
	requires HasPerFaceQuality<MeshType>;

// Face Wedge Colors

template<FaceMeshConcept MeshType>
void requirePerFaceWedgeColors(const MeshType& m)
	requires HasPerFaceWedgeColors<MeshType>;

// Face Wedge TexCoords

template<FaceMeshConcept MeshType>
void requirePerFaceWedgeTexCoords(const MeshType& m)
	requires HasPerFaceWedgeTexCoords<MeshType>;

} // namespace vcl

#include "face_requirements.cpp"

#endif // VCL_MESH_REQUIREMENTS_FACE_REQUIREMENTS_H
