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

#ifndef VCL_ALGORITHMS_UPDATE_BOUNDING_BOX_H
#define VCL_ALGORITHMS_UPDATE_BOUNDING_BOX_H

#include <vclib/algorithms/bounding_box.h>

namespace vcl {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

template<HasBoundingBox MeshType>
void updateBoundingBox(MeshType& m);

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

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

#endif // VCL_ALGORITHMS_UPDATE_BOUNDING_BOX_H
