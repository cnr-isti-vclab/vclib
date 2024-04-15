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

#ifndef VCL_ALGORITHMS_MESH_UPDATE_BOUNDING_BOX_H
#define VCL_ALGORITHMS_MESH_UPDATE_BOUNDING_BOX_H

#include <vclib/algorithms/mesh/bounding_box.h>

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
    m.boundingBox() = vcl::boundingBox(m);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_UPDATE_BOUNDING_BOX_H
