// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_DRAWERS_TRACKBALL_EVENT_DRAWER_H
#define VCL_RENDER_DRAWERS_TRACKBALL_EVENT_DRAWER_H

#include "event_drawer.h"

#include <vclib/render/gizmos/directional_light_gizmo.h>
#include <vclib/render/gizmos/trackball_gizmo.h>
#include <vclib/render/input.h>
#include <vclib/render/settings/draw_object_settings.h>
#include <vclib/render/settings/trackball_settings.h>
#include <vclib/render/viewer/trackball.h>

#include <vclib/space/core.h>

#include <functional>
#include <map>
#include <string>

namespace vcl {

template<typename Scalar, typename DerivedRenderApp>
class TrackBallEventDrawerT : public EventDrawer<DerivedRenderApp>
{
public:
    using ScalarType    = Scalar;
    using PointType     = Point3<Scalar>;
    using MatrixType    = Matrix44<Scalar>;
    using TrackBallType = vcl::TrackBall<Scalar>;

    inline static const Point3<Scalar> UNIT_X = {1, 0, 0};
    inline static const Point3<Scalar> UNIT_Y = {0, 1, 0};

private:
    using Base = EventDrawer<DerivedRenderApp>;

    using DragMotionMap        = TrackballSettings::DragMotionMap;
    using ScrollAtomicMap      = TrackballSettings::ScrollAtomicMap;
    using KeyAtomicMap         = TrackballSettings::KeyAtomicMap;
    using AtomicActionCallback = std::function<void(TrackBallEventDrawerT& d)>;

    // translation step in camera space
    static constexpr double DISCRETE_TRANSLATION_STEP = 0.1;
    static constexpr double DISCRETE_ROTATION_STEP    = M_PI_4 / 3.0; // 15 deg

    uint mWidth  = 1024;
    uint mHeight = 768;

    vcl::TrackBall<Scalar> mTrackball;

    Point3<Scalar> mDefaultTrackBallCenter;
    float          mDefaultTrackBallRadius = 1.0;

    // current modifiers state (must be updated using setKeyModifiers)
    KeyModifiers mCurrentKeyModifiers = {KeyModifier::NO_MODIFIER};
    // current mouse button (automatically updated)
    // if dragging holds the current mouse button
    MouseButton::Enum mCurrentMouseButton = MouseButton::NO_BUTTON;

    // trackball gizmo
    TrackballGizmo mTrackballGizmo;

    // directional light gizmo
    DirectionalLightGizmo mDirectionalLightGizmo;

    std::function<void(void)> mCustomShortcutToggleTrackballCallback =
        [this]() {
            toggleTrackBallVisibility();
        };

    TrackballSettings mDefaultTrackballSettings;

    std::map<std::string, AtomicActionCallback> mAtomicActionRegistry =
        defaultAtomicActionRegistry();

public:
    TrackBallEventDrawerT(uint width = 1024, uint height = 768) :
            Base(width, height)
    {
        resizeViewer(width, height);
    }

    virtual TrackballSettings& trackballSettings()
    {
        return mDefaultTrackballSettings;
    }

    virtual const TrackballSettings& trackballSettings() const
    {
        return mDefaultTrackballSettings;
    }

    Matrix44<Scalar> viewMatrix() const { return mTrackball.viewMatrix(); }

    Matrix44<Scalar> projectionMatrix() const
    {
        return mTrackball.projectionMatrix();
    }

    void reset()
    {
        mTrackball.reset(
            mDefaultTrackBallCenter, 1.5 / mDefaultTrackBallRadius);
    }

    void focus(const Point3<Scalar>& center)
    {
        mTrackball.applyAtomicMotion(TrackballMotionType::FOCUS, center);
    }

    void fitScene(const Point3<Scalar>& center, Scalar radius)
    {
        mDefaultTrackBallCenter = center;
        mDefaultTrackBallRadius = radius;

        reset();
    }

    void fitView(const Point3<Scalar>& center)
    {
        mTrackball.adaptCurrentViewToCenter(center);
    }

    Camera<Scalar> camera() const { return mTrackball.camera(); }

    void setCamera(const Camera<Scalar>& cam) { mTrackball.setCamera(cam); }

    DirectionalLight<Scalar> light() const { return mTrackball.light(); }

    Matrix44<Scalar> lightGizmoMatrix() const
    {
        return mTrackball.lightGizmoMatrix();
    }

    Matrix44<Scalar> gizmoMatrix() const { return mTrackball.gizmoMatrix(); }

    /**
     * @brief Changes the current zoom (scale) of the trackball.
     * @param[in] factor: Positive value to zoom in, negative to zoom out.
     */
    void trackballZoom(ScalarType factor)
    {
        mTrackball.applyScale(factor);
        requestUpdate();
    }

    /**
     * @brief Pans the current view in the camera coordinate system.
     * @param[in] translation: 3D translation vector.
     */
    void trackballPan(const PointType& translation)
    {
        mTrackball.applyPan(translation);
        requestUpdate();
    }

    /**
     * @brief Rotates the trackball around an arbitrary axis.
     * @param[in] axis: Rotation axis.
     * @param[in] angleRad: Rotation angle in radians.
     */
    void trackballRotate(const PointType& axis, ScalarType angleRad)
    {
        mTrackball.applyArc(axis, angleRad);
        requestUpdate();
    }

    /**
     * @brief Rolls the trackball around the camera view axis.
     * @param[in] angleRad: Rotation angle in radians.
     */
    void trackballRoll(ScalarType angleRad)
    {
        mTrackball.applyRoll(angleRad);
        requestUpdate();
    }

    // trackball gizmo

    /**
     * @brief Check if the trackball is visible.
     *
     * @return true if the trackball is visible, false otherwise.
     */
    bool isTrackBallVisible() const { return mTrackballGizmo.isVisible(); }

    /**
     * @brief Toggles the visibility of the trackball.
     */
    void toggleTrackBallVisibility()
    {
        mTrackballGizmo.setVisibility(!mTrackballGizmo.isVisible());
    }

    /**
     * @brief Sets the callback function that will be called when the user
     * presses the shortcut to toggle the trackball visibility (by default, the
     * shortcut is T).
     *
     * This is useful when the user wants to execute some custom code when the
     * trackball visibility is toggled through the shortcut.
     *
     * @param callback The function to execute.
     */
    void setShortcutToggleTrackballCallback(std::function<void(void)> callback)
    {
        mCustomShortcutToggleTrackballCallback = callback;
    }

    // event key/mouse assignments

    /**
     * @brief Returns a reference to the DragMotionMap to allow reading or
     * modifying drag bindings.
     */
    DragMotionMap& dragMotionMap() { return trackballSettings().dragMotionMap; }

    const DragMotionMap& dragMotionMap() const
    {
        return trackballSettings().dragMotionMap;
    }

    /**
     * @brief Returns a reference to the ScrollAtomicMap to allow reading or
     * modifying scroll bindings.
     */
    ScrollAtomicMap& scrollAtomicMap()
    {
        return trackballSettings().scrollAtomicMap;
    }

    const ScrollAtomicMap& scrollAtomicMap() const
    {
        return trackballSettings().scrollAtomicMap;
    }

    /**
     * @brief Returns a reference to the KeyAtomicMap to allow reading or
     * modifying key bindings.
     */
    KeyAtomicMap& keyAtomicMap() { return trackballSettings().keyAtomicMap; }

    const KeyAtomicMap& keyAtomicMap() const
    {
        return trackballSettings().keyAtomicMap;
    }

    /**
     * @brief Registers a new atomic action or overwrites an existing one.
     *
     * @param[in] name: The unique identifier/name for the action.
     * @param[in] callback: The function to execute when the action is
     * triggered.
     */
    void registerAtomicAction(
        const std::string&   name,
        AtomicActionCallback callback)
    {
        mAtomicActionRegistry[name] = callback;
    }

    // events

    void onDraw(uint viewId) override
    {
        Base::onDraw(viewId);

        mDirectionalLightGizmo.setVisibility(
            currentMotion() == TrackballMotionType::DIR_LIGHT_ARC);

        if (mDirectionalLightGizmo.isVisible()) {
            auto v = lightGizmoMatrix();
            mDirectionalLightGizmo.updateRotation(v);
            mDirectionalLightGizmo.draw(viewId);
        }

        if (mTrackballGizmo.isVisible()) {
            mTrackballGizmo.setTransform(gizmoMatrix());
            mTrackballGizmo.updateDragging(isDragging());
            mTrackballGizmo.draw(viewId);
        }
    }

    void onResize(unsigned int width, unsigned int height) override
    {
        resizeViewer(width, height);
    }

    bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        setKeyModifiers(modifiers);
        // handle shortcut for trackball visibility
        if (key == Key::T && modifiers[KeyModifier::NO_MODIFIER]) {
            if (mCustomShortcutToggleTrackballCallback)
                mCustomShortcutToggleTrackballCallback();
            return true;
        }
        keyPress(key);
        return false;
    }

    bool onKeyRelease(Key::Enum key, const KeyModifiers& modifiers) override
    {
        setKeyModifiers(modifiers);
        keyRelease(key);
        return false;
    }

    bool onMouseMove(double x, double y, const KeyModifiers& modifiers) override
    {
        setKeyModifiers(modifiers);
        moveMouse(x, y);
        return false;
    }

    bool onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        setKeyModifiers(modifiers);
        moveMouse(x, y);
        pressMouse(button);
        return false;
    }

    bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        setKeyModifiers(modifiers);
        moveMouse(x, y);
        releaseMouse(button);
        return false;
    }

    bool onMouseScroll(double dx, double dy, const KeyModifiers& modifiers)
        override
    {
        setKeyModifiers(modifiers);
        scroll(dx, dy);
        return false;
    }

protected:
    bool isDragging() const { return mTrackball.isDragging(); }

    TrackballMotionType currentMotion() const
    {
        return mTrackball.currentMotion();
    }

    void requestUpdate() { static_cast<DerivedRenderApp*>(this)->update(); }

private:
    void resizeViewer(uint w, uint h)
    {
        mWidth  = w;
        mHeight = h;
        mTrackball.setScreenSize(w, h);
    }

    void setKeyModifiers(KeyModifiers keys) { mCurrentKeyModifiers = keys; }

    void moveMouse(int x, int y)
    {
        // ugly AF
        auto actionOpt = dragMotionMap().action(
            std::make_pair(mCurrentMouseButton, mCurrentKeyModifiers));
        if (actionOpt.has_value()) {
            mTrackball.beginDragMotion(actionOpt.value());
        }
        mTrackball.setMousePosition(x, y);
        mTrackball.update();
    }

    void pressMouse(MouseButton::Enum button)
    {
        // if dragging, do not update the current mouse button
        if (mTrackball.isDragging()) {
            return;
        }

        mCurrentMouseButton = button;

        auto actionOpt = dragMotionMap().action(
            std::make_pair(button, mCurrentKeyModifiers));
        if (actionOpt.has_value()) {
            mTrackball.beginDragMotion(actionOpt.value());
            // no need to update here, it will be updated in moveMouse
            // for event driven rendering (e.g., Qt) this can trigger
            // an unwanted drag motion using the previous mouse position
        }
    }

    void releaseMouse(MouseButton::Enum button)
    {
        // if dragging, update the current mouse button only if it matches
        if (mTrackball.isDragging() && mCurrentMouseButton == button) {
            mCurrentMouseButton = MouseButton::NO_BUTTON;
        }

        auto actionOpt = dragMotionMap().action(
            std::make_pair(button, mCurrentKeyModifiers));
        if (actionOpt.has_value()) {
            mTrackball.endDragMotion(actionOpt.value());
            mTrackball.update();
        }
    }

    void scroll(Scalar pixelDeltaX, Scalar pixelDeltaY)
    {
        if (pixelDeltaX == 0 && pixelDeltaY == 0) {
            return;
        }

        if (pixelDeltaX != 0) {
            auto actionOpt = scrollAtomicMap().action(
                {ScrollAxis::HORIZONTAL, mCurrentKeyModifiers});
            if (actionOpt.has_value()) {
                mTrackball.applyAtomicMotion(actionOpt.value(), pixelDeltaX);
            }
        }

        if (pixelDeltaY != 0) {
            auto actionOpt = scrollAtomicMap().action(
                {ScrollAxis::VERTICAL, mCurrentKeyModifiers});
            if (actionOpt.has_value()) {
                mTrackball.applyAtomicMotion(actionOpt.value(), pixelDeltaY);
            }
        }
    }

    void keyPress(Key::Enum key)
    {
        // atomic motions are enabled while dragging
        auto actionNameOpt = keyAtomicMap().action({key, mCurrentKeyModifiers});
        if (actionNameOpt.has_value()) {
            auto it = mAtomicActionRegistry.find(actionNameOpt.value());
            if (it != mAtomicActionRegistry.end()) {
                it->second(*this);
            }
        }

        // dragging
        auto actionOpt = dragMotionMap().action(
            std::make_pair(mCurrentMouseButton, mCurrentKeyModifiers));
        if (actionOpt.has_value()) {
            mTrackball.beginDragMotion(actionOpt.value());
        }
        else {
            mTrackball.endDragMotion(currentMotion());
            mTrackball.update();
        }
    }

    void keyRelease(Key::Enum key)
    {
        // ugly solution to end drag motion when a key is released
        if (!mTrackball.isDragging())
            return;

        // dragging
        auto actionOpt = dragMotionMap().action(
            std::make_pair(mCurrentMouseButton, mCurrentKeyModifiers));
        if (actionOpt.has_value()) {
            mTrackball.beginDragMotion(actionOpt.value());
        }
        else {
            mTrackball.endDragMotion(currentMotion());
            mTrackball.update();
        }
    }

    static void rotate(
        TrackBallType&        t,
        const Point3<Scalar>& axis,
        Scalar                angle = M_PI / 6)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(TrackballMotionType::ARC, Args(axis, angle));
    }

    static void rotateLight(
        TrackBallType&        t,
        const Point3<Scalar>& axis,
        Scalar                angle = M_PI / 6)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(
            TrackballMotionType::DIR_LIGHT_ARC, Args(axis, angle));
    }

    static void translate(TrackBallType& t, const Point3<Scalar>& translation)
    {
        t.applyAtomicMotion(TrackballMotionType::PAN, translation);
    }

    static std::map<std::string, AtomicActionCallback>
    defaultAtomicActionRegistry()
    {
        using TBED = TrackBallEventDrawerT;

        return {
            {"Reset Trackball",
             [](TBED& d) {
                 d.reset();
             }},
            {"Reset Directional Light",
             [](TBED& d) {
                 d.mTrackball.resetDirectionalLight();
             }},

            // rotate
            {"Rotate X+",
             [](TBED& d) {
                 rotate(d.mTrackball, UNIT_X, DISCRETE_ROTATION_STEP);
             }},
            {"Rotate Y-",
             [](TBED& d) {
                 rotate(d.mTrackball, UNIT_Y, -DISCRETE_ROTATION_STEP);
             }},
            {"Rotate Y+",
             [](TBED& d) {
                 rotate(d.mTrackball, UNIT_Y, DISCRETE_ROTATION_STEP);
             }},
            {"Rotate X-",
             [](TBED& d) {
                 rotate(d.mTrackball, UNIT_X, -DISCRETE_ROTATION_STEP);
             }},

            // translate
            {"Translate Y+",
             [](TBED& d) {
                 translate(d.mTrackball, UNIT_Y * DISCRETE_TRANSLATION_STEP);
             }},
            {"Translate Y-",
             [](TBED& d) {
                 translate(d.mTrackball, -UNIT_Y * DISCRETE_TRANSLATION_STEP);
             }},
            {"Translate X-",
             [](TBED& d) {
                 translate(d.mTrackball, -UNIT_X * DISCRETE_TRANSLATION_STEP);
             }},
            {"Translate X+",
             [](TBED& d) {
                 translate(d.mTrackball, UNIT_X * DISCRETE_TRANSLATION_STEP);
             }},

            // set view
            {"View Front",
             [](TBED& d) {
                 d.mTrackball.reset();
             }},
            {"View Top",
             [](TBED& d) {
                 d.mTrackball.reset();
                 rotate(d.mTrackball, UNIT_X, M_PI_2);
             }},
            {"View Right",
             [](TBED& d) {
                 d.mTrackball.reset();
                 rotate(d.mTrackball, UNIT_Y, -M_PI_2);
             }},
            {"View Back",
             [](TBED& d) {
                 d.mTrackball.reset();
                 rotate(d.mTrackball, UNIT_Y, M_PI);
             }},
            {"View Bottom",
             [](TBED& d) {
                 d.mTrackball.reset();
                 rotate(d.mTrackball, UNIT_X, -M_PI_2);
             }},
            {"View Left",
             [](TBED& d) {
                 d.mTrackball.reset();
                 rotate(d.mTrackball, UNIT_Y, M_PI_2);
             }},

            // projection mode
            {"Toggle Projection",
             [](TBED& d) {
                 const auto v =
                     d.mTrackball.projectionMode() ==
                             Camera<Scalar>::ProjectionMode::PERSPECTIVE ?
                         Camera<Scalar>::ProjectionMode::ORTHO :
                         Camera<Scalar>::ProjectionMode::PERSPECTIVE;
                 d.mTrackball.setProjectionMode(v);
             }},

            // rotate light
            {"Rotate Light X+",
             [](TBED& d) {
                 rotateLight(d.mTrackball, UNIT_X, DISCRETE_ROTATION_STEP);
             }},
            {"Rotate Light Y-",
             [](TBED& d) {
                 rotateLight(d.mTrackball, UNIT_Y, -DISCRETE_ROTATION_STEP);
             }},
            {"Rotate Light Y+",
             [](TBED& d) {
                 rotateLight(d.mTrackball, UNIT_Y, DISCRETE_ROTATION_STEP);
             }},
            {"Rotate Light X-",
             [](TBED& d) {
                 rotateLight(d.mTrackball, UNIT_X, -DISCRETE_ROTATION_STEP);
             }},
        };
    }
};

template<typename DerivedRenderApp>
using TrackBallEventDrawer = TrackBallEventDrawerT<float, DerivedRenderApp>;

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_TRACKBALL_EVENT_DRAWER_H
