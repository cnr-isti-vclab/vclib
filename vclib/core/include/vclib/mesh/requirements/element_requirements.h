/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_MESH_REQUIREMENTS_ELEMENT_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_ELEMENT_REQUIREMENTS_H

#include <vclib/concepts.h>
#include <vclib/exceptions.h>

namespace vcl {

/**
 * @brief Returns `true` if the given mesh has its Container of the given
 * Element compact.
 *
 * @tparam ELEM_ID: the Element ID of the ElementContainer to check.
 * @tparam MeshType: the type of the Mesh to check. It must satisfy the
 * MeshConcept.
 *
 * @param[in] m: the mesh on which check the compactness of the Container of the
 * given Element.
 * @return `true` if the Container of the given Element is compact.
 *
 * @ingroup mesh_requirements
 */
template<uint ELEM_ID, MeshConcept MeshType>
bool isElementContainerCompact(const MeshType& m)
{
    return (
        m.template number<ELEM_ID>() == m.template containerSize<ELEM_ID>());
}

/**
 * @brief Returns true if the given component is available in the given element
 * of the input mesh m.
 *
 * This function returns `true` when the component can be used on the element,
 * whether the component is horizontal, vertical or optional.
 *
 * These are the following cases:
 * - if the Mesh does not have an ElementContainer of the given Element ID,
 *   the function returns `false`;
 * - if the Element does not have a Component of the given Component ID,
 *   the function returns `false`;
 * - if the Element has a non-optional Component of the given Component ID,
 *   the function returns `true`;
 * - if the Element has an optional Component of the given Component ID,
 *   the function returns `true` if the component is enabled, false otherwise
 *   (this check is the only one that is made at runtime);
 *
 * @tparam ELEM_ID: the ID of the Element to check.
 * @tparam COMP_ID: the ID of the Component to check.
 * @tparam MeshType: the type of the Mesh to check.
 *
 * @param[in] m: the mesh on which check the availability of the Component in
 * the Element.
 * @return `true` if the Component is available in the Element of the given
 * Mesh.
 *
 * @ingroup mesh_requirements
 */
template<uint ELEM_ID, uint COMP_ID, MeshConcept MeshType>
bool isPerElementComponentAvailable(const MeshType& m)
{
    if constexpr (mesh::HasElementContainer<MeshType, ELEM_ID>) {
        using Container = mesh::ContainerOfElementType<ELEM_ID, MeshType>;
        using Element   = Container::ElementType;
        if constexpr (comp::HasOptionalComponentOfType<Element, COMP_ID>) {
            return m.template isPerElementComponentEnabled<ELEM_ID, COMP_ID>();
        }
        else {
            return comp::HasComponentOfType<Element, COMP_ID>;
        }
    }
    else {
        return false;
    }
}

/**
 * @brief Makes available the given Component in the given Element of the input
 * mesh m, and returns `true` if it succeeds.
 *
 * If the input mesh has a Container of the given Element ID, and the Element
 * has a Component of the given Component ID, this function enables the
 * Component in the Element if the component needs to be enabled. Returns `true`
 * if, after the call of this function, the component will be available in the
 * Element of the mesh.
 *
 * @tparam ELEM_ID: the ID of the Element on which enable the component.
 * @tparam COMP_ID: the ID of the Component to enable.
 * @tparam MeshType: the type of the Mesh to check.
 *
 * @param[in] m: the mesh on which enable the component in the Element.
 * @return `true` if the Component is available in the Element after the call of
 * this funciton.
 *
 * @ingroup mesh_requirements
 */
template<uint ELEM_ID, uint COMP_ID, MeshConcept MeshType>
bool enableIfPerElementComponentOptional(MeshType& m)
{
    if constexpr (mesh::HasElementContainer<MeshType, ELEM_ID>) {
        using Container = mesh::ContainerOfElementType<ELEM_ID, MeshType>;
        using Element   = Container::ElementType;
        if constexpr (comp::HasOptionalComponentOfType<Element, COMP_ID>) {
            m.template enablePerElementComponent<ELEM_ID, COMP_ID>();
            return true;
        }
        else {
            return comp::HasComponentOfType<Element, COMP_ID>;
        }
    }
    else {
        return false;
    }
}

/**
 * @brief This function asserts that a Mesh has the Container of the given
 * Element ID compact (no elements flagged as deleted).
 *
 * If the Container of the given Element ID is not compact, a
 * vcl::MissingCompactnessException will be thrown. If the mesh has not a
 * Container of the given Element ID, a build error will be raised.
 *
 * @tparam ELEM_ID: the Element ID of the ElementContainer to check.
 * @tparam MeshType: the type of the Mesh to check.
 *
 * @throws vcl::MissingCompactnessException if the Container of the given
 * ElementEnumType is not compact.
 *
 * @param[in] m: the mesh on which check the compactness of the Container of the
 * given Element.
 *
 * @ingroup mesh_requirements
 */
template<uint ELEM_ID, MeshConcept MeshType>
void requireElementContainerCompactness(const MeshType& m)
{
    if (!isElementContainerCompact<ELEM_ID>(m))
        throw MissingCompactnessException(
            elementEnumString<ELEM_ID>() +
            " Container of the Mesh is not compact.");
}

/**
 * @brief This function asserts that a Mesh has the Container of the given
 * Element ID, the Element has a Component of the given Component ID,
 * and that Component is available at runtime.
 *
 * If the Mesh does not satisfy one of these conditions, a
 * vcl::MissingComponentException will be thrown:
 * - has not a Container of the given ElementEnumType;
 * - has the Container but the Element has not a Component of the given
 *   ComponentEnumType;
 * - has the Container, the Element has the Component, but the Component is not
 *   enabled.
 *
 * @tparam ELEM_ID: the ID of the Element to check.
 * @tparam COMP_ID: the ID of the Component to check.
 * @tparam MeshType: the type of the Mesh to check.
 *
 * @throws vcl::MissingComponentException if the Component is not enabled or
 * available in the Element Container of the Mesh.
 *
 * @param[in] m: the mesh on which check the availability of the Component in
 * the Element.
 *
 * @todo fix this - add compile time requirements
 *
 * @ingroup mesh_requirements
 */
template<uint ELEM_ID, uint COMP_ID, MeshConcept MeshType>
void requirePerElementComponent(const MeshType& m)
{
    if (!isPerElementComponentAvailable<ELEM_ID, COMP_ID>(m)) {
        throw MissingComponentException(
            "Per " + std::string(elementEnumString<ELEM_ID>()) + " " +
            std::string(componentEnumString<COMP_ID>()) +
            " Component is not enabled.");
    }
}

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_ELEMENT_REQUIREMENTS_H
