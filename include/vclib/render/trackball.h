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

#ifndef VCL_RENDER_TRACKBALL_H
#define VCL_RENDER_TRACKBALL_H

#include <variant>

#include <vclib/space/quaternion.h>

#include "camera.h"
#include "lights/directional_light.h"

namespace vcl {

/**
 * @brief The TrackBall class implements a trackball camera.
 *
 * The trackball class stores a camera and provides a set motions that allow the
 * user to manipulate it.
 *
 * There are two main types of motions:
 * - atomic motions: these motions have no state and they are applied atomicly
 *   to the camera. They are not affected by the mouse position. An example of
 *   an atomic motion is a zoom.
 * - drag motions: these motions have a state, that may change with the mouse
 *   position. They begin and end with the `begin`/`end` member functions, and
 *   they update their state trough the change of the mouse position. An
 *   example of a drag motion is an arcball rotation.
 */
template<typename Scalar>
class TrackBall
{
public:
    enum MotionType { ARC, PAN, ROLL, ZOOM, DIR_LIGHT_ARC, MOTION_NUMBER };

    enum ViewAxis { HORIZONTAL, VERTICAL, AXIAL };

    struct TransformArgs
    {
        ViewAxis axis;
        Scalar   scalar; // could be an angle or a distance

        TransformArgs(ViewAxis a, Scalar s) : axis(a), scalar(s) {}
    };

    using AtomicMotionArg = std::variant<bool, TransformArgs, std::monostate>;

private:
    static constexpr Scalar RADIUS_RATIO = 1.75;

    enum Quadrant { UPPER_RIGHT, UPPER_LEFT, LOWER_LEFT, LOWER_RIGHT };

    vcl::DirectionalLight<Scalar> mDirLight;

    Camera<Scalar> mCamera;

    Point2<Scalar> mCurrMousePosition;
    Point2<Scalar> mPrevMousePosition;

    Scalar mHeight = 1; // heigth
    Scalar mWidth = 1; // width

    bool       mDragging       = false;
    MotionType mCurrDragMotion = MOTION_NUMBER;

    // arc motion state
    Point3<Scalar>     mStartVector;
    Point3<Scalar>     mStopVector;
    Quaternion<Scalar> mArcRotationSum;

    Scalar mPanScale = 0.005;

    Scalar mRollScale = 0.005;

    Scalar mZoomScale = 0.05;

    Scalar mEyeCenterDist = mCamera.eye().dist(mCamera.center());

    inline static const Point3<Scalar> X = Point3<Scalar>(1, 0, 0);
    inline static const Point3<Scalar> Y = Point3<Scalar>(0, 1, 0);
    inline static const Point3<Scalar> Z = Point3<Scalar>(0, 0, 1);

public:
    TrackBall() = default;

    const Camera<Scalar>& camera() const { return mCamera; }

    const DirectionalLight<Scalar>& light() const { return mDirLight; }

    Matrix44<Scalar> viewMatrix() const { return mCamera.viewMatrix(); }

    const Point3<Scalar>& center() const { return mCamera.center(); }

    const Scalar radius() const { return mEyeCenterDist / RADIUS_RATIO; }

    void reset(const Point3<Scalar>& center, Scalar radius = 1.0)
    {
        mCamera.reset();
        mCamera.center()      = center;
        mCamera.aspectRatio() = mWidth / mHeight;

        mArcRotationSum = Quaternion<Scalar>();
        mEyeCenterDist  = radius * RADIUS_RATIO;

        mDragging = false;

        mCurrDragMotion = MOTION_NUMBER;

        mCurrMousePosition = Point2<Scalar>();
        mPrevMousePosition = Point2<Scalar>();

        mStartVector = Point3<Scalar>();
        mStopVector  = Point3<Scalar>();

        updateCameraEye();
    }

    void resetDirectionalLight() { mDirLight.reset(); }

    // Settings member functions

    void setScreenSize(Scalar width, Scalar height)
    {
        if (width > 1 || height > 1) {
            mWidth                 = width;
            mHeight                 = height;
            mCamera.aspectRatio() = mWidth / mHeight;
        }
    }

    // Atomic motions

    /**
     * @brief Applies an atomic motion to the trackball.
     *
     * Atomic motions are motions that are applied atomically to the camera.
     * The step parameter is a std::variant that can store different values
     * depending on the motion type.
     *
     * Possible motion types and supported step values are:
     * - ROLL:
     *   - bool (true if roll clockwise, false if roll counter-clockwise)
     * - ZOOM:
     *   - bool (true if zoom out, false if zoom in)
     * - ARC:
     *   - TransformArgs (axis, angle) rotation along axis by angle
     * - PAN:
     *   - TransformArgs (axis, distance) translation along axis by distance
     *
     * @param motion
     * @param step
     */
    void applyAtomicMotion(
        MotionType      motion,
        AtomicMotionArg step = std::monostate())
    {
        if (std::holds_alternative<bool>(step)) {
            Scalar inc = std::get<bool>(step) ? 1 : -1;

            switch (motion) {
            case ROLL: performRoll(inc); break;
            case ZOOM: performZoom(inc); break;
            default: break;
            }
        }
        else if (std::holds_alternative<TransformArgs>(step)) {
            const TransformArgs& args = std::get<TransformArgs>(step);
            switch (motion) {
            case ARC: rotate(args.axis, args.scalar); break;
            case PAN: translate(args.axis, args.scalar); break;
            case DIR_LIGHT_ARC: rotateDirLight(args.axis, args.scalar); break;
            default: break;
            }
        }
    }

    void applyZoom(bool up) { applyAtomicMotion(ZOOM, up); }

    void applyRoll(bool clockwise) { applyAtomicMotion(ROLL, clockwise); }

    void applyPan(ViewAxis axis, Scalar distance)
    {
        applyAtomicMotion(PAN, TransformArgs(axis, distance));
    }

    void applyArc(ViewAxis axis, Scalar angle)
    {
        applyAtomicMotion(ARC, TransformArgs(axis, angle));
    }

    // Drag motions

    void beginDragMotion(MotionType motion)
    {
        setDragMotionValue(motion, true);
    }

    void endDragMotion(MotionType motion) { setDragMotionValue(motion, false); }

    void beginArc() { setDragMotionValue(ARC, true); }

    void endArc() { setDragMotionValue(ARC, false); }

    void beginPan() { setDragMotionValue(PAN, true); }

    void endPan() { setDragMotionValue(PAN, false); }

    void beginRoll() { setDragMotionValue(ROLL, true); }

    void endRoll() { setDragMotionValue(ROLL, false); }

    void beginZoom() { setDragMotionValue(ZOOM, true); }

    void endZoom() { setDragMotionValue(ZOOM, false); }

    void beginDirectionalLightArc() { setDragMotionValue(DIR_LIGHT_ARC, true); }

    void endDirectionalLightArc() { setDragMotionValue(DIR_LIGHT_ARC, false); }

    void setMousePosition(Scalar x, Scalar y)
    {
        mPrevMousePosition     = mCurrMousePosition;
        mCurrMousePosition.x() = x;
        mCurrMousePosition.y() = mHeight - y;
    }

    void setMousePosition(const Point2<Scalar>& point)
    {
        setMousePosition(point.x(), point.y());
    }

    /**
     * @brief Updates the state of the trackball during a drag motion.
     *
     * @note: this member function must be called only when a drag motion:
     * - begins (e.g. when the mouse is pressed)
     * - is in progress (e.g. when the mouse is mDragging);
     * - ends (e.g. when the mouse is released).
     */
    void update()
    {
        if (mCurrMousePosition != mPrevMousePosition) {
            bool clicking = mCurrDragMotion != MOTION_NUMBER;

            // first update when a drag begins
            if (!mDragging && clicking) {
                resetState();
                mStartVector = pointOnSphere(mCurrMousePosition);
                mDragging    = true;
            }
            // update when a motion is in progress
            else if (mDragging && clicking) {
                drag(mCurrDragMotion);
            }
            // update when a motion ends
            else if (mDragging && !clicking) {
                mDragging = false;
                resetState();
            }
        }
    }

private:
    /**-------------- Generic Functions  --------------**/

    void updateCameraEye()
    {
        Point3<Scalar> orientation = mArcRotationSum * Z;
        mCamera.eye() = orientation * mEyeCenterDist + mCamera.center();
    }

    void updateCameraUp() { mCamera.up() = (mArcRotationSum * Y).normalized(); }

    void resetState()
    {
        mArcRotationSum = Quaternion<Scalar>(mCamera.viewMatrix()).inverse();
        mEyeCenterDist  = mCamera.eye().dist(mCamera.center());
    }

    void setDragMotionValue(MotionType motion, bool value)
    {
        mCurrDragMotion = value ? motion : MOTION_NUMBER;
    }

    void drag(MotionType motion)
    {
        switch (motion) {
        case ARC: dragArc(); break;
        case PAN: dragPan(); break;
        case ROLL: dragRoll(); break;
        case ZOOM: dragZoom(); break;
        case DIR_LIGHT_ARC: dragDirLightArc(); break;
        default: break;
        }

        mPrevMousePosition = mCurrMousePosition;
    }

    /**-------------- Arc --------------**/

    void performArc(const Quaternion<Scalar>& rotation)
    {
        mArcRotationSum *= rotation;
        updateCameraEye();
        updateCameraUp();
    }

    // atomic

    void rotate(ViewAxis rot, Scalar angle = M_PI / 6)
    {
        switch (rot) {
        case HORIZONTAL: performArc(Quaternion<Scalar>(-angle, Y)); break;
        case VERTICAL: performArc(Quaternion<Scalar>(angle, X)); break;
        case AXIAL: performArc(Quaternion<Scalar>(angle, Z)); break;
        }
    }

    // drag

    vcl::Point3<Scalar> pointOnSphere(const vcl::Point2<Scalar>& point) const
    {
        vcl::Point3<Scalar> result;

        Scalar x = (2.f * point.x() - mWidth) / mWidth;
        Scalar y = (2.f * point.y() - mHeight) / mHeight;

        Scalar length2 = x * x + y * y;

        if (length2 <= .5) {
            result.z() = std::sqrt(1.0 - length2);
        }
        else {
            result.z() = 0.5 / std::sqrt(length2);
        }

        Scalar norm = 1.0 / std::sqrt(length2 + result.z() * result.z());

        result.x() = x * norm;
        result.y() = y * norm;
        result.z() *= norm;

        return result;
    }

    void dragArc()
    {
        mStopVector = pointOnSphere(mCurrMousePosition);
        Quaternion<Scalar> rotation(mStartVector, mStopVector);
        performArc(rotation.conjugate());
        mStartVector = mStopVector;
    }

    /**-------------- Roll --------------**/

    void performRoll(Scalar delta)
    {
        Point3<Scalar> axis  = (mCamera.center() - mCamera.eye()).normalized();
        Scalar         angle = mRollScale * delta;

        Quaternion<Scalar> rotation(angle, axis);

        mCamera.up() = rotation * mCamera.up();
    }

    Quadrant quadrant(Scalar x, Scalar y) const
    {
        Scalar halfw = mWidth / 2.0;
        Scalar halfh = mHeight / 2.0;

        // image coordinates origin is upperleft.
        if (x < halfw) {
            if (y < halfh) {
                return UPPER_LEFT;
            }
            else {
                return LOWER_LEFT;
            }
        }
        else {
            if (y < halfh) {
                return UPPER_RIGHT;
            }
            else {
                return LOWER_RIGHT;
            }
        }
    }

    void dragRoll()
    {
        auto     deltaP = mPrevMousePosition - mCurrMousePosition;
        Quadrant quad =
            quadrant(mCurrMousePosition.x(), mCurrMousePosition.y());
        switch (quad) {
        case UPPER_RIGHT: deltaP *= -1; break;
        case UPPER_LEFT: deltaP.x() *= -1; break;
        case LOWER_RIGHT: deltaP.y() *= -1; break;
        default: break;
        }

        Scalar delta = deltaP.x() + deltaP.y();

        performRoll(delta);
    }

    /**-------------- Pan --------------**/

    void performPan(const Point2<Scalar>& diff)
    {
        Scalar         l = mCamera.eye().dist(mCamera.center());
        Point3<Scalar> r = (mArcRotationSum * X).normalized();

        Point3<Scalar> pan =
            -(mCamera.up() * diff.y() + r * diff.x()) * l * mPanScale;
        mCamera.center() += pan;
        mCamera.eye() += pan;
    }

    // atomic

    void translate(ViewAxis axis, Scalar dist)
    {
        switch (axis) {
        case HORIZONTAL: {
            Point2<Scalar> diff(dist, 0);
            performPan(diff);
        } break;
        case VERTICAL: {
            Point2<Scalar> diff(0, dist);
            performPan(diff);
        } break;
        case AXIAL: performZoom(dist); break;
        }
    }

    // drag

    void dragPan()
    {
        Point2<Scalar> diff = mCurrMousePosition - mPrevMousePosition;
        performPan(diff);
    }

    /**-------------- Zoom --------------**/

    void performZoom(Scalar inc)
    {
        mEyeCenterDist += (mZoomScale * radius()) * inc;
        updateCameraEye();
    }

    void dragZoom()
    {
        Point2<Scalar> diff = mCurrMousePosition - mPrevMousePosition;

        Scalar ax = std::abs(diff.x());
        Scalar ay = std::abs(diff.y());

        bool up = true;

        if (ax > ay) {
            up = diff.x() < 0;
        }
        else if (ay > ax) {
            up = diff.y() < 0;
        }

        Scalar dist = mCurrMousePosition.dist(mPrevMousePosition);
        Scalar inc  = up ? dist : -dist;

        performZoom(inc);
    }

    /**-------------- Directional Light Arc --------------**/

    void performDirLightArc(const Quaternion<Scalar>& rotation)
    {
        mDirLight.direction() = rotation.conjugate() * mDirLight.direction();
        mDirLight.direction().normalize();
    }

    // atomic

    void rotateDirLight(ViewAxis rot, Scalar angle = M_PI / 6)
    {
        switch (rot) {
        case HORIZONTAL:
            performDirLightArc(Quaternion<Scalar>(-angle, Y));
            break;
        case VERTICAL: performDirLightArc(Quaternion<Scalar>(angle, X)); break;
        case AXIAL: performDirLightArc(Quaternion<Scalar>(angle, Z)); break;
        }
    }

    // drag

    void dragDirLightArc()
    {
        mStopVector = pointOnSphere(mCurrMousePosition);
        Quaternion<Scalar> rotation(mStartVector, mStopVector);
        performDirLightArc(rotation.conjugate());
        mStartVector = mStopVector;
    }
};

} // namespace vcl

#endif // VCL_RENDER_TRACKBALL_H
