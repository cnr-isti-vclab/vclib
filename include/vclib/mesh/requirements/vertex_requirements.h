/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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
#include <vclib/exceptions/mesh_exceptions.h>

#include "../containers/vertex_container.h"
#include "element_requirements.h"

/**
 * @defgroup vertex_requirements Vertex Requirements
 * @ingroup mesh_requirements
 *
 * @brief List functions that allows to check, enable and assert the
 * availability of components in the Vertex Elements of a Mesh.
 */

namespace vcl {

/*************************
 * is/enableIf functions *
 *************************/

/**
 * @brief Returns `true` if the given mesh has its Vertex Container compact.
 *
 * @tparam MeshType: type of the mesh. It must satisfy the MeshConcept.
 * @param[in] m: input mesh on which test if the its Vertex Container is
 * compact.
 * @return `true` if the Vertex Container of the mesh is compact.
 *
 * @ingroup vertex_requirements
 */
template<MeshConcept MeshType>
bool isVertexContainerCompact(const MeshType& m)
{
    return isElementContainerCompact<VERTEX>(m);
}

template<MeshConcept MeshType>
bool isPerVertexAdjacentFacesAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<VERTEX, ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexAdjacentFacesOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<VERTEX, ADJACENT_FACES>(m);
}

template<MeshConcept MeshType>
bool isPerVertexAdjacentVerticesAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<VERTEX, ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexAdjacentVerticesOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<VERTEX, ADJACENT_VERTICES>(m);
}

template<MeshConcept MeshType>
bool isPerVertexColorAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<VERTEX, COLOR>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexColorOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<VERTEX, COLOR>(m);
}

template<MeshConcept MeshType>
bool isPerVertexMarkAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<VERTEX, MARK>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexMarkOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<VERTEX, MARK>(m);
}

template<MeshConcept MeshType>
bool isPerVertexNormalAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<VERTEX, NORMAL>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexNormalOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<VERTEX, NORMAL>(m);
}

template<MeshConcept MeshType>
bool isPerVertexPrincipalCurvatureAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<VERTEX, PRINCIPAL_CURVATURE>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexPrincipalCurvatureOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<VERTEX, PRINCIPAL_CURVATURE>(m);
}

template<MeshConcept MeshType>
bool isPerVertexQualityAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<VERTEX, QUALITY>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexQualityOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<VERTEX, QUALITY>(m);
}

template<MeshConcept MeshType>
bool isPerVertexTexCoordAvailable(const MeshType& m)
{
    return isPerElementComponentAvailable<VERTEX, TEX_COORD>(m);
}

template<MeshConcept MeshType>
bool enableIfPerVertexTexCoordOptional(MeshType& m)
{
    return enableIfPerElementComponentOptional<VERTEX, TEX_COORD>(m);
}

/*********************
 * require functions *
 *********************/

template<MeshConcept MeshType>
void requireVertexContainerCompactness(const MeshType& m)
{
    requireElementContainerCompactness<VERTEX>(m);
}

template<typename MeshType>
void requirePerVertexAdjacentFaces(const MeshType& m)
    requires HasPerVertexAdjacentFaces<MeshType>
{
    requirePerElementComponent<VERTEX, ADJACENT_FACES>(m);
}

template<typename MeshType>
void requirePerVertexAdjacentVertices(const MeshType& m)
    requires HasPerVertexAdjacentVertices<MeshType>
{
    requirePerElementComponent<VERTEX, ADJACENT_VERTICES>(m);
}

template<typename MeshType>
void requirePerVertexColor(const MeshType& m)
    requires HasPerVertexColor<MeshType>
{
    requirePerElementComponent<VERTEX, COLOR>(m);
}

template<typename MeshType>
void requirePerVertexMark(const MeshType& m) requires HasPerVertexMark<MeshType>
{
    requirePerElementComponent<VERTEX, MARK>(m);
}

template<typename MeshType>
void requirePerVertexNormal(const MeshType& m)
    requires HasPerVertexNormal<MeshType>
{
    requirePerElementComponent<VERTEX, NORMAL>(m);
}

template<typename MeshType>
void requirePerVertexPrincipalCurvature(const MeshType& m)
    requires HasPerVertexPrincipalCurvature<MeshType>
{
    requirePerElementComponent<VERTEX, PRINCIPAL_CURVATURE>(m);
}

template<typename MeshType>
void requirePerVertexQuality(const MeshType& m)
    requires HasPerVertexQuality<MeshType>
{
    requirePerElementComponent<VERTEX, QUALITY>(m);
}

template<typename MeshType>
void requirePerVertexTexCoord(const MeshType& m)
    requires HasPerVertexTexCoord<MeshType>
{
    requirePerElementComponent<VERTEX, TEX_COORD>(m);
}

} // namespace vcl

#endif // VCL_MESH_REQUIREMENTS_VERTEX_REQUIREMENTS_H
