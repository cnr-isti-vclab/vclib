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

#ifndef VCL_MESHES_H
#define VCL_MESHES_H

#include "meshes/edge_mesh.h"
#include "meshes/point_cloud.h"
#include "meshes/poly_mesh.h"
#include "meshes/tri_mesh.h"

/**
 * @defgroup meshes Meshes
 *
 * @brief List of predefined Mesh data structures that are available in VCLib.
 *
 * All the Mesh data structure have three configurations available:
 * - `MeshTypeNameT`: a Templated Mesh data structure that allows to select the
 * scalar data type with a template (e.g. `MeshTypeNameT<long double>`);
 * - `MeshTypeName`: The same Mesh data structure that uses, for all its stored
 * scalar data, the `double` type (equivalent to `MeshTypeNameT<double>`);
 * - `MeshTypeNamef`: The same Mesh data structure that uses, for all its stored
 * scalar data, the `float` type (equivalent to `MeshTypeNameT<float>`);
 */

#endif // VCL_MESHES_H
