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

#include "face_requirements.h"

#include <vclib/exceptions/mesh_exceptions.h>

#include "../containers/face_container.h"

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

/**
 * @brief Returns `true` if the given mesh has its Face Container compact.
 * @param[in] m: input mesh on which test if the its Face Container is compact.
 * @return `true` if the Face Container of the mesh is compact.
 */
template<FaceMeshConcept MeshType>
bool isFaceContainerCompact(const MeshType& m)
{
	return isElementContainerCompact<FACE>(m);
}

template<FaceMeshConcept MeshType>
bool isPerFaceAdjacentFacesEnabled(const MeshType& m)
{
	return isPerElementComponentEnabled<FACE, ADJACENT_FACES>(m);
}

template<FaceMeshConcept MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, ADJACENT_FACES>(m);
}

template<FaceMeshConcept MeshType>
bool isPerFaceColorEnabled(const MeshType& m)
{
	return isPerElementComponentEnabled<FACE, COLOR>(m);
}

template<FaceMeshConcept MeshType>
bool enableIfPerFaceColorOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, COLOR>(m);
}

template<FaceMeshConcept MeshType>
bool isPerFaceMarkEnabled(const MeshType& m)
{
	return isPerElementComponentEnabled<FACE, MARK>(m);
}

template<FaceMeshConcept MeshType>
bool enableIfPerFaceMarkOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, MARK>(m);
}

template<FaceMeshConcept MeshType>
bool isPerFaceNormalEnabled(const MeshType& m)
{
	return isPerElementComponentEnabled<FACE, NORMAL>(m);
}

template<FaceMeshConcept MeshType>
bool enableIfPerFaceNormalOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, NORMAL>(m);
}

template<FaceMeshConcept MeshType>
bool isPerFacePrincipalCurvatureEnabled(const MeshType& m)
{
	return isPerElementComponentEnabled<FACE, PRINCIPAL_CURVATURE>(m);
}

template<FaceMeshConcept MeshType>
bool enableIfPerFacePrincipalCurvatureOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, PRINCIPAL_CURVATURE>(m);
}

template<FaceMeshConcept MeshType>
bool isPerFaceQualityEnabled(const MeshType& m)
{
	return isPerElementComponentEnabled<FACE, QUALITY>(m);
}

template<FaceMeshConcept MeshType>
bool enableIfPerFaceQualityOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, QUALITY>(m);
}

template<FaceMeshConcept MeshType>
bool isPerFaceWedgeColorsEnabled(const MeshType& m)
{
	return isPerElementComponentEnabled<FACE, WEDGE_COLORS>(m);
}

template<FaceMeshConcept MeshType>
bool enableIfPerFaceWedgeColorsOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, WEDGE_COLORS>(m);
}

template<FaceMeshConcept MeshType>
bool isPerFaceWedgeTexCoordsEnabled(const MeshType& m)
{
	return isPerElementComponentEnabled<FACE, WEDGE_TEX_COORDS>(m);
}

template<FaceMeshConcept MeshType>
bool enableIfPerFaceWedgeTexCoordsOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, WEDGE_TEX_COORDS>(m);
}

/*********************
 * require functions *
 *********************/

template<FaceMeshConcept MeshType>
void requireFaceContainerCompactness(const MeshType& m)
{
	requireElementContainerCompactness<FACE>(m);
}

template<FaceMeshConcept MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m)
	requires HasPerFaceAdjacentFaces<MeshType>
{
	requirePerElementComponent<FACE, ADJACENT_FACES>(m);
}

template<FaceMeshConcept MeshType>
void requirePerFaceColor(const MeshType& m)
	requires HasPerFaceColor<MeshType>
{
	requirePerElementComponent<FACE, COLOR>(m);
}

template<FaceMeshConcept MeshType>
void requirePerFaceMark(const MeshType& m)
	requires HasPerFaceMark<MeshType>
{
	requirePerElementComponent<FACE, MARK>(m);
}

template<FaceMeshConcept MeshType>
void requirePerFaceNormal(const MeshType& m)
	requires HasPerFaceNormal<MeshType>
{
	requirePerElementComponent<FACE, NORMAL>(m);
}

template<FaceMeshConcept MeshType>
void requirePerFacePrincipalCurvature(const MeshType& m)
	requires HasPerFacePrincipalCurvature<MeshType>
{
	requirePerElementComponent<FACE, PRINCIPAL_CURVATURE>(m);
}

template<FaceMeshConcept MeshType>
void requirePerFaceQuality(const MeshType& m)
	requires HasPerFaceQuality<MeshType>
{
	requirePerElementComponent<FACE, QUALITY>(m);
}

template<FaceMeshConcept MeshType>
void requirePerFaceWedgeColors(const MeshType& m)
	requires HasPerFaceWedgeColors<MeshType>
{
	requirePerElementComponent<FACE, WEDGE_COLORS>(m);
}

template<FaceMeshConcept MeshType>
void requirePerFaceWedgeTexCoords(const MeshType& m)
	requires HasPerFaceWedgeTexCoords<MeshType>
{
	requirePerElementComponent<FACE, WEDGE_TEX_COORDS>(m);
}

} // namespace vcl
