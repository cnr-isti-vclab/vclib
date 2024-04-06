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
    struct ProjectionMode
    {
        enum Enum { ORTHO, PERSPECTIVE };
    };

private:
    using PointType  = vcl::Point3<Scalar>;
    using MatrixType = vcl::Matrix44<Scalar>;

    /* Extrinsics */

    /// @brief Position where the camera is looking at
    PointType m_center = PointType(0.0f, 0.0f, 0.0f);

    /// @brief Position of (eye of) the camera
    PointType m_eye = PointType(0.0f, 0.0f, 1.0f);

    /// @brief Up direction of the camera
    PointType m_up = PointType(0.0f, 1.0f, 0.0f);

    /* Intrinsics */

    /// @brief Field of view in degrees
    Scalar m_fovDeg = 60.0;

    /// @brief Projection mode
    ProjectionMode::Enum m_projectionMode = ProjectionMode::PERSPECTIVE;

    /// @brief Height of the target in world space
    /// (used for ortho projection, and adapting the eye distance for
    /// perspective projection)
    Scalar m_verticalHeight = 1.0;

    /// @brief Aspect ratio
    Scalar m_aspect = 1.0;

    /// @brief Z position of the m_near plane
    Scalar m_near = 0.1;

    /// @brief Z position of the m_far plane
    Scalar m_far = 500.0;

public:
    Camera() { setFieldOfViewAdaptingEyeDistance(m_fovDeg); };

    void reset() { *this = {}; }

    PointType& center() { return m_center; }

    const PointType& center() const { return m_center; }

    PointType& eye() { return m_eye; }

    const PointType& eye() const { return m_eye; }

    PointType& up() { return m_up; }

    const PointType& up() const { return m_up; }

    Scalar& fieldOfView() { return m_fovDeg; }

    const Scalar& fieldOfView() const { return m_fovDeg; }

    void setFieldOfViewAdaptingEyeDistance(const Scalar& fov)
    {
        m_fovDeg = fov;
        PointType targetToEye = (m_eye - m_center).normalized();
        m_eye =
            m_center + targetToEye * ((m_verticalHeight / 2.0) /
                                       std::tan((fov / 2.0) / 180.0 * M_PI));
    }

    ProjectionMode::Enum& projectionMode() { return m_projectionMode; }

    ProjectionMode::Enum projectionMode() const { return m_projectionMode; }

    Scalar& verticalHeight() { return m_verticalHeight; }

    const Scalar& verticalHeight() const { return m_verticalHeight; }

    Scalar& aspectRatio() { return m_aspect; }

    const Scalar& aspectRatio() const { return m_aspect; }

    Scalar& nearPlane() { return m_near; }

    const Scalar& nearPlane() const { return m_near; }

    Scalar& farPlane() { return m_far; }

    const Scalar& farPlane() const { return m_far; }

    MatrixType viewMatrix() const
    {
        return lookAtMatrix<MatrixType>(m_eye, m_center, m_up);
    }

    MatrixType projMatrix() const
    {
        switch (m_projectionMode) {
        case ProjectionMode::PERSPECTIVE : {
            return projectionMatrix<MatrixType>(
                m_fovDeg, m_aspect, m_near, m_far, false);
        }
        case ProjectionMode::ORTHO : {
            const Scalar h = m_verticalHeight / 2.0;
            const Scalar w = h * m_aspect;
            return orthoProjectionMatrix<MatrixType>(
                -w, w, -h, h, m_near, m_far, false);
            break;
        }
        default:
            assert(false);
            return MatrixType::Identity();
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_CAMERA_H
