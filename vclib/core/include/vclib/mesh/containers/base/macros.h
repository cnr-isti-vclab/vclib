// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_BASE_MACROS_H
#define VCL_MESH_CONTAINERS_BASE_MACROS_H

/**
 * @brief Macro that generates named availability, enable and disable methods
 * for a specific component on a container.
 *
 * It generates:
 * - is##Elem##CompName##Available()
 * - enable##Elem##CompName()
 * - disable##Elem##CompName()
 *
 * Example:
 *   VCL_CONTAINER_COMPONENT_METHODS(Vertex, Color, COLOR)
 * generates:
 *   bool isVertexColorAvailable() const { return isVertexComponentAvailable<CompId::COLOR>(); }
 *   void enableVertexColor() requires ... { enableVertexComponent<CompId::COLOR>(); }
 *   void disableVertexColor() requires ... { disableVertexComponent<CompId::COLOR>(); }
 *
 * TODO: Remove this macro when C++26 static reflection is available.
 */
#define VCL_CONTAINER_COMPONENT_METHODS(Elem, CompName, COMP_ID) \
    bool is##Elem##CompName##Available() const \
    { \
        return is##Elem##ComponentAvailable<CompId::COMP_ID>(); \
    } \
    void enable##Elem##CompName() \
        requires comp::HasOptionalComponent< \
            CompId::COMP_ID, \
            typename Base::Components> \
    { \
        enable##Elem##Component<CompId::COMP_ID>(); \
    } \
    void disable##Elem##CompName() \
        requires comp::HasOptionalComponent< \
            CompId::COMP_ID, \
            typename Base::Components> \
    { \
        disable##Elem##Component<CompId::COMP_ID>(); \
    }

/**
 * @brief Macro that generates methods for common components shared
 * across Vertex, Face, and Edge containers (Color, Normal, Quality).
 *
 * TODO: Remove this macro when C++26 static reflection is available.
 */
#define VCL_COMMON_CONTAINER_COMPONENT_METHODS(Elem) \
    VCL_CONTAINER_COMPONENT_METHODS(Elem, Color, COLOR) \
    VCL_CONTAINER_COMPONENT_METHODS(Elem, Normal, NORMAL) \
    VCL_CONTAINER_COMPONENT_METHODS(Elem, Quality, QUALITY)

#endif // VCL_MESH_CONTAINERS_BASE_MACROS_H
