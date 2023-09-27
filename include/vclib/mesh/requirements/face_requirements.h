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
#include <vclib/exceptions/mesh_exceptions.h>

#include "element_requirements.h"
#include "../containers/face_container.h"

/**
 * @defgroup face_requirements Face Requirements
 * @ingroup mesh_requirements
 *
 * @brief List functions that allows to check, enable and assert the
 * availability of components in the Face Elements of a Mesh.
 */

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

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

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/*************************
 * is/enableIf functions *
 *************************/

/**
 * @brief Returns `true` if the given mesh has its FaceContainer compact.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy
 * the FaceMeshConcept.
 *
 * @param m: the mesh on which check the compactness of the FaceContainer.
 * @return `true` if the FaceContainer is compact.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isFaceContainerCompact(const MeshType& m)
{
	return isElementContainerCompact<FACE>(m);
}

// Face AdjacentEdges

/**
 * @brief Returns true if the AdjacentEdges component is available (enabled) in
 * the Face element of the input mesh m.
 *
 * This function returns `true` when the AdjacentEdges component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a AdjacentEdges Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional AdjacentEdges Component, the
 *   function returns `true`;
 * - if the Face Element has an optional AdjacentEdges Component, the function
 *   returns `true` if the AdjacentEdges component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the AdjacentEdges
 * Component in the Face Element.
 * @return `true` if the AdjacentEdges Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFaceAdjacentEdgesAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, ADJACENT_EDGES>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a AdjacentEdges Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the AdjacentEdges
 * component will be available in the Face Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the AdjacentEdges component in the
 * Face Element.
 * @return `true` if the AdjacentEdges Component is available in the FaceElement
 * after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFaceAdjacentEdgesOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, ADJACENT_EDGES>(m);
}

// Face AdjacentFaces

/**
 * @brief Returns true if the AdjacentFaces component is available (enabled) in
 * the Face element of the input mesh m.
 *
 * This function returns `true` when the AdjacentFaces component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a AdjacentFaces Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional AdjacentFaces Component, the
 *   function returns `true`;
 * - if the Face Element has an optional AdjacentFaces Component, the function
 *   returns `true` if the AdjacentFaces component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the AdjacentFaces
 * Component in the Face Element.
 * @return `true` if the AdjacentFaces Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFaceAdjacentFacesAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, ADJACENT_FACES>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a AdjacentFaces Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the AdjacentFaces
 * component will be available in the Face Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the AdjacentFaces component in the
 * Face Element.
 * @return `true` if the AdjacentFaces Component is available in the FaceElement
 * after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFaceAdjacentFacesOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, ADJACENT_FACES>(m);
}

// Face Color

/**
 * @brief Returns true if the Color component is available (enabled) in
 * the Face element of the input mesh m.
 *
 * This function returns `true` when the Color component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a Color Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional Color Component, the
 *   function returns `true`;
 * - if the Face Element has an optional Color Component, the function
 *   returns `true` if the Color component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the Color
 * Component in the Face Element.
 * @return `true` if the Color Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFaceColorAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, COLOR>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a Color Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the Color
 * component will be available in the Face Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the Color component in the
 * Face Element.
 * @return `true` if the Color Component is available in the FaceElement
 * after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFaceColorOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, COLOR>(m);
}

// Face Mark

/**
 * @brief Returns true if the Mark component is available (enabled) in
 * the Face element of the input mesh m.
 *
 * This function returns `true` when the Mark component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a Mark Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional Mark Component, the
 *   function returns `true`;
 * - if the Face Element has an optional Mark Component, the function
 *   returns `true` if the Mark component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the Mark
 * Component in the Face Element.
 * @return `true` if the Mark Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFaceMarkAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, MARK>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a Mark Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the Mark
 * component will be available in the Face Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the Mark component in the
 * Face Element.
 * @return `true` if the Mark Component is available in the FaceElement
 * after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFaceMarkOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, MARK>(m);
}

// Face Normal

/**
 * @brief Returns true if the Normal component is available (enabled) in
 * the Face element of the input mesh m.
 *
 * This function returns `true` when the Normal component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a Normal Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional Normal Component, the
 *   function returns `true`;
 * - if the Face Element has an optional Normal Component, the function
 *   returns `true` if the Normal component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the Normal
 * Component in the Face Element.
 * @return `true` if the Normal Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFaceNormalAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, NORMAL>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a Normal Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the Normal
 * component will be available in the Face Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the Normal component in the
 * Face Element.
 * @return `true` if the Normal Component is available in the FaceElement
 * after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFaceNormalOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, NORMAL>(m);
}

// Face PrincipalCurvature

/**
 * @brief Returns true if the PrincipalCurvature component is available
 * (enabled) in the Face element of the input mesh m.
 *
 * This function returns `true` when the PrincipalCurvature component can be
 * used on the element, whether the component is horizontal, vertical or
 * optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a PrincipalCurvature Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional PrincipalCurvature Component, the
 *   function returns `true`;
 * - if the Face Element has an optional PrincipalCurvature Component, the
 * function returns `true` if the PrincipalCurvature component is enabled, false
 * otherwise (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the
 * PrincipalCurvature Component in the Face Element.
 * @return `true` if the PrincipalCurvature Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFacePrincipalCurvatureAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, PRINCIPAL_CURVATURE>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a PrincipalCurvature Component, this function enables the
 * Component in the Element if the component needs to be enabled (meaning that
 * it is optional). Returns `true` if, after the call of this function, the
 * PrincipalCurvature component will be available in the Face Element of the
 * mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the PrincipalCurvature component in
 * the Face Element.
 * @return `true` if the PrincipalCurvature Component is available in the
 * FaceElement after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFacePrincipalCurvatureOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, PRINCIPAL_CURVATURE>(m);
}

// Face Quality

/**
 * @brief Returns true if the Quality component is available (enabled) in
 * the Face element of the input mesh m.
 *
 * This function returns `true` when the Quality component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a Quality Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional Quality Component, the
 *   function returns `true`;
 * - if the Face Element has an optional Quality Component, the function
 *   returns `true` if the Quality component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the Quality
 * Component in the Face Element.
 * @return `true` if the Quality Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFaceQualityAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, QUALITY>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a Quality Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the Quality
 * component will be available in the Face Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the Quality component in the
 * Face Element.
 * @return `true` if the Quality Component is available in the FaceElement
 * after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFaceQualityOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, QUALITY>(m);
}

// Face WedgeColors

/**
 * @brief Returns true if the WedgeColors component is available (enabled) in
 * the Face element of the input mesh m.
 *
 * This function returns `true` when the WedgeColors component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a WedgeColors Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional WedgeColors Component, the
 *   function returns `true`;
 * - if the Face Element has an optional WedgeColors Component, the function
 *   returns `true` if the WedgeColors component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the WedgeColors
 * Component in the Face Element.
 * @return `true` if the WedgeColors Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFaceWedgeColorsAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, WEDGE_COLORS>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a WedgeColors Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the WedgeColors
 * component will be available in the Face Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the WedgeColors component in the
 * Face Element.
 * @return `true` if the WedgeColors Component is available in the FaceElement
 * after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFaceWedgeColorsOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, WEDGE_COLORS>(m);
}

// Face WedgeTexCoords

/**
 * @brief Returns true if the WedgeTexCoords component is available (enabled) in
 * the Face element of the input mesh m.
 *
 * This function returns `true` when the WedgeTexCoords component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Face Element does not have a WedgeTexCoords Component, the
 *   function returns `false`;
 * - if the Face Element has a non-optional WedgeTexCoords Component, the
 *   function returns `true`;
 * - if the Face Element has an optional WedgeTexCoords Component, the function
 *   returns `true` if the WedgeTexCoords component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the WedgeTexCoords
 * Component in the Face Element.
 * @return `true` if the WedgeTexCoords Component is available in the Face
 * Element of the given Mesh.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool isPerFaceWedgeTexCoordsAvailable(const MeshType& m)
{
	return isPerElementComponentAvailable<FACE, WEDGE_TEX_COORDS>(m);
}

/**
 * @brief If the input mesh has a FaceContainer, and the Face
 * Element has a WedgeTexCoords Component, this function enables the Component
 * in the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the WedgeTexCoords
 * component will be available in the Face Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] m: the mesh on which enable the WedgeTexCoords component in the
 * Face Element.
 * @return `true` if the WedgeTexCoords Component is available in the
 * FaceElement after the call of this funciton.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
bool enableIfPerFaceWedgeTexCoordsOptional(MeshType& m)
{
	return enableIfPerElementComponentOptional<FACE, WEDGE_TEX_COORDS>(m);
}


/*********************
 * require functions *
 *********************/

/**
 * @brief This function asserts that a Mesh has the FaceContainer compact
 * (no elements flagged as deleted).
 *
 * If the FaceContainer is not compact, a
 * vcl::MissingCompactnessException will be thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingCompactnessException if the FaceContainer
 * is not compact.
 *
 * @param[in] m: the mesh on which check the compactness of the
 * FaceContainer.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requireFaceContainerCompactness(const MeshType& m)
{
	requireElementContainerCompactness<FACE>(m);
}

// Face AdjacentEdges

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a AdjacentEdges Component, and that the AdjacentEdges Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the AdjacentEdges Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the AdjacentEdges Component is not
 * enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the AdjacentEdges
 * Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFaceAdjacentEdges(const MeshType& m)
	requires HasPerFaceAdjacentEdges<MeshType>
{
	requirePerElementComponent<FACE, ADJACENT_EDGES>(m);
}

// Face AdjacentFaces

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a AdjacentFaces Component, and that the AdjacentFaces Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the AdjacentFaces Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the AdjacentFaces Component is not
 * enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the AdjacentFaces
 * Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFaceAdjacentFaces(const MeshType& m)
	requires HasPerFaceAdjacentFaces<MeshType>
{
	requirePerElementComponent<FACE, ADJACENT_FACES>(m);
}

// Face Color

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a Color Component, and that the Color Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the Color Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the Color Component is not
 * enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Color
 * Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFaceColor(const MeshType& m)
	requires HasPerFaceColor<MeshType>
{
	requirePerElementComponent<FACE, COLOR>(m);
}

// Face Mark

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a Mark Component, and that the Mark Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the Mark Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the Mark Component is not
 * enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Mark
 * Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFaceMark(const MeshType& m)
	requires HasPerFaceMark<MeshType>
{
	requirePerElementComponent<FACE, MARK>(m);
}

// Face Normal

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a Normal Component, and that the Normal Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the Normal Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the Normal Component is not
 * enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Normal
 * Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFaceNormal(const MeshType& m)
	requires HasPerFaceNormal<MeshType>
{
	requirePerElementComponent<FACE, NORMAL>(m);
}

// Face PrincipalCurvature

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a PrincipalCurvature Component, and that the PrincipalCurvature
 * Component is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the PrincipalCurvature Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the PrincipalCurvature Component is
 * not enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the
 * PrincipalCurvature Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFacePrincipalCurvature(const MeshType& m)
	requires HasPerFacePrincipalCurvature<MeshType>
{
	requirePerElementComponent<FACE, PRINCIPAL_CURVATURE>(m);
}

// Face Quality

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a Quality Component, and that the Quality Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the Quality Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the Quality Component is not
 * enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Quality
 * Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFaceQuality(const MeshType& m)
	requires HasPerFaceQuality<MeshType>
{
	requirePerElementComponent<FACE, QUALITY>(m);
}

// Face WedgeColors

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a WedgeColors Component, and that the WedgeColors Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the WedgeColors Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the WedgeColors Component is not
 * enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the WedgeColors
 * Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFaceWedgeColors(const MeshType& m)
	requires HasPerFaceWedgeColors<MeshType>
{
	requirePerElementComponent<FACE, WEDGE_COLORS>(m);
}

// Face WedgeTexCoords

/**
 * @brief This function asserts that a Mesh has a FaceContainer,
 * the Face has a WedgeTexCoords Component, and that the WedgeTexCoords
 * Component is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Face Container, the Face has the WedgeTexCoords Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * FaceMeshConcept.
 *
 * @throws vcl::MissingComponentException if the WedgeTexCoords Component is not
 * enabled in the FaceContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the WedgeTexCoords
 * Component in the Face.
 *
 * @ingroup face_requirements
 */
template<FaceMeshConcept MeshType>
void requirePerFaceWedgeTexCoords(const MeshType& m)
	requires HasPerFaceWedgeTexCoords<MeshType>
{
	requirePerElementComponent<FACE, WEDGE_TEX_COORDS>(m);
}

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_FACE_REQUIREMENTS_H
