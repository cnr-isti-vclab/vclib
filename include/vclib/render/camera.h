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

#ifndef VCL_RENDER_CAMERA_H
#define VCL_RENDER_CAMERA_H

#include "matrix.h"

namespace vcl {

template<typename Scalar>
class Camera
{
public:
    enum ProjectionMode {
        Ortho,
        Perspective
    };

private:
    using PointType  = vcl::Point3<Scalar>;
    using MatrixType = vcl::Matrix44<Scalar>;

    // extrinsic parameters
    PointType centerPos = PointType(0.0f, 0.0f, 0.0f);
    PointType eyePos    = PointType(0.0f, 0.0f, 1.0f);
    PointType upDir     = PointType(0.0f, 1.0f, 0.0f);

    // intrinsic parameters
    Scalar fov    = 60.0;
    Scalar aspect = 1.0;
    Scalar near   = 0.1;
    Scalar far    = 500.0;

public:
    Camera() = default;

    void reset() { *this = {}; }

    PointType& center() { return centerPos; }

    const PointType& center() const { return centerPos; }

    PointType& eye() { return eyePos; }

    const PointType& eye() const { return eyePos; }

    PointType& up() { return upDir; }

    const PointType& up() const { return upDir; }

    Scalar& fieldOfView() { return fov; }

    const Scalar& fieldOfView() const { return fov; }

    Scalar& aspectRatio() { return aspect; }

    const Scalar& aspectRatio() const { return aspect; }

    Scalar& nearPlane() { return near; }

    const Scalar& nearPlane() const { return near; }

    Scalar& farPlane() { return far; }

    const Scalar& farPlane() const { return far; }

    MatrixType viewMatrix() const
    {
        return lookAtMatrix<MatrixType>(eyePos, centerPos, upDir);
    }

    MatrixType projMatrix() const
    {
        return projectionMatrix<MatrixType>(fov, aspect, near, far, false);
    }
};

} // namespace vcl

#endif // VCL_RENDER_CAMERA_H
