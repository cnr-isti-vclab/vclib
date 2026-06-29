// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESHES_H
#define VCL_MESHES_H

#include "meshes/edge_mesh.h"
#include "meshes/point_cloud.h"
#include "meshes/poly_edge_mesh.h"
#include "meshes/poly_mesh.h"
#include "meshes/tri_edge_mesh.h"
#include "meshes/tri_mesh.h"

/**
 * @defgroup meshes Meshes
 *
 * @brief List of predefined Mesh data structures that are available in VCLib.
 *
 * All the Mesh data structure have three configurations available:
 * - `MeshTypeNameT`: a Templated Mesh data structure that allows to select the
 * scalar data type  and whether the mesh uses pointers or indices to store
 * references, with templates (e.g. `MeshTypeNameT<long double, true>`);
 * - `MeshTypeName`: The same Mesh data structure that uses, for all its stored
 * scalar data, the `double` type, and pointers to store references (equivalent
 * to `MeshTypeNameT<double, false>`);
 * - `MeshTypeNamef`: The same Mesh data structure that uses, for all its stored
 * scalar data, the `float` type, and pointers to store references (equivalent
 * to `MeshTypeNameT<float, false>`);
 * - `MeshTypeNameIndexed`: The same Mesh data structure that uses, for all its
 * stored scalar data, the `double` type, and indices to store references
 * (equivalent to `MeshTypeNameT<double, true>`);
 * - `MeshTypeNameIndexedf`: The same Mesh data structure that uses, for all its
 * stored scalar data, the `float` type, and indices to store references
 * (equivalent to `MeshTypeNameT<float, true>`).
 */

#endif // VCL_MESHES_H
