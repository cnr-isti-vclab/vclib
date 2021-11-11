/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_REQUIREMENTS_H
#define MGP_MESH_REQUIREMENTS_H

#include "requirements/mesh_requirements.h"

/**
 * The requirements file gives the user the possibility to check or require the existence of mesh
 * Elements or per-Element Components.
 *
 * There are three main types of functions:
 *
 * - hasSomething(): this is a constexpr function that checks at compile time if the mesh has some
 *     feature. These functions should be used in "if constexpr(...)" statements, e.g.:
 *
 *       if constexpr (mgp::hasTriangles(m))
 *       {
 *           // portion of code that works on triangle meshes
 *       }
 *
 *       if constexpr (mgp::hasPerFaceColor(m))
 *       {
 *          // portion of code that use per face colors
 *       }
 *
 *     Note: this function does not check for components that may be not enabled at runtime.
 *     For runtime check look for "isSomethingEnabled".
 *
 * - isSomethingEnabled(): checks at compile time if an Element has a particular Component, and then
 *     checks at runtime if the Component is enabled. Therefore, the whole function is not a
 *     constexpr.
 *
 *     For example, if you have a portion of code that does something particular in case of Vertices
 *     have Colors, you could write:
 *
 *     if constexpr (mgp::hasPerVertexColor(m)) {
 *         if (mgp::isPerVertexColorEnabled(m)) {
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
 *     if (mgp::isPerVertexColorEnabled(m)) {
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

#endif // MGP_MESH_REQUIREMENTS_H
