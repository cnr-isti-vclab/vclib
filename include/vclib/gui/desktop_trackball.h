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

    uint mWidth  = 1024;
    uint mHeight = 768;

    vcl::TrackBall<Scalar> mTrackball;

    Point3<Scalar> mDefaultTrackBallCenter;
    float          mDefaultTrackBallRadius = 1.0;

    KeyModifiers mCurrentKeyModifiers = {KeyModifier::NO_MODIFIER};

    std::map<std::pair<MouseButton::Enum, KeyModifiers>, MotionType>
        mDragMotionMap = {
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

    std::map<KeyModifiers, MotionType> mWheelAtomicMap = {
        {{KeyModifier::NO_MODIFIER}, TrackBallType::ZOOM},
        {{KeyModifier::CONTROL},     TrackBallType::ROLL},
    };

    std::map<
        std::pair<Key::Enum, KeyModifiers>,
        std::function<void(TrackBallType& t)>>
        mKeyAtomicMap = {
            {{Key::R, {KeyModifier::NO_MODIFIER}},
             [&](TrackBallType& t) {
                 t.reset(mDefaultTrackBallCenter, mDefaultTrackBallRadius);
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

    const DirectionalLight<Scalar>& light() const { return mTrackball.light(); }

    const Camera<Scalar>& camera() const { return mTrackball.camera(); }

    Matrix44<Scalar> viewMatrix() const
    {
        return mTrackball.camera().viewMatrix();
    }

    Matrix44<Scalar> projectionMatrix() const
    {
        return mTrackball.camera().projMatrix();
    }

    void resetTrackBall()
    {
        mTrackball.reset(mDefaultTrackBallCenter, mDefaultTrackBallRadius);
    }

    void setTrackBall(const Point3<Scalar>& center, Scalar radius)
    {
        mDefaultTrackBallCenter = center;
        mDefaultTrackBallRadius = radius;

        resetTrackBall();
    }

    void resizeViewer(uint w, uint h)
    {
        mWidth  = w;
        mHeight = h;
        mTrackball.setScreenSize(w, h);
    }

    void setKeyModifiers(KeyModifiers keys) { mCurrentKeyModifiers = keys; }

    void moveMouse(int x, int y)
    {
        mTrackball.setMousePosition(x, y);
        mTrackball.update();
    }

    void pressMouse(MouseButton::Enum button)
    {
        auto it =
            mDragMotionMap.find(std::make_pair(button, mCurrentKeyModifiers));
        if (it != mDragMotionMap.end()) {
            mTrackball.beginDragMotion(it->second);
            mTrackball.update();
        }
    }

    void releaseMouse(MouseButton::Enum button)
    {
        auto it =
            mDragMotionMap.find(std::make_pair(button, mCurrentKeyModifiers));
        if (it != mDragMotionMap.end()) {
            mTrackball.endDragMotion(it->second);
            mTrackball.update();
        }
    }

    void wheelMouse(bool up)
    {
        auto it = mWheelAtomicMap.find(mCurrentKeyModifiers);
        if (it != mWheelAtomicMap.end()) {
            mTrackball.applyAtomicMotion(it->second, up);
            mTrackball.update();
        }
    }

    void keyPress(Key::Enum key)
    {
        auto it = mKeyAtomicMap.find({key, mCurrentKeyModifiers});
        if (it != mKeyAtomicMap.end()) {
            it->second(mTrackball);
        }
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
};

} // namespace vcl

#endif // VCL_GUI_DESKTOP_TRACKBALL_H
