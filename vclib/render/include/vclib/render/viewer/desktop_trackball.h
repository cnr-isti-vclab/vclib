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

#ifndef VCL_RENDER_VIEWER_DESKTOP_TRACKBALL_H
#define VCL_RENDER_VIEWER_DESKTOP_TRACKBALL_H

#include <map>

#include <vclib/space/core/bit_set.h>

#include <vclib/render/input.h>

#include "trackball.h"

namespace vcl {

template<typename Scalar>
class DesktopTrackBall
{
public:
    using ScalarType    = Scalar;
    using MatrixType    = vcl::Matrix44<Scalar>;
    using TrackBallType = vcl::TrackBall<Scalar>;

    inline static const Point3<Scalar> UNIT_X = {1, 0, 0};
    inline static const Point3<Scalar> UNIT_Y = {0, 1, 0};

private:
    using MotionType = vcl::TrackBall<Scalar>::MotionType;

    // translation step in camera space
    static constexpr double DISCRETE_TRANSLATION_STEP = 0.1;
    static constexpr double DISCRETE_ROTATION_STEP    = M_PI_4 / 3.0; // 15 deg

    uint mWidth  = 1024;
    uint mHeight = 768;

    vcl::TrackBall<Scalar> mTrackball;

    Point3<Scalar> mDefaultTrackBallCenter;
    float          mDefaultTrackBallRadius = 1.0;

    KeyModifiers mCurrentKeyModifiers = {KeyModifier::NO_MODIFIER};

    std::map<std::pair<MouseButton::Enum, KeyModifiers>, MotionType>
        mDragMotionMap = {
            // clang-format off
            {{MouseButton::LEFT, {KeyModifier::NO_MODIFIER}},
             TrackBallType::ARC                                               },
            {{MouseButton::LEFT, {KeyModifier::CONTROL}}, TrackBallType::PAN  },
            {{MouseButton::LEFT, {KeyModifier::ALT}},     TrackBallType::ZMOVE},
            {{MouseButton::LEFT, {KeyModifier::SHIFT}},   TrackBallType::SCALE},
            {{MouseButton::MIDDLE, {KeyModifier::NO_MODIFIER}},
             TrackBallType::PAN                                               },
            {{MouseButton::MIDDLE, {KeyModifier::CONTROL}},
             TrackBallType::ROLL                                              },
            {{MouseButton::LEFT, {KeyModifier::SHIFT, KeyModifier::CONTROL}},
             TrackBallType::DIR_LIGHT_ARC                                     },
            // clang-format on
    };

    using Axis = unsigned char;
    std::map<std::pair<KeyModifiers, Axis>, MotionType> mScrollAtomicMap = {
        {{{KeyModifier::NO_MODIFIER}, 1}, TrackBallType::SCALE},
        {{{KeyModifier::CONTROL}, 1},     TrackBallType::ROLL },
        {{{KeyModifier::SHIFT}, 1},       TrackBallType::FOV  },
#ifdef __APPLE__
        {{{KeyModifier::SHIFT}, 0},       TrackBallType::FOV  },
#endif
    };

    std::map<
        std::pair<Key::Enum, KeyModifiers>,
        std::function<void(TrackBallType& t)>>
        mKeyAtomicMap = {
            {{Key::R, {KeyModifier::NO_MODIFIER}},
             [&](TrackBallType& t) {
                 t.reset(
                     mDefaultTrackBallCenter, 1.5 / mDefaultTrackBallRadius);
             }},
            {{Key::R, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [&](TrackBallType& t) {
                 t.resetDirectionalLight();
             }},

            // rotate
            {{Key::NP_2, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, UNIT_X, DISCRETE_ROTATION_STEP);
             }},
            {{Key::NP_4, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, UNIT_Y, -DISCRETE_ROTATION_STEP);
             }},
            {{Key::NP_6, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, UNIT_Y, DISCRETE_ROTATION_STEP);
             }},
            {{Key::NP_8, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 rotate(t, UNIT_X, -DISCRETE_ROTATION_STEP);
             }},

            // translate
            {{Key::UP, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 translate(t, UNIT_Y * DISCRETE_TRANSLATION_STEP);
             }},
            {{Key::DOWN, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 translate(t, -UNIT_Y * DISCRETE_TRANSLATION_STEP);
             }},
            {{Key::LEFT, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 translate(t, -UNIT_X * DISCRETE_TRANSLATION_STEP);
             }},
            {{Key::RIGHT, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) {
                 translate(t, UNIT_X * DISCRETE_TRANSLATION_STEP);
             }},

            // set view
            {{Key::NP_1, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) { // front
                 t.reset();
             }},
            {{Key::NP_7, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) { // top
                 t.reset();
                 rotate(t, UNIT_X, M_PI_2);
             }},
            {{Key::NP_3, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) { // right
                 t.reset();
                 rotate(t, UNIT_Y, -M_PI_2);
             }},
            {{Key::NP_1, {KeyModifier::CONTROL}},
             [](TrackBallType& t) { // back
                 t.reset();
                 rotate(t, UNIT_Y, M_PI);
             }},
            {{Key::NP_7, {KeyModifier::CONTROL}},
             [](TrackBallType& t) { // bottom
                 t.reset();
                 rotate(t, UNIT_X, -M_PI_2);
             }},
            {{Key::NP_3, {KeyModifier::CONTROL}},
             [](TrackBallType& t) { // left
                 t.reset();
                 rotate(t, UNIT_Y, M_PI_2);
             }},
            // projection mode
            {{Key::NP_5, {KeyModifier::NO_MODIFIER}},
             [](TrackBallType& t) { // reset
                 const auto v =
                     t.projectionMode() ==
                             Camera<Scalar>::ProjectionMode::PERSPECTIVE ?
                         Camera<Scalar>::ProjectionMode::ORTHO :
                         Camera<Scalar>::ProjectionMode::PERSPECTIVE;
                 t.setProjectionMode(v);
             }},

            // rotate light
            {{Key::NP_2, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [](TrackBallType& t) {
                 rotateLight(t, UNIT_X, DISCRETE_ROTATION_STEP);
             }},
            {{Key::NP_4, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [](TrackBallType& t) {
                 rotateLight(t, UNIT_Y, -DISCRETE_ROTATION_STEP);
             }},
            {{Key::NP_6, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [](TrackBallType& t) {
                 rotateLight(t, UNIT_Y, DISCRETE_ROTATION_STEP);
             }},
            {{Key::NP_8, {KeyModifier::CONTROL, KeyModifier::SHIFT}},
             [](TrackBallType& t) {
                 rotateLight(t, UNIT_X, -DISCRETE_ROTATION_STEP);
             }},
    };

public:
    DesktopTrackBall(uint width = 1024, uint height = 768)
    {
        resizeViewer(width, height);
    }

    bool isDragging() const { return mTrackball.isDragging(); }

    DirectionalLight<Scalar> light() const { return mTrackball.light(); }

    const Camera<Scalar>& camera() const { return mTrackball.camera(); }

    Matrix44<Scalar> viewMatrix() const { return mTrackball.viewMatrix(); }

    Matrix44<Scalar> lightGizmoMatrix() const
    {
        return mTrackball.lightGizmoMatrix();
    }

    Matrix44<Scalar> gizmoMatrix() const { return mTrackball.gizmoMatrix(); }

    Matrix44<Scalar> projectionMatrix() const
    {
        return mTrackball.projectionMatrix();
    }

    void resetTrackBall()
    {
        mTrackball.reset(
            mDefaultTrackBallCenter, 1.5 / mDefaultTrackBallRadius);
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
            // no need to update here, it will be updated in moveMouse
            // for event driven rendering (e.g., Qt) this can trigger
            // an unwanted drag motion using the previous mouse position
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

    void scroll(Scalar pixelDeltaX, Scalar pixelDeltaY)
    {
        if (pixelDeltaX == 0 && pixelDeltaY == 0) {
            return;
        }

        if (pixelDeltaX != 0) {
            auto it = mScrollAtomicMap.find({mCurrentKeyModifiers, Axis(0)});
            if (it != mScrollAtomicMap.end()) {
                mTrackball.applyAtomicMotion(it->second, pixelDeltaX);
            }
        }

        if (pixelDeltaY != 0) {
            auto it = mScrollAtomicMap.find({mCurrentKeyModifiers, Axis(1)});
            if (it != mScrollAtomicMap.end()) {
                mTrackball.applyAtomicMotion(it->second, pixelDeltaY);
            }
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
        TrackBallType&        t,
        const Point3<Scalar>& axis,
        Scalar                angle = M_PI / 6)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(TrackBallType::ARC, Args(axis, angle));
    }

    static void rotateLight(
        TrackBallType&        t,
        const Point3<Scalar>& axis,
        Scalar                angle = M_PI / 6)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(TrackBallType::DIR_LIGHT_ARC, Args(axis, angle));
    }

    static void translate(TrackBallType& t, const Point3<Scalar>& translation)
    {
        t.applyAtomicMotion(TrackBallType::PAN, translation);
    }
};

} // namespace vcl

#endif // VCL_RENDER_VIEWER_DESKTOP_TRACKBALL_H
