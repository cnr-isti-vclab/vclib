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

#ifndef VCL_MESH_FACE_REQUIREMENTS_H
#define VCL_MESH_FACE_REQUIREMENTS_H

#include "element_requirements.h"
#include "face_concepts.h"

namespace vcl {

/********************
 * is/has functions *
 ********************/

// Face Compactness

template<typename MeshType>
bool isFaceContainerCompact(const MeshType&);

// Face Adjacent Faces

template<typename MeshType>
bool constexpr hasPerFaceAdjacentFaces();

template<typename MeshType>
bool isPerFaceAdjacentFacesEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType&);

// Face Color

template<typename MeshType>
bool constexpr hasPerFaceColor();

template<typename MeshType>
bool isPerFaceColorEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerFaceColorOptional(MeshType&);

// Face Mark

template<typename MeshType>
bool constexpr hasPerFaceMark();

template<typename MeshType>
bool isPerFaceMarkEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerFaceMarkOptional(MeshType&);

// Face Normal

template<typename MeshType>
bool constexpr hasPerFaceNormal();

template<typename MeshType>
bool isPerFaceNormalEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerFaceNormalOptional(MeshType&);

// Face Principal Curvature

template<typename MeshType>
bool constexpr hasPerFacePrincipalCurvature();

template<typename MeshType>
bool isPerFacePrincipalCurvatureEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerFacePrincipalCurvatureOptional(MeshType&);

// Face Scalar

template<typename MeshType>
bool constexpr hasPerFaceScalar();

template<typename MeshType>
bool isPerFaceScalarEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerFaceScalarOptional(MeshType&);

// Face Wedge Colors

template<typename MeshType>
bool constexpr hasPerFaceWedgeColors();

template<typename MeshType>
bool isPerFaceWedgeColorsEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerFaceWedgeColorsOptional(MeshType&);

// Face Wedge TexCoords

template<typename MeshType>
bool constexpr hasPerFaceWedgeTexCoords();

template<typename MeshType>
bool isPerFaceWedgeTexCoordsEnabled(const MeshType&);

template<typename MeshType>
bool enableIfPerFaceWedgeTexCoordsOptional(MeshType&);

// Face Custom Components

template<typename MeshType>
bool constexpr hasPerFaceCustomComponents();

/*********************
 * require functions *
 *********************/

// Face Compactness

template<typename MeshType>
void requireFaceContainerCompactness(const MeshType& m);

// Face Adjacent Faces

template<typename MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m);

// Face Color

template<typename MeshType>
void requirePerFaceColor(const MeshType& m);

// Face Color

template<typename MeshType>
void requirePerFaceMark(const MeshType& m);

// Face Normal

template<typename MeshType>
void requirePerFaceNormal(const MeshType& m);

// Face Principal Curvature

template<typename MeshType>
void requirePerFacePrincipalCurvature(const MeshType& m);

// Face Scalar

template<typename MeshType>
void requirePerFaceScalar(const MeshType& m);

// Face Wedge Colors

template<typename MeshType>
void requirePerFaceWedgeColors(const MeshType& m);

// Face Wedge TexCoords

template<typename MeshType>
void requirePerFaceWedgeTexCoords(const MeshType& m);

// Face Custom Components

template<typename MeshType>
bool constexpr requirePerFaceCustomComponents();

template<typename MeshType>
bool requirePerFaceCustomComponents(const MeshType& m);

} // namespace vcl

#include "face_requirements.cpp"

#endif // VCL_MESH_FACE_REQUIREMENTS_H
