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

#ifndef VCL_SPACE_CORE_CAMERA_MATRIX_H
#define VCL_SPACE_CORE_CAMERA_MATRIX_H

#include "../matrix.h"
#include "../point.h"

namespace vcl {

enum Handedness { LEFT_HAND, RIGHT_HAND };

namespace detail {

// creates a look at matrix and stores it in the given array, that represents a
// 4x4 column major matrix.
template<Point3Concept PointType>
void lookAtMatrix(
    auto*            res,
    const PointType& eye,
    const PointType& center,
    const PointType& up,
    Handedness       handedness = RIGHT_HAND)
{
    if (center != eye) {
        PointType zaxis = handedness == RIGHT_HAND ?
                              (eye - center).normalized() :
                              (center - eye).normalized();

        PointType xaxis = up.cross(zaxis);

        if (xaxis.dot(xaxis) == 0) {
            xaxis = handedness == RIGHT_HAND ? PointType(1, 0, 0) :
                                               PointType(-1, 0, 0);
        }
        else {
            xaxis = xaxis.normalized();
        }

        PointType yaxis = zaxis.cross(xaxis);

        res[0] = xaxis.x();
        res[1] = yaxis.x();
        res[2] = zaxis.x();
        res[3] = 0.0f;

        res[4] = xaxis.y();
        res[5] = yaxis.y();
        res[6] = zaxis.y();
        res[7] = 0.0f;

        res[8]  = xaxis.z();
        res[9]  = yaxis.z();
        res[10] = zaxis.z();
        res[11] = 0.0f;

        res[12] = -xaxis.dot(eye);
        res[13] = -yaxis.dot(eye);
        res[14] = -zaxis.dot(eye);
        res[15] = 1.0f;
    }
}

template<typename Scalar>
void projectionMatrixXYWH(
    auto*      res,
    Scalar     x,
    Scalar     y,
    Scalar     width,
    Scalar     height,
    Scalar     nearPlane,
    Scalar     farPlane,
    bool       homogeneousNDC,
    Handedness handedness = RIGHT_HAND)
{
    // note: don't use 'near' and 'far' variable names, as they are already
    // defined in windows.h headers
    Scalar diff = farPlane - nearPlane;
    Scalar a = homogeneousNDC ? (farPlane + nearPlane) / diff : farPlane / diff;
    Scalar b =
        homogeneousNDC ? (2.0 * farPlane * nearPlane) / diff : nearPlane * a;

    std::fill(res, res + 16, 0);

    res[0]  = width;
    res[5]  = height;
    res[8]  = handedness == RIGHT_HAND ? x : -x;
    res[9]  = handedness == RIGHT_HAND ? y : -y;
    res[10] = handedness == RIGHT_HAND ? -a : a;
    res[11] = handedness == RIGHT_HAND ? -1.0 : 1.0;
    res[14] = -b;
}

template<typename Scalar>
void projectionMatrix(
    auto*      res,
    Scalar     fov,
    Scalar     aspect,
    Scalar     nearPlane,
    Scalar     farPlane,
    bool       homogeneousNDC,
    Handedness handedness = RIGHT_HAND)
{
    Scalar h = 1.0 / std::tan(vcl::toRad(fov) * 0.5);
    Scalar w = h * 1.0 / aspect;
    detail::projectionMatrixXYWH(
        res,
        (Scalar) 0,
        (Scalar) 0,
        w,
        h,
        nearPlane,
        farPlane,
        homogeneousNDC,
        handedness);
}

template<typename Scalar>
void orthoProjectionMatrix(
    auto*      res,
    Scalar     left,
    Scalar     right,
    Scalar     top,
    Scalar     bottom,
    Scalar     nearPlane,
    Scalar     farPlane,
    bool       homogeneousNDC,
    Handedness handedness = RIGHT_HAND)
{
    // note: don't use 'near' and 'far' variable names, as they are already
    // defined in windows.h headers
    Scalar c = homogeneousNDC ? 2.0 / (farPlane - nearPlane) :
                                1.0 / (farPlane - nearPlane);
    Scalar f = homogeneousNDC ?
                   (farPlane + nearPlane) / (nearPlane - farPlane) :
                   nearPlane / (nearPlane - farPlane);

    std::fill(res, res + 16, 0);
    res[0]  = 2.0 / (right - left);
    res[5]  = 2.0 / (top - bottom);
    res[10] = RIGHT_HAND ? -c : c;
    res[12] = (right + left) / (left - right);
    res[13] = (bottom + top) / (bottom - top);
    res[14] = f;
    res[15] = 1.0;
}

} // namespace detail

} // namespace vcl

#endif // VCL_SPACE_CORE_CAMERA_MATRIX_H
