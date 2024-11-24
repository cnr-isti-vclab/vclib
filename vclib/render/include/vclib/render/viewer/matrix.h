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

#ifndef VCL_RENDER_VIEWER_MATRIX_H
#define VCL_RENDER_VIEWER_MATRIX_H

#include <vclib/space/core/matrix.h>
#include <vclib/space/core/point.h>

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

template<MatrixConcept Matrix44, typename Scalar>
Matrix44 projectionMatrix(
    Scalar     fov,
    Scalar     aspect,
    Scalar     nearPlane,
    Scalar     farPlane,
    bool       homogeneousNDC,
    Handedness handedness = RIGHT_HAND)
{
    Matrix44 res(4, 4);
    projectionMatrix(
        res.data(),
        fov,
        aspect,
        nearPlane,
        farPlane,
        homogeneousNDC,
        handedness);
    return res;
}

template<typename Scalar>
void projectionMatrixLeftHanded(
    auto*  res,
    Scalar fov,
    Scalar aspect,
    Scalar nearPlane,
    Scalar farPlane,
    bool   homogeneousNDC)
{
    Scalar h = 1.0 / std::tan(vcl::toRad(fov) * 0.5);
    Scalar w = h * 1.0 / aspect;
    projectionMatrix(
        res, fov, aspect, nearPlane, farPlane, homogeneousNDC, LEFT_HAND);
}

template<MatrixConcept Matrix44, typename Scalar>
Matrix44 projectionMatrixLeftHanded(
    Scalar fov,
    Scalar aspect,
    Scalar nearPlane,
    Scalar farPlane,
    bool   homogeneousNDC)
{
    Matrix44 res(4, 4);
    projectionMatrix(
        res.data(),
        fov,
        aspect,
        nearPlane,
        farPlane,
        homogeneousNDC,
        LEFT_HAND);
    return res;
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

template<MatrixConcept Matrix44, typename Scalar>
Matrix44 orthoProjectionMatrix(
    Scalar     left,
    Scalar     right,
    Scalar     top,
    Scalar     bottom,
    Scalar     nearPlane,
    Scalar     farPlane,
    bool       homogeneousNDC,
    Handedness handedness = RIGHT_HAND)
{
    Matrix44 res(4, 4);
    orthoProjectionMatrix(
        res.data(),
        left,
        right,
        top,
        bottom,
        nearPlane,
        farPlane,
        homogeneousNDC,
        handedness);
    return res;
}

/**
 * @brief Unprojects a screen position to a 3D point
 *
 * This function unprojects a screen position to a 3D point using the given
 * model view projection matrix, the viewport and a flag indicating if the NDC
 * coordinates are homogeneous.
 *
 * @param[in] screenPos: The screen position to unproject
 * @param[in] modelViewProjection: The model view projection matrix (or any
 * transform+projection matrix, depending on the space to unproject to)
 * @param[in] viewport: The viewport
 * @param[in] homogeneousNDC: Flag to indicate if the NDC coordinates are
 * homogeneous (i.e., z is in [-1,1] if true, or [0,1] otherwise)
 * @return The unprojected 3D point
 */
template<MatrixConcept Matrix44, Point3Concept PointType>
PointType unproject(
    const PointType& screenPos,
    const Matrix44&  modelViewProjection,
    const std::array<typename Matrix44::Scalar,4>& viewport,
    bool             homogeneousNDC)
{
    using Scalar = Matrix44::Scalar;
    const Matrix44 inv = modelViewProjection.inverse();
    Point4<Scalar> p(
        (screenPos.x() - viewport[0]) / viewport[2] * 2.0 - 1.0,
        (screenPos.y() - viewport[1]) / viewport[3] * 2.0 - 1.0,
        homogeneousNDC ? 2.0 * screenPos.z() - 1.0 : screenPos.z(),
        1.0);
    Point4<Scalar> res = inv * Eigen::Matrix<Scalar, 4, 1>(p);
    if (res.w() == 0.0)
    {
        std::cerr << "unproject: division by zero" << std::endl;
        return PointType::Zero();
    }
    return res.template head<3>() / res.w();
}

} // namespace vcl

#endif // VCL_RENDER_VIEWER_MATRIX_H
