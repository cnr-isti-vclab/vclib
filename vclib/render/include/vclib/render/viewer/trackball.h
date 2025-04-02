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

#ifndef VCL_RENDER_VIEWER_TRACKBALL_H
#define VCL_RENDER_VIEWER_TRACKBALL_H

#include "camera.h"
#include "lights/directional_light.h"

#include <vclib/space/core/quaternion.h>

#include <variant>

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
    enum MotionType {
        ARC,
        PAN,
        ZMOVE,
        ROLL,
        SCALE,
        FOV,
        FOCUS,
        DIR_LIGHT_ARC,
        MOTION_NUMBER
    };

    struct TransformArgs
    {
        Point3<Scalar> axis;
        Scalar         scalar; // could be an angle or a distance

        TransformArgs(const Point3<Scalar>& a, Scalar s) : axis(a), scalar(s) {}
    };

    using AtomicMotionArg =
        std::variant<TransformArgs, Scalar, Point3<Scalar>, std::monostate>;

private:
    // arcball radius ratio wrt minimum screen dimension
    // set as the inverse of the golden ratio
    static constexpr Scalar ARC_BALL_RADIUS_RATIO = 1.0 / 1.61803398875;

    static constexpr Scalar FOCUS_SCALE_FACTOR = 1.15;

    static constexpr Scalar DEFAULT_FOV_DEG = 54.0;

    Camera<Scalar> mCamera;

    // Similarity holding the manipulator transformation.
    // The convention is that the transformation is expressed as
    // TRANSLATION * ROTATION * SCALE
    // to avoid error accumulation we should split the transformation
    // into the three components (Point3, Quaternion, Scalar)
    Affine3<Scalar> mTransform = Affine3<Scalar>::Identity();

    Quaternion<Scalar> mDirectionalLightTransform;

    Point2<Scalar> mScreenSize = {-1, -1};

    // trackball radius in camera space
    // this value affects the interaction and the visualization of the trackball
    Scalar mRadius = ARC_BALL_RADIUS_RATIO;

    // trackball interaction state
    bool       mDragging       = false;
    MotionType mCurrDragMotion = MOTION_NUMBER;

    // initial arcball hit point
    Point3<Scalar> mInitialPoint;
    // initial transformation
    Affine3<Scalar> mInitialTransform = Affine3<Scalar>::Identity();
    // initial light rotation
    Quaternion<Scalar> mInitialDirRotation = Quaternion<Scalar>::Identity();

    Point2<Scalar> mCurrMousePosition;
    Point2<Scalar> mPrevMousePosition;

public:
    TrackBall() { mCamera.setFieldOfViewAdaptingEyeDistance(DEFAULT_FOV_DEG); }

    void reset()
    {
        auto screenSize        = mScreenSize;
        auto currMousePosition = mCurrMousePosition;
        auto prevMousePosition = mPrevMousePosition;
        *this                  = TrackBall();
        setScreenSize(screenSize);
        mCurrMousePosition = currMousePosition;
        mPrevMousePosition = prevMousePosition;
    }

    /**
     * @brief Reset the manipulator to a given center and scale.
     * @param center
     * @param scale
     */
    void reset(const Point3<Scalar>& center, Scalar scale)
    {
        reset();
        mTransform.scale(scale);
        mTransform.translate(-center);
    }

    void resetDirectionalLight()
    {
        mDirectionalLightTransform = Quaternion<Scalar>();
    }

    Point3<Scalar> center() const
    {
        // obtain the point in world space that maps to zero when transformed
        return mTransform.inverse().translation();
    }

    void setCenter(const Point3<Scalar>& center)
    {
        // transform the center (world space) using the current transformation
        // then translate it to the origin
        mTransform.pretranslate(-(mTransform * center));
    }

    Scalar scale() const
    {
        // return the average of the norm of linear part vectors
        return mTransform.linear().colwise().norm().mean();
    }

    void setScale(Scalar scale)
    {
        // scale the linear part of the transformation
        // so the manipulator will be scaled around the origin
        mTransform.prescale(scale);
        // TODO: scale also near/far?
    }

    void changeScale(Scalar factor) { mTransform.prescale(factor); }

    void setRotation(const Quaternion<Scalar>& rotation)
    {
        Affine3<Scalar> tx = Affine3<Scalar>::Identity();
        tx.rotate(rotation).scale(scale());
        mTransform.linear() = tx.linear();
    }

    void setRotation(const Point3<Scalar>& axis, Scalar angle)
    {
        setRotation(Quaternion<Scalar>(angle, axis));
    }

    /**
     * @brief Get the vertical field of view.
     */
    Scalar fovDeg() const { return mCamera.fieldOfView(); }

    /**
     * @brief Set the vertical field of view adapting the eye distance.
     * @param[in] fov: the field of view in degrees.
     */
    void setFovDeg(Scalar fov)
    {
        mCamera.setFieldOfViewAdaptingEyeDistance(fov);
    }

    Camera<Scalar>::ProjectionMode projectionMode() const
    {
        return mCamera.projectionMode();
    }

    void setProjectionMode(Camera<Scalar>::ProjectionMode mode)
    {
        mCamera.projectionMode() = mode;
        mCamera.setFieldOfViewAdaptingEyeDistance(mCamera.fieldOfView());
    }

    void setScreenSize(Scalar width, Scalar height)
    {
        if (width > 1 || height > 1) {
            mScreenSize.x()       = width;
            mScreenSize.y()       = height;
            mCamera.aspectRatio() = width / height;
            mRadius = ARC_BALL_RADIUS_RATIO * mCamera.verticalHeight() / 2.0;
            if (width < height)
                mRadius *= mCamera.aspectRatio();
        }
    }

    void setScreenSize(const Point2<Scalar>& sz)
    {
        setScreenSize(sz.x(), sz.y());
    }

    DirectionalLight<Scalar> light() const
    {
        // TODO: return a light direction stored in this class,
        // in order to store also the light color
        return DirectionalLight<Scalar>(
            mDirectionalLightTransform * Point3<Scalar>(0, 0, 1));
    }

    void setLightDirection(const Point3<Scalar>& direction)
    {
        mDirectionalLightTransform = Quaternion<Scalar>::FromTwoVectors(
            Point3<Scalar>(0, 0, 1), direction);
    }

    const vcl::Camera<Scalar>& camera() const { return mCamera; }

    Matrix44<Scalar> viewMatrix() const
    {
        return mCamera.viewMatrix() * mTransform.matrix();
    }

    Matrix44<Scalar> projectionMatrix() const
    {
        return mCamera.projectionMatrix();
    }

    Matrix44<Scalar> gizmoMatrix() const
    {
        Affine3<Scalar> rot_radius = Affine3<Scalar>::Identity();
        rot_radius.rotate(mTransform.rotation()).scale(mRadius);
        return mCamera.viewMatrix() * rot_radius.matrix();
    }

    Matrix44<Scalar> lightGizmoMatrix() const
    {
        Affine3<Scalar> rot_radius = Affine3<Scalar>::Identity();
        rot_radius.rotate(mDirectionalLightTransform).scale(mRadius);
        return mCamera.viewMatrix() * rot_radius.matrix();
    }

    bool isDragging() const { return mDragging; }

    MotionType currentMotion() const { return mCurrDragMotion; }

    // Atomic motions

    /**
     * @brief Applies an atomic motion to the trackball.
     * Atomic motions are motions that are applied atomically to the camera.
     * The step parameter is a std::variant that can store different values
     * depending on the motion type.
     *
     * Possible motion types and supported step values are:
     * - ROLL:
     *   - Scalar (positive counter-clockwise roll, negative otherwise)
     * - SCALE:
     *   - Scalar (positive zoom in, negative zoom out)
     * - ARC:
     *   - TransformArgs (axis, angle) rotation along axis by angle
     * - PAN:
     *   - Point3 translation in trackball camera space
     * - ZMOVE:
     *   - Scalar z translation in trackball camera space
     * - FOCUS:
     *   - Point3 set center in world space
     *@param motion
     * @param step
     */
    void applyAtomicMotion(
        MotionType      motion,
        AtomicMotionArg step = std::monostate())
    {
        if (std::holds_alternative<Scalar>(step)) {
            Scalar inc = std::get<Scalar>(step);

            switch (motion) {
            case ROLL: roll(inc); break;
            case SCALE: performScale(inc); break;
            case FOV: performFov(inc); break;
            case ZMOVE: performZmove(inc); break;
            default: break;
            }
        }
        else if (std::holds_alternative<TransformArgs>(step)) {
            const TransformArgs& args = std::get<TransformArgs>(step);
            switch (motion) {
            case ARC: rotate(args.axis, args.scalar); break;
            case DIR_LIGHT_ARC: rotateDirLight(args.axis, args.scalar); break;
            default: break;
            }
        }
        else if (std::holds_alternative<Point3<Scalar>>(step)) {
            Point3<Scalar> val = std::get<Point3<Scalar>>(step);
            switch (motion) {
            case PAN: translate(val); break;
            case FOCUS: setCenter(val); changeScale(FOCUS_SCALE_FACTOR);
            default: break;
            }
        }
    }

    void applyScale(Scalar value) { applyAtomicMotion(SCALE, value); }

    void applyRoll(Scalar angleRad) { applyAtomicMotion(ROLL, angleRad); }

    void applyPan(const Point3<Scalar>& translation)
    {
        applyAtomicMotion(PAN, translation);
    }

    void applyArc(const Point3<Scalar>& axis, Scalar angle)
    {
        applyAtomicMotion(ARC, TransformArgs(axis, angle));
    }

    // Drag motions

    void setMousePosition(Scalar x, Scalar y)
    {
        mPrevMousePosition     = mCurrMousePosition;
        mCurrMousePosition.x() = x;
        mCurrMousePosition.y() = mScreenSize.y() - y;
    }

    void setMousePosition(const Point2<Scalar>& point)
    {
        setMousePosition(point.x(), point.y());
    }

    /**
     * @brief Starts a drag motion.
     * @param[in] motion: the motion type
     * @note this function should be called when the drag motion begins
     * (e.g., the mouse is pressed) or the motion type changes.
     */
    void beginDragMotion(MotionType motion)
    {
        assert(motion != MOTION_NUMBER && "Invalid motion type");

        // no need to restart?
        if (mCurrDragMotion == motion)
            return;

        // end previous motion
        if (mCurrDragMotion != motion)
            endDragMotion(mCurrDragMotion);

        setDragMotionValue(motion, true);
        mInitialPoint       = pointOnArcball(mCurrMousePosition);
        mInitialTransform   = mTransform;
        mInitialDirRotation = mDirectionalLightTransform;
        mDragging           = true;
    }

    /**
     * @brief Ends a drag motion
     * @param[in] motion: the motion type
     * @note this function should be called when the drag motion ends
     * (e.g., the mouse is released)
     */
    void endDragMotion(MotionType motion)
    {
        setDragMotionValue(motion, false);
        mDragging = false;
    }

    /**
     * @brief Updates the state of the trackball during a drag motion.
     * @note: this member function must be called only when a drag motion
     * is in progress (e.g., when the mouse is dragging);
     */
    void update() // TODO: rename this function (it just updates the motion)
    {
        assert(
            mDragging != (mCurrDragMotion == MOTION_NUMBER) &&
            "Invalid state: dragging and no motion");
        if (mDragging && mCurrMousePosition != mPrevMousePosition)
            drag(mCurrDragMotion);
    }

private:
    /**-------------- Generic Functions  --------------**/

    void setDragMotionValue(MotionType motion, bool value)
    {
        mCurrDragMotion = value ? motion : MOTION_NUMBER;
    }

    void drag(MotionType motion)
    {
        switch (motion) {
        case ARC: dragArc(); break;
        case PAN: dragPan(); break;
        case ZMOVE: dragZmove(); break;
        case ROLL: dragRoll(); break;
        case SCALE: dragScale(); break;
        case DIR_LIGHT_ARC: dragDirLightArc(); break;
        default: break;
        }
    }

    Scalar trackballToPixelRatio() const
    {
        return mCamera.verticalHeight() / mScreenSize.y();
    }

    Point3<Scalar> pointOnTrackballPlane(Point2<Scalar> screenCoord) const
    {
        // convert to camera space
        // range [-1, 1] for Y and [-aspectRatio, aspectRatio] for X
        screenCoord =
            (screenCoord - mScreenSize / 2.0) * trackballToPixelRatio();
        return Point3<Scalar>(screenCoord.x(), screenCoord.y(), 0.0);
    }

    Point3<Scalar> pointOnArcball(Point2<Scalar> screenCoord) const
    {
        // convert to range [-1, 1] for Y and [-aspectRatio, aspectRatio] for X
        screenCoord =
            (screenCoord - mScreenSize / 2.0) * trackballToPixelRatio();

        // Solve the equations in 2D on the plane passing through the eye,
        // the trackball center, and the intersection line.
        // The X coordinate corresponds to the Z axis, while the Y coordinate
        // is on the XY trackball plane.

        const double   h = screenCoord.norm();
        Point2<Scalar> hitPoint;

        if (mCamera.projectionMode() == Camera<Scalar>::ProjectionMode::ORTHO) {
            // in ortho projection we can project the Y coordinate directly
            if (h < (M_SQRT1_2 * mRadius)) {
                // hit sphere
                // Y^2 + X^2 = r^2
                // X = sqrt(r^2 - Y^2)
                hitPoint =
                    Point2<Scalar>(std::sqrt(mRadius * mRadius - h * h), h);
            }
            else {
                // hit hyperbola
                // X*Y = (r^2 / 2)
                // X = r^2 / (2 * Y)
                hitPoint = Point2<Scalar>(mRadius * mRadius / (2 * h), h);
            }
        }
        else {
            assert(
                mCamera.projectionMode() ==
                    Camera<Scalar>::ProjectionMode::PERSPECTIVE &&
                "invalid camera projection value is supported");
            // PERSPECTIVE PROJECTION
            //
            //      |            ^
            //      |     h     /
            //--target------------> Y axis
            //      |         /
            //      |        /
            //      |       /
            //      |      /
            //      |     / intersecting line
            //    d |    /
            //      |   /
            //      |  /
            //      | /
            //      |/
            //     eye
            //     /|
            //    / |
            //   /  |
            //     \./ X axis

            // Equation constants:
            //     d = distance from the eye to the target
            //     h = distance(target, line plane inter
            const double d = (mCamera.eye() - mCamera.center()).norm();

            const double m = -h / d;

            bool           sphereHit = false;
            Point2<Scalar> spherePoint;
            {
                // hit point on the sphere
                //
                // line equation:
                // Y = -(h/d) * X + h
                //
                // circle equation:
                // X^2 + Y^2 = r^2
                //
                // substitute Y in the circle equation:
                // X^2 + (-h/d * X + h)^2 = r^2
                // X^2 + h^2/d^2 * X^2 - 2 * h^2/d * X + h^2 = r^2
                // (1 + h^2/d^2) * X^2 - 2 * h^2/d * X + h^2 - r^2 = 0

                // a = 1 + h^2/d^2
                // b = -2 * h^2/d
                // c = h^2 - r^2
                Scalar a = 1 + m * m;
                Scalar b = -2 * h * h / d;
                Scalar c = h * h - mRadius * mRadius;

                // X = (-b +- sqrt(b^2 - 4ac)) / 2a
                // Y = -(h/d) * X + h
                // we take the positive solution (hit point closest to the eye)

                Scalar delta = b * b - 4 * a * c;
                sphereHit    = delta >= 0;
                if (sphereHit) {
                    spherePoint.x() = (-b + std::sqrt(delta)) / (2 * a);
                    spherePoint.y() = m * spherePoint.x() + h;
                }
            }

            bool           hyperHit = false;
            Point2<Scalar> hyperPoint;
            {
                // hit point on the hyperbola
                //
                // line equation:
                // Y = -(h/d) * X + h
                //
                // hyperbola equation:
                // Y = 1/X * (r^2 / 2)

                // substitute Y in the hyperbola equation:
                // -(h/d) * X + h = 1/X * (r^2 / 2)
                // -(h/d) * X^2 + h * X = r^2 / 2
                // -h * X^2 + d * h * X = d * r^2 / 2
                // -2 * h * X^2 + 2 * d * h * X = d * r^2
                // -2 * h * X^2 + 2 * d * h * X - d * r^2 = 0

                // a = -2 * h
                // b = 2 * d * h
                // c = - d * r^2

                Scalar a = -2 * h;
                Scalar b = 2 * d * h;
                Scalar c = -d * mRadius * mRadius;

                // X = (-b +- sqrt(b^2 - 4ac)) / 2a
                // Y = -(h/d) * X + h
                // we take the solution with smallest x
                // (hit point farthest from the eye)

                Scalar delta = b * b - 4 * a * c;
                hyperHit     = delta >= 0;
                if (hyperHit) {
                    hyperPoint.x() = (-b + std::sqrt(delta)) / (2 * a);
                    hyperPoint.y() = m * hyperPoint.x() + h;
                }
            }

            // discriminate between sphere and hyperbola hit points
            const int hitCase = sphereHit + 2 * hyperHit;
            switch (hitCase) {
            case 0: // no hit
            {
                // get closeset point of the line to the origin
                // rotate the line vector by 90 degrees counter-clockwise
                Point2<Scalar> lineVector = Point2<Scalar>(d, -h).normalized();
                Point2<Scalar> lineNormal =
                    Point2<Scalar>(-lineVector.y(), lineVector.x());
                // project the eye on the vector
                hitPoint = lineNormal * lineNormal.dot(Point2<Scalar>(d, 0));
            } break;
            case 1: // sphere hit
                hitPoint = spherePoint;
                break;
            case 2: // hyperbola hit
                hitPoint = hyperPoint;
                break;
            case 3: // both hit
            {
                // check angle of sphere point
                Scalar angle = std::atan2(spherePoint.y(), spherePoint.x());
                // if angle is more than 45 degrees, take the hyperbola point
                if (angle > M_PI / 4) {
                    hitPoint = hyperPoint;
                }
                else {
                    hitPoint = spherePoint;
                }
            } break;
            default: assert(false && "Invalid hit case");
            }
        }

        // convert hit point to 3D
        // rescale in trackball space
        // FIXME: Avoid cancellation issue with different solution
        assert(hitPoint.x() == hitPoint.x());
        double factor =
            (h > 0.0 && hitPoint.y() > 0.0) ? hitPoint.y() / h : 0.0;
        return Point3<Scalar>(
            screenCoord.x() * factor, screenCoord.y() * factor, hitPoint.x());
    }

    /**--------- Base functions ---------**/
    // (general purpose and used for atomic operations)

    void rotate(const Quaternion<Scalar>& q) { mTransform.prerotate(q); }

    void rotate(Point3<Scalar> axis, Scalar angleRad)
    {
        mTransform.prerotate(Quaternion<Scalar>(angleRad, axis));
    }

    /**
     * @brief Translate in the camera space
     * @note from the trackball center
     * Y range is [-1, 1] and X is [-aspectRatio, aspectRatio]
     */
    void translate(Point3<Scalar> t) { mTransform.pretranslate(t); }

    void rotateDirLight(const Quaternion<Scalar>& rotation)
    {
        mDirectionalLightTransform = rotation * mDirectionalLightTransform;
    }

    void rotateDirLight(Point3<Scalar> axis, Scalar angle)
    {
        rotateDirLight(Quaternion<Scalar>(angle, axis));
    }

    /**--- User interaction functions ---**/

    /**-------------- Arc --------------**/

    void dragArc()
    {
        const Point3<Scalar>     point = pointOnArcball(mCurrMousePosition);
        Eigen::AngleAxis<Scalar> ax(
            Eigen::Quaternion<Scalar>::FromTwoVectors(mInitialPoint, point));
        // use angle proportional to the arc length
        const Scalar phi = (point - mInitialPoint).norm() / mRadius;
        ax.angle()       = phi;

        // rotate from freezed transformation
        // mTransform = mInitialTransform;
        // mTransform.prerotate(ax);

        // modify rotation only
        mTransform.prerotate(
            ax * mInitialTransform.rotation() *
            mTransform.rotation().inverse());
    }

    /**-------------- Roll --------------**/

    void roll(Scalar delta) { rotate(Point3<Scalar>(0, 0, 1), delta); }

    void dragRoll()
    {
        static constexpr Scalar ROLL_DIST_TO_CENTER_THRESHOLD = 0.025;

        const Point3<Scalar> prev = pointOnTrackballPlane(mPrevMousePosition);
        const Point3<Scalar> curr = pointOnTrackballPlane(mCurrMousePosition);
        if (prev.norm() < ROLL_DIST_TO_CENTER_THRESHOLD ||
            curr.norm() < ROLL_DIST_TO_CENTER_THRESHOLD)
            return;

        Scalar angle =
            std::atan2(curr.y(), curr.x()) - std::atan2(prev.y(), prev.x());

        roll(angle);
    }

    /**-------------- Pan --------------**/

    /**
     * @brief perform a pan operation
     * @param pixelDelta the pan movement in pixels
     */
    void performPan(const Point2<Scalar>& pixelDelta)
    {
        const Point2<Scalar> pan = pixelDelta * trackballToPixelRatio();
        translate(Point3<Scalar>(pan.x(), pan.y(), 0.0));
    }

    // drag
    void dragPan()
    {
        Point2<Scalar> pixelDelta = mCurrMousePosition - mPrevMousePosition;
        performPan(pixelDelta);
    }

    /**-------------- Z-Move --------------**/
    /**
     * @brief translate in the camera z direction
     * @param pixelDelta the delta movement in pixels
     */
    void performZmove(const Scalar& pixelDelta)
    {
        const Scalar translation = pixelDelta * trackballToPixelRatio();
        translate(Point3<Scalar>(0.0, 0.0, -translation));
    }

    // drag
    void dragZmove()
    {
        auto pixelDelta = mCurrMousePosition.y() - mPrevMousePosition.y();
        performZmove(pixelDelta);
    }

    /**-------------- Scaling --------------**/

    // scrolling and scaling are setup with "magic" numbers
    void performScale(Scalar pixelDelta)
    {
        pixelDelta /= 60;
        const auto factor = std::pow(1.2f, -pixelDelta);
        changeScale(factor);
    }

    void dragScale()
    {
        Scalar pixelDelta = mCurrMousePosition.y() - mPrevMousePosition.y();
        performScale(pixelDelta);
    }

    /**----- Directional Light Arc -----**/

    // drag
    void dragDirLightArc()
    {
        const Point3<Scalar>     point = pointOnArcball(mCurrMousePosition);
        Eigen::AngleAxis<Scalar> ax(
            Eigen::Quaternion<Scalar>::FromTwoVectors(mInitialPoint, point));
        // use angle proportional to the arc length
        const Scalar phi = (point - mInitialPoint).norm() / mRadius;

        mDirectionalLightTransform = mInitialDirRotation;
        rotateDirLight(Point3<Scalar>(ax.axis()), phi);
    }

    /**----- Fov -----**/

    void performFov(Scalar pixelDelta)
    {
        static constexpr double MIN_FOV_DEG = 5.0;
        static constexpr double MAX_FOV_DEG = 90.0;

        pixelDelta /= 60.0;
        double fov = fovDeg();

        // ortho -> perspective
        if (mCamera.projectionMode() == Camera<Scalar>::ProjectionMode::ORTHO) {
            if (pixelDelta > 0) {
                fov = MIN_FOV_DEG;
                mCamera.projectionMode() =
                    Camera<Scalar>::ProjectionMode::PERSPECTIVE;
            }
        }

        // update fov
        fov = std::clamp(fov + 1.2 * pixelDelta, MIN_FOV_DEG, MAX_FOV_DEG);

        // perspective -> ortho
        if (mCamera.projectionMode() ==
                Camera<Scalar>::ProjectionMode::PERSPECTIVE &&
            fov == MIN_FOV_DEG) {
            mCamera.projectionMode() = Camera<Scalar>::ProjectionMode::ORTHO;
        }

        // commit fov
        setFovDeg(fov);
    }
};

} // namespace vcl

#endif // VCL_RENDER_VIEWER_TRACKBALL_H
