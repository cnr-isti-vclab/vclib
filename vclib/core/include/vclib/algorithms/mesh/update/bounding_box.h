// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_UPDATE_BOUNDING_BOX_H
#define VCL_ALGORITHMS_MESH_UPDATE_BOUNDING_BOX_H

#include <vclib/algorithms/mesh/stat/bounding_box.h>

namespace vcl {

/**
 * @brief Updates the bounding box of the mesh.
 *
 * @tparam MeshType: type of the input mesh. It must satisfy the HasBoundingBox
 * concept.
 *
 * @param[in] m: input mesh on which the bounding box is computed and updated.
 *
 * @ingroup update
 */
template<HasBoundingBox MeshType>
void updateBoundingBox(MeshType& m)
{
    m.boundingBox() = boundingBox(m);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_BOUNDING_BOX_H
