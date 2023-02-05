/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_COMPONENTS_H
#define VCL_MESH_COMPONENTS_H

#include "components/bounding_box.h"
#include "components/color.h"
#include "components/mark.h"
#include "components/name.h"
#include "components/texture_paths.h"
#include "components/transform_matrix.h"

namespace vcl::mesh {

/** Port BoundingBox class into mesh namespace **/
template<typename P>
using BoundingBox = comp::BoundingBox<P, void*, true>;

template <typename S>
using BoundingBox3 = comp::BoundingBox3<S, void*, true>;

using BoundingBox3f = comp::BoundingBox3f<void*, true>;
using BoundingBox3d = comp::BoundingBox3d<void*, true>;

/** Port Color class into mesh namespace **/
using Color = comp::Color;

/** Port Mark class into mesh namespace **/
using Mark = comp::Mark;

/** Port Name class into mesh namespace **/
using Name = comp::Name;

/** Port TexturePaths class into mesh namespace **/
using TexturePaths = comp::TexturePaths;

/** Port TransformMatrix class into mesh namespace **/
template <typename Scalar>
using TransformMatrix = comp::TransformMatrix<Scalar>;

using TransformMatrixf = comp::TransformMatrix<float>;
using TransformMatrixd = comp::TransformMatrix<double>;

} // namespace vcl::mesh

#endif // VCL_MESH_COMPONENTS_H
