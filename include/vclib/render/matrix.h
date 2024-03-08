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

#ifndef VCL_RENDER_MATRIX_H
#define VCL_RENDER_MATRIX_H

#include <vclib/space/matrix.h>
#include <vclib/space/point.h>

namespace vcl {

enum Handedness { LEFT_HAND, RIGHT_HAND };

namespace detail {

template<typename Scalar>
void projectionMatrixXYWH(
    auto*      res,
    Scalar     x,
    Scalar     y,
    Scalar     width,
    Scalar     height,
    Scalar     near,
    Scalar     far,
    bool       homogeneousNDC,
    Handedness handedness = RIGHT_HAND)
{
    Scalar diff = far - near;
    Scalar a    = homogeneousNDC ? (far + near) / diff : far / diff;
    Scalar b    = homogeneousNDC ? (2.0 * far * near) / diff : near * a;

    std::fill(res, res + 16, 0);

    res[0]  = width;
    res[5]  = height;
    res[8]  = handedness == RIGHT_HAND ? x : -x;
    res[9]  = handedness == RIGHT_HAND ? y : -y;
    res[10] = handedness == RIGHT_HAND ? -a : a;
    res[11] = handedness == RIGHT_HAND ? -1.0 : 1.0;
    res[14] = -b;
}

} // namespace detail

/**
 * @brief Creates a look at matrix
 *
 * This function creates a look at matrix and stores it in the given array,
 * that represents a 4x4 column major matrix.
 *
 * @param[out] res: The array to store the matrix in
 * @param[in] eye: The position of the camera
 * @param[in] center: The point the camera is looking at
 * @param[in] up: The up vector of the camera
 * @param[in] handedness: The handedness of the coordinate system
 *
 * @requires PointType::DIM == 3
 */
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

/**
 * @brief Creates and returns a look at matrix.
 *
 * This function creates a look at matrix and returns it.
 *
 * @param[in] eye: The position of the camera
 * @param[in] center: The point the camera is looking at
 * @param[in] up: The up vector of the camera
 * @param[in] handedness: The handedness of the coordinate system
 * @return The look at matrix
 *
 * @requires PointType::DIM == 3
 */
template<MatrixConcept Matrix44, Point3Concept PointType>
Matrix44 lookAtMatrix(
    const PointType& eye,
    const PointType& center,
    const PointType& up,
    Handedness       handedness = RIGHT_HAND)
{
    Matrix44 res(4, 4);
    lookAtMatrix(res.data(), eye, center, up, handedness);
    return res;
}

/**
 * @brief Creates a left handed look at matrix
 *
 * This function creates a look at matrix and stores it in the given array,
 * that represents a 4x4 column major matrix.
 *
 * @param[out] res: The array to store the matrix in
 * @param[in] eye: The position of the camera
 * @param[in] center: The point the camera is looking at
 * @param[in] up: The up vector of the camera
 *
 * @requires PointType::DIM == 3
 */
template<Point3Concept PointType>
void lookAtMatrixLeftHanded(
    auto*            res,
    const PointType& eye,
    const PointType& center,
    const PointType& up)
{
    lookAtMatrix(res, eye, center, up, LEFT_HAND);
}

/**
 * @brief Creates and returns a left handed look at matrix.
 *
 * This function creates a look at matrix and returns it.
 *
 * @param[in] eye: The position of the camera
 * @param[in] center: The point the camera is looking at
 * @param[in] up: The up vector of the camera
 * @return The look at matrix
 *
 * @requires PointType::DIM == 3
 */
template<MatrixConcept Matrix44, Point3Concept PointType>
Matrix44 lookAtMatrixLeftHanded(
    const PointType& eye,
    const PointType& center,
    const PointType& up)
{
    Matrix44 res(4, 4);
    lookAtMatrix(res.data(), eye, center, up, LEFT_HAND);
    return res;
}

template<typename Scalar>
void projectionMatrix(
    auto*      res,
    Scalar     fov,
    Scalar     aspect,
    Scalar     near,
    Scalar     far,
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
        near,
        far,
        homogeneousNDC,
        handedness);
}

template<MatrixConcept Matrix44, typename Scalar>
Matrix44 projectionMatrix(
    Scalar fov,
    Scalar aspect,
    Scalar near,
    Scalar far,
    bool         homogeneousNDC,
    Handedness   handedness = RIGHT_HAND)
{
    Matrix44 res(4, 4);
    projectionMatrix(
        res.data(), fov, aspect, near, far, homogeneousNDC, handedness);
    return res;
}

template<typename Scalar>
void projectionMatrixLeftHanded(
    auto*  res,
    Scalar fov,
    Scalar aspect,
    Scalar near,
    Scalar far,
    bool   homogeneousNDC)
{
    Scalar h = 1.0 / std::tan(vcl::toRad(fov) * 0.5);
    Scalar w = h * 1.0 / aspect;
    projectionMatrix(res, fov, aspect, near, far, homogeneousNDC, LEFT_HAND);
}

template<MatrixConcept Matrix44, typename Scalar>
Matrix44 projectionMatrixLeftHanded(
    Scalar fov,
    Scalar aspect,
    Scalar near,
    Scalar far,
    bool   homogeneousNDC)
{
    Matrix44 res(4, 4);
    projectionMatrix(
        res.data(), fov, aspect, near, far, homogeneousNDC, LEFT_HAND);
    return res;
}

} // namespace vcl

#endif // VCL_RENDER_MATRIX_H
