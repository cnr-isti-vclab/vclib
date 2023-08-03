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

#ifndef VCL_MESH_REQUIREMENTS_ELEMENT_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_ELEMENT_REQUIREMENTS_H

#include <vclib/concepts/mesh.h>
#include <vclib/exceptions/mesh_exceptions.h>

namespace vcl {

template<uint ELEMENT_TYPE, MeshConcept MeshType>
bool isElementContainerCompact(const MeshType&);

template<uint ELEMENT_TYPE, uint COMPONENT_TYPE, MeshConcept MeshType>
bool isPerElementComponentAvailable(const MeshType& m);

template<uint ELEMENT_TYPE, uint COMPONENT_TYPE, MeshConcept MeshType>
bool enableIfPerElementComponentOptional(MeshType& m);

template<uint ELEMENT_TYPE, MeshConcept MeshType>
void requireElementContainerCompactness(const MeshType& m);

template<uint ELEMENT_TYPE, uint COMPONENT_TYPE, MeshConcept MeshType>
void requirePerElementComponent(const MeshType& m);

} // namespace vcl

#include "element_requirements.cpp"

#endif // VCL_MESH_REQUIREMENTS_ELEMENT_REQUIREMENTS_H
