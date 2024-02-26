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

#ifndef VCL_GUI_DESKTOP_TRACKBALL_H
#define VCL_GUI_DESKTOP_TRACKBALL_H

#include "input.h"

#include "vclib/render/trackball.h"

#include <vclib/space/bit_set.h>

#include <map>

namespace vcl {

template<typename Scalar>
class DesktopTrackBall
{
public:
    using ScalarType    = Scalar;
    using MatrixType    = vcl::Matrix44<Scalar>;
    using TrackBallType = vcl::TrackBall<Scalar>;

private:
    using MotionType = vcl::TrackBall<Scalar>::MotionType;

    uint width  = 1024;
    uint height = 768;

    Scalar fov    = 60.0;
    Scalar aspect = 1.0;
    Scalar near   = 0.1;
    Scalar far    = 500.0;

    vcl::Matrix44<Scalar> projMatrix =
        vcl::projectionMatrix<vcl::Matrix44<Scalar>>(
            fov,
            aspect,
            near,
            far,
            false);

    vcl::TrackBall<Scalar> trackball;

    Point3<Scalar> defaultTrackBallCenter;
    float          defaultTrackBallRadius = 1.0;

    KeyModifiers currentKeyModifiers = {KeyModifier::NO_MODIFIER};

    std::map<std::pair<MouseButton::Enum, KeyModifiers>, MotionType>
        dragMotionMap = {
            {{MouseButton::LEFT, {KeyModifier::NO_MODIFIER}},
             TrackBallType::ARC                                                                  },
            {{MouseButton::LEFT, {KeyModifier::CONTROL}},                     TrackBallType::PAN },
            {{MouseButton::LEFT, {KeyModifier::SHIFT}},                       TrackBallType::ZOOM},
            {{MouseButton::MIDDLE, {KeyModifier::NO_MODIFIER}},
             TrackBallType::PAN                                                                  },
            {{MouseButton::MIDDLE, {KeyModifier::CONTROL}},
             TrackBallType::ROLL                                                                 },
            {{MouseButton::LEFT, {KeyModifier::SHIFT, KeyModifier::CONTROL}},
             TrackBallType::DIR_LIGHT_ARC                                                        },
    };

    std::map<KeyModifiers, MotionType> wheelAtomicMap = {
        {{KeyModifier::NO_MODIFIER}, TrackBallType::ZOOM},
        {{KeyModifier::CONTROL},     TrackBallType::ROLL},
    };

    std::map<
        std::pair<Key::Enum, KeyModifiers>,
        std::function<void(TrackBallType& t)>>
        keyAtomicMap = {
            {{Key::R, {KeyModifier::NO_MODIFIER}},
             [&](TrackBallType& t) {
                 t.reset(defaultTrackBallCenter, defaultTrackBallRadius);
             }},
            {{Key::R, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [&](TrackBallType& t) {
                 t.resetDirectionalLight();
             }},

 // rotate
            {{Key::NP_1, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, TrackBallType::AXIAL, -M_PI / 6);
             }},
            {{Key::NP_2, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, TrackBallType::VERTICAL, -M_PI / 6);
             }},
            {{Key::NP_4, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, TrackBallType::HORIZONTAL, -M_PI / 6);
             }},
            {{Key::NP_6, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, TrackBallType::HORIZONTAL);
             }},
            {{Key::NP_8, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, TrackBallType::VERTICAL);
             }},
            {{Key::NP_9, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, TrackBallType::AXIAL);
             }},

 // translate
            {{Key::UP, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 translate(t, TrackBallType::VERTICAL, 1);
             }},
            {{Key::DOWN, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 translate(t, TrackBallType::VERTICAL, -1);
             }},
            {{Key::LEFT, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 translate(t, TrackBallType::HORIZONTAL, -1);
             }},
            {{Key::RIGHT, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 translate(t, TrackBallType::HORIZONTAL, 1);
             }},

 // rotate light
            {{Key::NP_2, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [](TrackBallType& t) {
                 rotateLight(t, TrackBallType::VERTICAL, -M_PI / 6);
             }},
            {{Key::NP_4, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [](TrackBallType& t) {
                 rotateLight(t, TrackBallType::HORIZONTAL, -M_PI / 6);
             }},
            {{Key::NP_6, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [](TrackBallType& t) {
                 rotateLight(t, TrackBallType::HORIZONTAL);
             }},
            {{Key::NP_8, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [](TrackBallType& t) {
                 rotateLight(t, TrackBallType::VERTICAL);
             }},
    };

public:
    DesktopTrackBall(uint width = 1024, uint height = 768)
    {
        resizeViewer(width, height);
    }

    const DirectionalLight<Scalar>& light() const { return trackball.light(); }

    const Camera<Scalar>& camera() const { return trackball.camera(); }

    const Matrix44<Scalar>& viewMatrix() const
    {
        return trackball.camera().matrix();
    }

    const Matrix44<Scalar>& projectionMatrix() const { return projMatrix; }

    void resetTrackBall()
    {
        trackball.reset(defaultTrackBallCenter, defaultTrackBallRadius);
    }

    void setTrackBall(const Point3<Scalar>& center, Scalar radius)
    {
        defaultTrackBallCenter = center;
        defaultTrackBallRadius = radius;

        resetTrackBall();
    }

    void resizeViewer(uint w, uint h)
    {
        width  = w;
        height = h;
        trackball.setScreenSize(w, h);
        aspect = static_cast<Scalar>(w) / static_cast<Scalar>(h);
        updateProjMatrix();
    }

    void setKeyModifiers(KeyModifiers keys) { currentKeyModifiers = keys; }

    void moveMouse(int x, int y)
    {
        trackball.setMousePosition(x, y);
        trackball.update();
    }

    void pressMouse(MouseButton::Enum button)
    {
        auto it =
            dragMotionMap.find(std::make_pair(button, currentKeyModifiers));
        if (it != dragMotionMap.end()) {
            trackball.beginDragMotion(it->second);
            trackball.update();
        }
    }

    void releaseMouse(MouseButton::Enum button)
    {
        auto it =
            dragMotionMap.find(std::make_pair(button, currentKeyModifiers));
        if (it != dragMotionMap.end()) {
            trackball.endDragMotion(it->second);
            trackball.update();
        }
    }

    void wheelMouse(bool up)
    {
        auto it = wheelAtomicMap.find(currentKeyModifiers);
        if (it != wheelAtomicMap.end()) {
            trackball.applyAtomicMotion(it->second, up);
            trackball.update();
        }
    }

    void keyPress(Key::Enum key)
    {
        auto it = keyAtomicMap.find({key, currentKeyModifiers});
        if (it != keyAtomicMap.end()) {
            it->second(trackball);
        }
    }

    void setFov(Scalar fov)
    {
        this->fov = fov;
        updateProjMatrix();
    }

private:
    static void rotate(
        TrackBallType&          t,
        TrackBallType::ViewAxis axis,
        Scalar                  angle = M_PI / 6)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(TrackBallType::ARC, Args(axis, angle));
    }

    static void rotateLight(
        TrackBallType&          t,
        TrackBallType::ViewAxis axis,
        Scalar                  angle = M_PI / 6)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(TrackBallType::DIR_LIGHT_ARC, Args(axis, angle));
    }

    static void translate(
        TrackBallType&          t,
        TrackBallType::ViewAxis axis,
        Scalar                  distance)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(TrackBallType::PAN, Args(axis, distance));
    }

    void updateProjMatrix()
    {
        projMatrix = vcl::projectionMatrix<vcl::Matrix44<Scalar>>(
            fov, aspect, near, far, false);
    }
};

} // namespace vcl

#endif // VCL_GUI_DESKTOP_TRACKBALL_H
