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

#ifndef VCL_MESH_REQUIREMENTS_H
#define VCL_MESH_REQUIREMENTS_H

#include "requirements/mesh_requirements.h"

/**
 * The requirements file gives the user the possibility to check or require the existence of mesh
 * Elements or per-Element Components.
 *
 * There are three main types of functions:
 *
 * - hasSomething<MeshType>(): this is a constexpr function that checks at compile time if the mesh
 *     has some feature. These functions should be used in "if constexpr(...)" statements, e.g.:
 *
 *       if constexpr (vcl::hasTriangles<MyMesh>())
 *       {
 *           // portion of code that works on triangle meshes
 *       }
 *
 *       if constexpr (vcl::hasPerFaceColor<MyMesh>())
 *       {
 *          // portion of code that use per face colors
 *       }
 *
 *     Note: this function does not check for components that may be not enabled at runtime.
 *     For runtime check look for "isSomethingEnabled".
 *
 * - isSomethingEnabled(m): checks at compile time if an Element has a particular Component, and
 *     then checks at runtime if the Component is enabled. Therefore, the whole function is not a
 *     constexpr.
 *
 *     For example, if you have a portion of code that does something particular in case of Vertices
 *     have Colors, you could write:
 *
 *     if constexpr (vcl::hasPerVertexColor<MyMesh>()) {
 *         if (vcl::isPerVertexColorEnabled(m)) {
 *             // do something with vertex color
 *         }
 *     }
 *
 *     the first if constexpr is necessary in order to do not compile the portion of code below
 *     if the mesh has no vertex color. That will make your code work also when the input mesh will
 *     not have vertex colors.
 *     If you just need to know if there is vertex color without using it, it is safe to use just
 *     the isSomethingEnabled() function, like:
 *
 *     if (vcl::isPerVertexColorEnabled(m)) {
 *         std::cout << "My mesh has per vertex color!\n";
 *     }
 *
 * - enableIfSomethingOptional(): checks at compile time if an Element has a particular component,
 *     and if the component is optional, the function will enable it. The function will return true
 *     if the mesh has the component (also if not optional). The function is not a constexpr.
 *
 * - requireSomething(): requires some feature of the mesh. Can be both constexpr or non-constexpr.
 *     In case of features that can be checked at compile time, a static assertion will be
 *     performed. For features that can be checked only at runtime, a specific exception will be
 *     thrown if the requirement is not satisfied.
 **/

#endif // VCL_MESH_REQUIREMENTS_H
