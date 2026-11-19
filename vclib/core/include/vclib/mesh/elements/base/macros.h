// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEMENTS_BASE_MACROS_H
#define VCL_MESH_ELEMENTS_BASE_MACROS_H

#include <vclib/mesh/components/base/base.h>

/**
 * @brief Macro that generates named availability method
 * for a specific component on an ElementProxy.
 *
 * It generates:
 * - is##CompName##Available()
 *
 * Example:
 *   VCL_ELEMENT_COMPONENT_METHODS(Color, COLOR)
 * generates:
 *   bool isColorAvailable() const { return isComponentAvailable<CompId::COLOR>(); }
 *
 * TODO: Remove this macro when C++26 static reflection is available.
 */
#define VCL_ELEMENT_COMPONENT_METHODS(CompName, COMP_ID) \
    bool is##CompName##Available() const \
    { \
        return isComponentAvailable<CompId::COMP_ID>(); \
    }

/**
 * @brief Macro that generates methods for common components shared
 * across elements (Color, Normal, Quality).
 *
 * TODO: Remove this macro when C++26 static reflection is available.
 */
#define VCL_COMMON_ELEMENT_COMPONENT_METHODS \
    VCL_ELEMENT_COMPONENT_METHODS(Color, COLOR) \
    VCL_ELEMENT_COMPONENT_METHODS(Normal, NORMAL) \
    VCL_ELEMENT_COMPONENT_METHODS(Quality, QUALITY)

/**
 * @brief Macro that generates availability methods for all standard optional
 * components on ElementProxy.
 *
 * TODO: Remove this macro when C++26 static reflection is available.
 */
#define VCL_ALL_ELEMENT_COMPONENT_METHODS \
    VCL_COMMON_ELEMENT_COMPONENT_METHODS \
    VCL_ELEMENT_COMPONENT_METHODS(AdjacentEdges, ADJACENT_EDGES) \
    VCL_ELEMENT_COMPONENT_METHODS(AdjacentFaces, ADJACENT_FACES) \
    VCL_ELEMENT_COMPONENT_METHODS(AdjacentVertices, ADJACENT_VERTICES) \
    VCL_ELEMENT_COMPONENT_METHODS(MaterialIndex, MATERIAL_INDEX) \
    VCL_ELEMENT_COMPONENT_METHODS( \
        PrincipalCurvature, \
        PRINCIPAL_CURVATURE) \
    VCL_ELEMENT_COMPONENT_METHODS(Tangent, TANGENT) \
    VCL_ELEMENT_COMPONENT_METHODS(TexCoord, TEX_COORD) \
    VCL_ELEMENT_COMPONENT_METHODS(WedgeColors, WEDGE_COLORS) \
    VCL_ELEMENT_COMPONENT_METHODS(WedgeNormals, WEDGE_NORMALS) \
    VCL_ELEMENT_COMPONENT_METHODS(WedgeTexCoords, WEDGE_TEX_COORDS)

#define VCL_ELEMENT_PROXY_COMPONENT_METHODS VCL_ALL_ELEMENT_COMPONENT_METHODS

#endif // VCL_MESH_ELEMENTS_BASE_MACROS_H

