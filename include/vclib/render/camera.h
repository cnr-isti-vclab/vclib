/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_RENDER_CAMERA_H
#define VCL_RENDER_CAMERA_H

#include "matrix.h"

namespace vcl {

template<typename ScalarType>
class Camera
{
    using PointType = vcl::Point3<ScalarType>;
    using MatrixType = vcl::Matrix44<ScalarType>;

    PointType centerPos;
    PointType eyePos = PointType(0.0f, 0.0f, 1.0f);
    PointType upDir = PointType(0.0f, 1.0f, 0.0f);

    MatrixType mat = lookAtMatrix<MatrixType>(eyePos, centerPos, upDir);

public:
    Camera() = default;

    void reset() { *this = {}; }

    void updateMatrix() { mat = lookAtMatrix<MatrixType>(eyePos, centerPos, upDir); }

    PointType& center() { return centerPos; }
    const PointType& center() const { return centerPos; }

    PointType& eye() { return eyePos; }
    const PointType& eye() const { return eyePos; }

    PointType& up() { return upDir; }
    const PointType& up() const { return upDir; }

    MatrixType& matrix() { return mat; }
    const MatrixType& matrix() const { return mat; }
};

} // namespace vcl

#endif // VCL_RENDER_CAMERA_H
