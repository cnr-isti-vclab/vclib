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

#ifndef VCL_RENDER_VIEWER_CAMERA_H
#define VCL_RENDER_VIEWER_CAMERA_H

#include "matrix.h"

namespace vcl {

template<typename Scalar>
class Camera
{
public:
    using ScalarType = Scalar;
    using PointType  = vcl::Point3<Scalar>;
    using MatrixType = vcl::Matrix44<Scalar>;

    enum class ProjectionMode { ORTHO, PERSPECTIVE };

private:
    /* Extrinsics */

    /// @brief Position where the camera is looking at
    PointType mCenter = PointType(0.0f, 0.0f, 0.0f);

    /// @brief Position of (eye of) the camera
    PointType mEye = PointType(0.0f, 0.0f, 1.0f);

    /// @brief Up direction of the camera
    PointType mUp = PointType(0.0f, 1.0f, 0.0f);

    /* Intrinsics */

    /// @brief Field of view in degrees
    Scalar mFovDeg = 54.0;

    /// @brief Projection mode
    ProjectionMode mProjectionMode = ProjectionMode::PERSPECTIVE;

    /// @brief Height of the target in world space
    /// (used for ortho projection, and adapting the eye distance for
    /// perspective projection)
    Scalar mVerticalHeight = 2.0;

    /// @brief Aspect ratio
    Scalar mAspect = 1.0;

    /// @brief Z position of the mNear plane
    Scalar mNear = 0.1;

    /// @brief Z position of the mFar plane
    Scalar mFar = 500.0;

public:
    Camera() { setFieldOfViewAdaptingEyeDistance(mFovDeg); };

    void reset() { *this = {}; }

    PointType& center() { return mCenter; }

    const PointType& center() const { return mCenter; }

    PointType& eye() { return mEye; }

    const PointType& eye() const { return mEye; }

    PointType& up() { return mUp; }

    const PointType& up() const { return mUp; }

    Scalar& fieldOfView() { return mFovDeg; }

    const Scalar& fieldOfView() const { return mFovDeg; }

    void setFieldOfViewAdaptingEyeDistance(const Scalar& fov)
    {
        mFovDeg               = fov;
        PointType targetToEye = (mEye - mCenter).normalized();
        mEye = mCenter + targetToEye * ((mVerticalHeight / 2.0) /
                                        std::tan((fov / 2.0) / 180.0 * M_PI));
    }

    ProjectionMode& projectionMode() { return mProjectionMode; }

    ProjectionMode projectionMode() const { return mProjectionMode; }

    Scalar& verticalHeight() { return mVerticalHeight; }

    const Scalar& verticalHeight() const { return mVerticalHeight; }

    Scalar& aspectRatio() { return mAspect; }

    const Scalar& aspectRatio() const { return mAspect; }

    Scalar& nearPlane() { return mNear; }

    const Scalar& nearPlane() const { return mNear; }

    Scalar& farPlane() { return mFar; }

    const Scalar& farPlane() const { return mFar; }

    MatrixType viewMatrix() const
    {
        return lookAtMatrix<MatrixType>(mEye, mCenter, mUp);
    }

    MatrixType projectionMatrix() const
    {
        switch (mProjectionMode) {
        case ProjectionMode::ORTHO: {
            const Scalar h = mVerticalHeight / 2.0;
            const Scalar w = h * mAspect;
            return orthoProjectionMatrix<MatrixType>(
                -w, w, h, -h, mNear, mFar, false);
        }
        case ProjectionMode::PERSPECTIVE: {
            return vcl::projectionMatrix<MatrixType>(
                mFovDeg, mAspect, mNear, mFar, false);
        }
        default: assert(false); return MatrixType::Identity();
        }
    }
};

} // namespace vcl

#endif // VCL_RENDER_VIEWER_CAMERA_H
