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

#ifndef VCL_MESH_COMPONENTS_H
#define VCL_MESH_COMPONENTS_H

#include "components/bounding_box.h"
#include "components/mark.h"
#include "components/transform_matrix.h"

namespace vcl::mesh {

/** Port BoundingBox class into mesh namespace **/
template<typename P>
using BoundingBox = comp::BoundingBox<P>;

using BoundingBox3f = comp::BoundingBox3f;
using BoundingBox3d = comp::BoundingBox3d;

template<typename T>
bool constexpr hasBoundingBox()
{
	return comp::hasBoundingBox<T>();
}

/** Port Mark class into mesh namespace **/
using Mark = comp::Mark;

template<typename T>
bool constexpr hasMark()
{
	return comp::hasMark<T>();
}

/** Port TransformMatrix class into mesh namespace **/
template <typename Scalar>
using TransformMatrix = comp::TransformMatrix<Scalar>;

using TransformMatrixf = comp::TransformMatrix<float>;
using TransformMatrixd = comp::TransformMatrix<double>;

template<typename T>
bool constexpr hasTransformMatrix()
{
	return comp::hasTransformMatrix<T>();
}

} // namespace vcl::mesh

#endif // VCL_MESH_COMPONENTS_H
