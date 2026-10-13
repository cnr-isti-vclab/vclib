// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_CORE_CAMERA_H
#define VCL_SPACE_CORE_CAMERA_H

#include "camera/matrix.h"

namespace vcl {

/**
 * @brief A Pinhole camera model.
 *
 * @ingroup space_core
 */
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

    /// @brief Position where the camera is looking at (i.e. target point)
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
    //      |\___
    //      |     \___
    //  h/2 |         \___
    //      |             \__
    //      |                \__
    //--- target ---------------X- eye --
    //      |              __/
    //      |           __/
    //  h/2 |        __/
    //      |     __/
    //      |  __/
    //      | /
    Scalar mVerticalHeight = 2.0;

    /// @brief Aspect ratio
    Scalar mAspect = 1.0;

    /// @brief Z position of the mNear plane
    Scalar mNear = 0.1;

    /// @brief Z position of the mFar plane
    Scalar mFar = 500.0;

public:
    Camera() { setFieldOfViewAdaptingEyeDistance(mFovDeg); };

    /**
     * @brief Resets the camera parameters to their default values.
     */
    void reset() { *this = {}; }

    /**
     * @brief Returns a reference to the camera center (look-at target point).
     *
     * @return Reference to the center point.
     */
    decltype(auto) center(this auto&& self) { return (self.mCenter); }

    /**
     * @brief Returns a reference to the camera eye (position) point.
     *
     * @return Reference to the eye point.
     */
    decltype(auto) eye(this auto&& self) { return (self.mEye); }

    /**
     * @brief Returns a reference to the camera up vector.
     *
     * @return Reference to the up vector.
     */
    decltype(auto) up(this auto&& self) { return (self.mUp); }

    /**
     * @brief Returns a reference to the vertical field of view in degrees.
     *
     * @return Reference to the field of view angle in degrees.
     */
    decltype(auto) fieldOfView(this auto&& self) { return (self.mFovDeg); }

    /**
     * @brief Sets the field of view in degrees while adjusting the eye distance
     * to preserve the visible vertical extent at the target center.
     *
     * @param[in] fovDeg: New field of view in degrees.
     */
    void setFieldOfViewAdaptingEyeDistance(const Scalar& fovDeg)
    {
        mFovDeg               = fovDeg;
        PointType targetToEye = (mEye - mCenter).normalized();
        mEye =
            mCenter + targetToEye * ((mVerticalHeight / 2.0) /
                                     std::tan((fovDeg / 2.0) / 180.0 * M_PI));
    }

    /**
     * @brief Returns a reference to the projection mode (Perspective or
     * Orthographic).
     *
     * @return Reference to the projection mode.
     */
    decltype(auto) projectionMode(this auto&& self)
    {
        return (self.mProjectionMode);
    }

    /**
     * @brief Returns a reference to the orthographic vertical height.
     *
     * @return Reference to the vertical height.
     */
    decltype(auto) verticalHeight(this auto&& self)
    {
        return (self.mVerticalHeight);
    }

    /**
     * @brief Returns a reference to the aspect ratio (width / height).
     *
     * @return Reference to the aspect ratio.
     */
    decltype(auto) aspectRatio(this auto&& self) { return (self.mAspect); }

    /**
     * @brief Returns a reference to the distance of the near clipping plane.
     *
     * @return Reference to the near plane distance.
     */
    decltype(auto) nearPlane(this auto&& self) { return (self.mNear); }

    /**
     * @brief Returns a reference to the distance of the far clipping plane.
     *
     * @return Reference to the far plane distance.
     */
    decltype(auto) farPlane(this auto&& self) { return (self.mFar); }

    MatrixType viewMatrix() const
    {
        MatrixType res;
        detail::lookAtMatrix(res.data(), mEye, mCenter, mUp);
        return res;
    }

    MatrixType projectionMatrix() const
    {
        MatrixType res;
        switch (mProjectionMode) {
        case ProjectionMode::ORTHO: {
            const Scalar h = mVerticalHeight / 2.0;
            const Scalar w = h * mAspect;
            detail::orthoProjectionMatrix(
                res.data(), -w, w, h, -h, mNear, mFar, false);
            break;
        }
        case ProjectionMode::PERSPECTIVE: {
            detail::projectionMatrix(
                res.data(), mFovDeg, mAspect, mNear, mFar, false);
            break;
        }
        default: assert(false); return MatrixType::Identity();
        }
        return res;
    }

    /**
     * @brief Serializes the camera to the given output stream.
     * @param[in] os: The output stream.
     */
    void serialize(std::ostream& os) const
    {
        mCenter.serialize(os);
        mEye.serialize(os);
        mUp.serialize(os);
        vcl::serialize(
            os,
            mFovDeg,
            mProjectionMode,
            mVerticalHeight,
            mAspect,
            mNear,
            mFar);
    }

    /**
     * @brief Deserializes the camera from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is)
    {
        mCenter.deserialize(is);
        mEye.deserialize(is);
        mUp.deserialize(is);
        vcl::deserialize(
            is,
            mFovDeg,
            mProjectionMode,
            mVerticalHeight,
            mAspect,
            mNear,
            mFar);
    }
};

/* Specialization Aliases */
using Cameraf = Camera<float>;

/* Concepts */

/**
 * @brief A concept representing a generic Camera.
 *
 * The concept is satisfied when `T` is a class that implements the methods
 * `viewMatrix()` and `projectionMatrix()`, returning 4x4 matrices.
 *
 * @tparam T: The type to be tested for conformity to the GenericCameraConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept GenericCameraConcept = requires (T&& c) {
    { c.viewMatrix() } -> Matrix44Concept;
    { c.projectionMatrix() } -> Matrix44Concept;
};

/**
 * @brief A concept representing a Camera.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Camera class having any scalar type.
 *
 * @tparam T: The type to be tested for conformity to the CameraConcept.
 *
 * @ingroup space_core
 */
template<typename T>
concept CameraConcept = std::derived_from< // same type or derived type
    std::remove_cvref_t<T>,
    Camera<typename RemoveRef<T>::ScalarType>>;

} // namespace vcl

#endif // VCL_SPACE_CORE_CAMERA_H
