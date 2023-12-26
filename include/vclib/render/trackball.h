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
    enum MotionType {
        ARC,
        PAN,
        ROLL,
        ZOOM,
        MOTION_NUMBER
    };

    enum ViewAxis {
        HORIZONTAL,
        VERTICAL,
        AXIAL
    };

    struct TransformArgs
    {
        ViewAxis axis;
        Scalar scalar; // could be an angle or a distance
        TransformArgs(ViewAxis a , Scalar s) : axis(a), scalar(s) {}
    };

    using AtomicMotionArg = std::variant<bool, TransformArgs, std::monostate>;

private:
    static constexpr Scalar RADIUS_RATIO = 1.75;

    enum Quadrant {
        UPPER_RIGHT,
        UPPER_LEFT,
        LOWER_LEFT,
        LOWER_RIGHT
    };

    vcl::DirectionalLight<Scalar> dl;

    Camera<Scalar> cam;

    Point2<Scalar> currMousePosition;
    Point2<Scalar> prevMousePosition;

    Scalar h = 1; // width
    Scalar w = 1; // heigth

    bool dragging = false;
    MotionType currDragMotion = MOTION_NUMBER;

    // arc motion state
    Point3<Scalar> startVector;
    Point3<Scalar> stopVector;
    Quaternion<Scalar> arcRotationSum;

    Scalar panScale = 0.005;

    Scalar rollScale = 0.005;

    Scalar zoomScale = 0.05;

    Scalar eyeCenterDist = cam.eye().dist(cam.center());

    inline static const Point3<Scalar> X = Point3<Scalar>(1, 0, 0);
    inline static const Point3<Scalar> Y = Point3<Scalar>(0, 1, 0);
    inline static const Point3<Scalar> Z = Point3<Scalar>(0, 0, 1);

public:

    TrackBall() = default;

    const Camera<Scalar>& camera() const { return cam; }

    const DirectionalLight<Scalar>& light() const { return dl; }

    const Matrix44<Scalar>& viewMatrix() const { return cam.viewMatrix(); }

    const Point3<Scalar>& center() const { return cam.center(); }

    const Scalar radius() const { return eyeCenterDist / RADIUS_RATIO; }

    void reset(Scalar radius = 1.0)
    {
        cam.reset();
        arcRotationSum = Quaternion<Scalar>();
        eyeCenterDist = radius * RADIUS_RATIO;

        dragging = false;

        currDragMotion = MOTION_NUMBER;

        currMousePosition = Point2<Scalar>();
        prevMousePosition = Point2<Scalar>();

        startVector = Point3<Scalar>();
        stopVector = Point3<Scalar>();

        updateCameraEye();
        cam.updateMatrix();
    }

    // Settings member functions

    void setScreenSize(Scalar width, Scalar height)
    {
        if (width > 1 || height > 1){
            w = width;
            h = height;
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
    void applyAtomicMotion(MotionType motion, AtomicMotionArg step = std::monostate())
    {
        if (std::holds_alternative<bool>(step)) {
            Scalar inc = std::get<bool>(step) ? 1 : -1;

            switch (motion) {
            case ROLL:
                performRoll(inc);
                break;
            case ZOOM:
                performZoom(inc);
                break;
            default: break;
            }
        } else if (std::holds_alternative<TransformArgs>(step)) {
            const TransformArgs& args = std::get<TransformArgs>(step);
            switch(motion) {
            case ARC:
                rotate(args.axis, args.scalar);
                break;
            case PAN:
                translate(args.axis, args.scalar);
                break;
            default: break;
            }
        }
    }

    void applyZoom(bool up)
    {
        applyAtomicMotion(ZOOM, up);
    }

    void applyRoll(bool clockwise)
    {
        applyAtomicMotion(ROLL, clockwise);
    }

    void applyPan(ViewAxis axis, Scalar distance)
    {
        applyAtomicMotion(PAN, TransformArgs(axis, distance));
    }

    void applyArc(ViewAxis axis, Scalar angle)
    {
        applyAtomicMotion(ARC, TransformArgs(axis, angle));
    }

    // Drag motions

    void beginDragMotion(MotionType motion) { setDragMotionValue(motion, true); }

    void endDragMotion(MotionType motion) { setDragMotionValue(motion, false); }

    void beginArc() { setDragMotionValue(ARC, true); }

    void endArc() { setDragMotionValue(ARC, false); }

    void beginPan() { setDragMotionValue(PAN, true); }

    void endPan() { setDragMotionValue(PAN, false); }

    void beginRoll() { setDragMotionValue(ROLL, true); }

    void endRoll() { setDragMotionValue(ROLL, false); }

    void beginZoom() { setDragMotionValue(ZOOM, true); }

    void endZoom() { setDragMotionValue(ZOOM, false); }

    void setMousePosition(Scalar x, Scalar y)
    {
        prevMousePosition = currMousePosition;
        currMousePosition.x() = x;
        currMousePosition.y() = h - y;
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
     * - is in progress (e.g. when the mouse is dragging);
     * - ends (e.g. when the mouse is released).
     */
    void update()
    {
        if (currMousePosition != prevMousePosition) {
            bool clicking = currDragMotion != MOTION_NUMBER;

            // first update when a drag begins
            if (!dragging && clicking) {
                resetState();
                startVector = pointOnSphere(currMousePosition);
                dragging = true;
            }
            // update when a motion is in progress
            else if (dragging && clicking) {
                drag(currDragMotion);
            }
            // update when a motion ends
            else if (dragging && !clicking) {
                dragging = false;
                resetState();
            }
        }
    }

private:
    /**-------------- Generic Functions  --------------**/

    void updateCameraEye()
    {
        Point3<Scalar> orientation = arcRotationSum * Z;
        cam.eye() = orientation * eyeCenterDist + cam.center();
    }

    void updateCameraUp()
    {
        cam.up() = (arcRotationSum * Y).normalized();
    }

    void resetState()
    {
        arcRotationSum = Quaternion<Scalar>(cam.matrix()).inverse();
        eyeCenterDist = cam.eye().dist(cam.center());
    }

    void setDragMotionValue(MotionType motion, bool value)
    {
        currDragMotion = value ? motion : MOTION_NUMBER;
    }

    void drag(MotionType motion)
    {

        switch(motion) {
        case ARC:
            dragArc();
            break;
        case PAN:
            dragPan();
            break;
        case ROLL:
            dragRoll();
            break;
        case ZOOM:
            dragZoom();
            break;
        default:
            break;
        }

        prevMousePosition = currMousePosition;
    }

    /**-------------- Arc --------------**/

    void performArc(const Quaternion<Scalar>& rotation)
    {
        arcRotationSum *= rotation;
        updateCameraEye();
        updateCameraUp();
        cam.updateMatrix();
    }

    // atomic

    void rotate(ViewAxis rot, Scalar angle = M_PI/6)
    {
        switch (rot)  {
        case HORIZONTAL:
            performArc(Quaternion<Scalar>(-angle, Y));
            break;
        case VERTICAL:
            performArc(Quaternion<Scalar>(angle, X));
            break;
        case AXIAL:
            performArc(Quaternion<Scalar>(angle, Z));
            break;
        }
    }

    // drag

    vcl::Point3<Scalar> pointOnSphere(const vcl::Point2<Scalar>& point) const
    {
        vcl::Point3<Scalar> result;

        Scalar x = (2.f * point.x() - w) / w;
        Scalar y = (2.f * point.y() - h) / h;

        Scalar length2 = x*x + y*y;

        if (length2 <= .5) {
            result.z() = std::sqrt(1.0 - length2);
        } else {
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
        stopVector = pointOnSphere(currMousePosition);
        Quaternion<Scalar> rotation(startVector, stopVector);
        performArc(rotation.conjugate());
        startVector = stopVector;
    }

    /**-------------- Roll --------------**/

    void performRoll(Scalar delta)
    {
        Point3<Scalar> axis = (cam.center() - cam.eye()).normalized();
        Scalar angle = rollScale * delta;

        Quaternion<Scalar> rotation(angle, axis);

        cam.up() = rotation * cam.up();
        cam.updateMatrix();
    }

    Quadrant quadrant(Scalar x, Scalar y) const
    {
        Scalar halfw = w / 2.0;
        Scalar halfh = h / 2.0;

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
        auto deltaP = prevMousePosition - currMousePosition;
        Quadrant quad = quadrant(currMousePosition.x(), currMousePosition.y());
        switch (quad) {
        case UPPER_RIGHT:
            deltaP *= -1;
            break;
        case UPPER_LEFT:
            deltaP.x() *= -1;
            break;
        case LOWER_RIGHT:
            deltaP.y() *= -1;
            break;
        default:
            break;
        }

        Scalar delta = deltaP.x() + deltaP.y();

        performRoll(delta);
    }

    /**-------------- Pan --------------**/

    void performPan(const Point2<Scalar>& diff)
    {
        Scalar l = cam.eye().dist(cam.center());
        Point3<Scalar> r = (arcRotationSum * X).normalized();

        Point3<Scalar> pan = -(cam.up() * diff.y() + r * diff.x()) * l * panScale;
        cam.center() += pan;
        cam.eye() += pan;
        cam.updateMatrix();
    }

    // atomic

    void translate(ViewAxis axis, Scalar dist)
    {
        switch (axis)  {
        case HORIZONTAL: {
            Point2<Scalar> diff(dist, 0);
            performPan(diff);
        }
            break;
        case VERTICAL: {
            Point2<Scalar> diff(0, dist);
            performPan(diff);
        }
            break;
        case AXIAL:
            performZoom(dist);
            break;
        }
    }

    // drag

    void dragPan()
    {
        Point2<Scalar> diff = currMousePosition - prevMousePosition;
        performPan(diff);
    }

    /**-------------- Zoom --------------**/

    void performZoom(Scalar inc)
    {
        eyeCenterDist += zoomScale * inc;
        updateCameraEye();
        cam.updateMatrix();
    }

    void dragZoom()
    {
        Point2<Scalar> diff = currMousePosition - prevMousePosition;

        Scalar ax = std::abs(diff.x());
        Scalar ay = std::abs(diff.y());

        bool up = true;

        if (ax > ay) {
            up = diff.x() < 0;
        }
        else if (ay > ax){
            up = diff.y() < 0;
        }

        Scalar dist = currMousePosition.dist(prevMousePosition);
        Scalar inc = up ? dist : -dist;

        performZoom(inc);
    }
};

} // namespace vcl

#endif // VCL_RENDER_TRACKBALL_H
