/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_SPACE_CORE_MATRIX_AFFINE_H
#define VCL_SPACE_CORE_MATRIX_AFFINE_H

#include <vclib/concepts/space/matrix.h>
#include <vclib/concepts/space/point.h>

#include <Eigen/Core>

namespace vcl {

template<typename Scalar>
using Affine3 = Eigen::Transform<Scalar, 3, Eigen::Affine>;

using Affine3f = Affine3<float>;
using Affine3d = Affine3<double>;

} // namespace vcl

#endif // VCL_SPACE_CORE_MATRIX_AFFINE_H
