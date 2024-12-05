/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_MESH_REQUIREMENTS_EDGE_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_EDGE_REQUIREMENTS_H

#include "../containers/edge_container.h"
#include "element_requirements.h"

#include <vclib/concepts/mesh.h>
#include <vclib/exceptions/mesh.h>

/**
 * @defgroup edge_requirements Edge Requirements
 * @ingroup mesh_requirements
 *
 * @brief List functions that allows to check, enable and assert the
 * availability of components in the Edge Elements of a Mesh.
 */

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

/**
 * @brief Returns `true` if the given mesh has its EdgeContainer compact.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy
 * the EdgeMeshConcept.
 *
 * @param m: the mesh on which check the compactness of the EdgeContainer.
 * @return `true` if the EdgeContainer is compact.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool isEdgeContainerCompact(const MeshType& m)
{
    return isElementContainerCompact<ElemId::EDGE>(m);
}

// Edge AdjacentEdges

/**
 * @brief Returns true if the AdjacentEdges component is available (enabled) in
 * the Edge element of the input mesh m.
 *
 * This function returns `true` when the AdjacentEdges component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Edge Element does not have a AdjacentEdges Component, the
 *   function returns `false`;
 * - if the Edge Element has a non-optional AdjacentEdges Component, the
 *   function returns `true`;
 * - if the Edge Element has an optional AdjacentEdges Component, the function
 *   returns `true` if the AdjacentEdges component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the AdjacentEdges
 * Component in the Edge Element.
 * @return `true` if the AdjacentEdges Component is available in the Edge
 * Element of the given Mesh.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool isPerEdgeAdjacentEdgesAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::EDGE, CompId::ADJACENT_EDGES>(
        m);
}

/**
 * @brief If the input mesh has a EdgeContainer, and the Edge
 * Element has a AdjacentEdges Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the AdjacentEdges
 * component will be available in the Edge Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which enable the AdjacentEdges component in the
 * Edge Element.
 * @return `true` if the AdjacentEdges Component is available in the EdgeElement
 * after the call of this funciton.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeAdjacentEdgesOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<
        ElemId::EDGE,
        CompId::ADJACENT_EDGES>(m);
}

// Edge AdjacentFaces

/**
 * @brief Returns true if the AdjacentFaces component is available (enabled) in
 * the Edge element of the input mesh m.
 *
 * This function returns `true` when the AdjacentFaces component can be used on
 * the element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Edge Element does not have a AdjacentFaces Component, the
 *   function returns `false`;
 * - if the Edge Element has a non-optional AdjacentFaces Component, the
 *   function returns `true`;
 * - if the Edge Element has an optional AdjacentFaces Component, the function
 *   returns `true` if the AdjacentFaces component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the AdjacentFaces
 * Component in the Edge Element.
 * @return `true` if the AdjacentFaces Component is available in the Edge
 * Element of the given Mesh.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool isPerEdgeAdjacentFacesAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::EDGE, CompId::ADJACENT_FACES>(
        m);
}

/**
 * @brief If the input mesh has a EdgeContainer, and the Edge
 * Element has a AdjacentFaces Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the AdjacentFaces
 * component will be available in the Edge Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which enable the AdjacentFaces component in the
 * Edge Element.
 * @return `true` if the AdjacentFaces Component is available in the EdgeElement
 * after the call of this funciton.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeAdjacentFacesOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<
        ElemId::EDGE,
        CompId::ADJACENT_FACES>(m);
}

// Edge Color

/**
 * @brief Returns true if the Color component is available (enabled) in
 * the Edge element of the input mesh m.
 *
 * This function returns `true` when the Color component can be used on the
 * element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Edge Element does not have a Color Component, the
 *   function returns `false`;
 * - if the Edge Element has a non-optional Color Component, the
 *   function returns `true`;
 * - if the Edge Element has an optional Color Component, the function
 *   returns `true` if the Color component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the Color
 * Component in the Edge Element.
 * @return `true` if the Color Component is available in the Edge
 * Element of the given Mesh.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool isPerEdgeColorAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::EDGE, CompId::COLOR>(m);
}

/**
 * @brief If the input mesh has a EdgeContainer, and the Edge
 * Element has a Color Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the Color component
 * will be available in the Edge Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which enable the Color component in the
 * Edge Element.
 * @return `true` if the Color Component is available in the EdgeElement
 * after the call of this funciton.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeColorOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<ElemId::EDGE, CompId::COLOR>(m);
}

// Edge Mark

/**
 * @brief Returns true if the Mark component is available (enabled) in
 * the Edge element of the input mesh m.
 *
 * This function returns `true` when the Mark component can be used on the
 * element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Edge Element does not have a Mark Component, the
 *   function returns `false`;
 * - if the Edge Element has a non-optional Mark Component, the
 *   function returns `true`;
 * - if the Edge Element has an optional Mark Component, the function
 *   returns `true` if the Mark component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the Mark
 * Component in the Edge Element.
 * @return `true` if the Mark Component is available in the Edge
 * Element of the given Mesh.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool isPerEdgeMarkAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::EDGE, CompId::MARK>(m);
}

/**
 * @brief If the input mesh has a EdgeContainer, and the Edge
 * Element has a Mark Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the Mark component
 * will be available in the Edge Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which enable the Mark component in the
 * Edge Element.
 * @return `true` if the Mark Component is available in the EdgeElement
 * after the call of this funciton.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeMarkOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<ElemId::EDGE, CompId::MARK>(m);
}

// Edge Normal

/**
 * @brief Returns true if the Normal component is available (enabled) in
 * the Edge element of the input mesh m.
 *
 * This function returns `true` when the Normal component can be used on the
 * element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Edge Element does not have a Normal Component, the
 *   function returns `false`;
 * - if the Edge Element has a non-optional Normal Component, the
 *   function returns `true`;
 * - if the Edge Element has an optional Normal Component, the function
 *   returns `true` if the Normal component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the Normal
 * Component in the Edge Element.
 * @return `true` if the Normal Component is available in the Edge
 * Element of the given Mesh.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool isPerEdgeNormalAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::EDGE, CompId::NORMAL>(m);
}

/**
 * @brief If the input mesh has a EdgeContainer, and the Edge
 * Element has a Normal Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the Normal component
 * will be available in the Edge Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which enable the Normal component in the
 * Edge Element.
 * @return `true` if the Normal Component is available in the EdgeElement
 * after the call of this funciton.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeNormalOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<ElemId::EDGE, CompId::NORMAL>(m);
}

// Edge Quality

/**
 * @brief Returns true if the Quality component is available (enabled) in
 * the Edge element of the input mesh m.
 *
 * This function returns `true` when the Quality component can be used on the
 * element, whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Edge Element does not have a Quality Component, the
 *   function returns `false`;
 * - if the Edge Element has a non-optional Quality Component, the
 *   function returns `true`;
 * - if the Edge Element has an optional Quality Component, the function
 *   returns `true` if the Quality component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which check the availability of the Quality
 * Component in the Edge Element.
 * @return `true` if the Quality Component is available in the Edge
 * Element of the given Mesh.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool isPerEdgeQualityAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<ElemId::EDGE, CompId::QUALITY>(m);
}

/**
 * @brief If the input mesh has a EdgeContainer, and the Edge
 * Element has a Quality Component, this function enables the Component in
 * the Element if the component needs to be enabled (meaning that it is
 * optional).
 * Returns `true` if, after the call of this function, the Quality component
 * will be available in the Edge Element of the mesh.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] m: the mesh on which enable the Quality component in the
 * Edge Element.
 * @return `true` if the Quality Component is available in the EdgeElement
 * after the call of this funciton.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
bool enableIfPerEdgeQualityOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<ElemId::EDGE, CompId::QUALITY>(
        m);
}

/*********************
 * require functions *
 *********************/

/**
 * @brief This function asserts that a Mesh has the EdgeContainer compact
 * (no elements flagged as deleted).
 *
 * If the EdgeContainer is not compact, a
 * vcl::MissingCompactnessException will be thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @throws vcl::MissingCompactnessException if the EdgeContainer
 * is not compact.
 *
 * @param[in] m: the mesh on which check the compactness of the
 * EdgeContainer.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
void requireEdgeContainerCompactness(const MeshType& m)
{
    requireElementContainerCompactness<ElemId::EDGE>(m);
}

// Edge AdjacentEdges

/**
 * @brief This function asserts that a Mesh has a EdgeContainer,
 * the Edge has a AdjacentEdges Component, and that the AdjacentEdges Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Edge Container, the Edge has the AdjacentEdges Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @throws vcl::MissingComponentException if the AdjacentEdges Component is not
 * enabled in the EdgeContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the AdjacentEdges
 * Component in the Edge.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
void requirePerEdgeAdjacentEdges(const MeshType& m)
    requires HasPerEdgeAdjacentEdges<MeshType>
{
    requirePerElementComponent<ElemId::EDGE, CompId::ADJACENT_EDGES>(m);
}

// Edge AdjacentFaces

/**
 * @brief This function asserts that a Mesh has a EdgeContainer,
 * the Edge has a AdjacentFaces Component, and that the AdjacentFaces Component
 * is enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Edge Container, the Edge has the AdjacentFaces Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @throws vcl::MissingComponentException if the AdjacentFaces Component is not
 * enabled in the EdgeContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the AdjacentFaces
 * Component in the Edge.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
void requirePerEdgeAdjacentFaces(const MeshType& m)
    requires HasPerEdgeAdjacentFaces<MeshType>
{
    requirePerElementComponent<ElemId::EDGE, CompId::ADJACENT_FACES>(m);
}

// Edge Color

/**
 * @brief This function asserts that a Mesh has a EdgeContainer,
 * the Edge has a Color Component, and that the Color Component is
 * enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Edge Container, the Edge has the Color Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @throws vcl::MissingComponentException if the Color Component is not
 * enabled in the EdgeContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Color
 * Component in the Edge.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
void requirePerEdgeColor(const MeshType& m) requires HasPerEdgeColor<MeshType>
{
    requirePerElementComponent<ElemId::EDGE, CompId::COLOR>(m);
}

// Edge Mark

/**
 * @brief This function asserts that a Mesh has a EdgeContainer,
 * the Edge has a Mark Component, and that the Mark Component is
 * enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Edge Container, the Edge has the Mark Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @throws vcl::MissingComponentException if the Mark Component is not
 * enabled in the EdgeContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Mark
 * Component in the Edge.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
void requirePerEdgeMark(const MeshType& m) requires HasPerEdgeMark<MeshType>
{
    requirePerElementComponent<ElemId::EDGE, CompId::MARK>(m);
}

// Edge Normal

/**
 * @brief This function asserts that a Mesh has a EdgeContainer,
 * the Edge has a Normal Component, and that the Normal Component is
 * enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Edge Container, the Edge has the Normal Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @throws vcl::MissingComponentException if the Normal Component is not
 * enabled in the EdgeContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Normal
 * Component in the Edge.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
void requirePerEdgeNormal(const MeshType& m) requires HasPerEdgeNormal<MeshType>
{
    requirePerElementComponent<ElemId::EDGE, CompId::NORMAL>(m);
}

// Edge Quality

/**
 * @brief This function asserts that a Mesh has a EdgeContainer,
 * the Edge has a Quality Component, and that the Quality Component is
 * enabled and available at runtime.
 *
 * If the Mesh:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * a build error will be generated.
 *
 * If the Mesh:
 * - has the Edge Container, the Edge has the Quality Component,
 * but the Component is not enabled, a vcl::MissingComponentException will be
 * thrown.
 *
 * @tparam MeshType: the type of the Mesh to check, it must satisfy the
 * EdgeMeshConcept.
 *
 * @throws vcl::MissingComponentException if the Quality Component is not
 * enabled in the EdgeContainer of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Quality
 * Component in the Edge.
 *
 * @ingroup edge_requirements
 */
template<EdgeMeshConcept MeshType>
void requirePerEdgeQuality(const MeshType& m)
    requires HasPerEdgeQuality<MeshType>
{
    requirePerElementComponent<ElemId::EDGE, CompId::QUALITY>(m);
}

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_EDGE_REQUIREMENTS_H
