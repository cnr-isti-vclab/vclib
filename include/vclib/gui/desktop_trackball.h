/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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
    using ScalarType = Scalar;
    using MatrixType = vcl::Matrix44<Scalar>;
    using TrackBallType = vcl::TrackBall<Scalar>;

private:
    using MotionType = vcl::TrackBall<Scalar>::MotionType;

    uint width = 0;
    uint height = 0;

    Scalar fov = 60.0;
    Scalar aspect = 1.0;
    Scalar near = 0.1;
    Scalar far = 100.0;

    vcl::Matrix44<Scalar> projMatrix =
        vcl::projectionMatrix<vcl::Matrix44<Scalar>>(fov, aspect, near, far, false);

    vcl::TrackBall<Scalar> trackball;
    float defaultTrackBallRadius = 1.0;

    KeyModifiers currentKeyModifiers = {NO_MODIFIER};

    std::map<std::pair<MouseButton, KeyModifiers>, MotionType> dragMotionMap =
    {
        {{LEFT, {NO_MODIFIER}}, TrackBallType::ARC},
        {{LEFT, {CONTROL}}, TrackBallType::PAN},
        {{LEFT, {SHIFT}}, TrackBallType::ZOOM},
        {{MIDDLE, {NO_MODIFIER}}, TrackBallType::PAN},
        {{MIDDLE, {CONTROL}}, TrackBallType::ROLL},
    };

    std::map<KeyModifiers, MotionType> wheelAtomicMap =
    {
        {{NO_MODIFIER}, TrackBallType::ZOOM},
        {{CONTROL}, TrackBallType::ROLL},
    };

    std::map<Key, std::function<void(TrackBallType& t)>> keyAtomicMap =
    {
        {KEY_R, [&](TrackBallType& t) { t.reset(defaultTrackBallRadius); }},

        // rotate
        {KEY_1, [](TrackBallType& t) { rotate(t, TrackBallType::AXIAL); }},
        {KEY_2, [](TrackBallType& t) { rotate(t, TrackBallType::VERTICAL); }},
        {KEY_4, [](TrackBallType& t) { rotate(t, TrackBallType::HORIZONTAL, -M_PI/6);}},
        {KEY_6, [](TrackBallType& t) { rotate(t, TrackBallType::HORIZONTAL); }},
        {KEY_8, [](TrackBallType& t) { rotate(t, TrackBallType::VERTICAL, -M_PI/6); }},
        {KEY_9, [](TrackBallType& t) { rotate(t, TrackBallType::AXIAL, -M_PI/6); }},

        // translate
        {KEY_W , [](TrackBallType& t) { translate(t, TrackBallType::VERTICAL, 1); }},
        {KEY_S , [](TrackBallType& t) { translate(t, TrackBallType::VERTICAL, -1); }},
        {KEY_A , [](TrackBallType& t) { translate(t, TrackBallType::HORIZONTAL, -1); }},
        {KEY_D , [](TrackBallType& t) { translate(t, TrackBallType::HORIZONTAL, 1); }},
    };

public:
    DesktopTrackBall() = default;

    const Camera<Scalar>& camera() const { return trackball.camera(); }

    const Matrix44<Scalar>& viewMatrix() const { return trackball.camera().matrix(); }

    const Matrix44<Scalar>& projectionMatrix() const { return projMatrix; }

    void resetTrackBall(Scalar radius = -1)
    {
        if (radius > 0.0)
            defaultTrackBallRadius = radius;
        trackball.reset(defaultTrackBallRadius);
    }

    void resizeViewer(uint w, uint h)
    {
        width = w;
        height = h;
        trackball.setScreenSize(w, h);
        aspect = static_cast<Scalar>(w) / static_cast<Scalar>(h);
        updateProjMatrix();
    }

    void setKeyModifiers(KeyModifiers keys)
    {
        currentKeyModifiers = keys;
    }

    void moveMouse(int x, int y)
    {
        trackball.setMousePosition(x, y);
        trackball.update();
    }

    void pressMouse(MouseButton button)
    {
        auto it = dragMotionMap.find(std::make_pair(button, currentKeyModifiers));
        if (it != dragMotionMap.end()) {
            trackball.beginDragMotion(it->second);
            trackball.update();
        }
    }

    void releaseMouse(MouseButton button)
    {
        auto it = dragMotionMap.find(std::make_pair(button, currentKeyModifiers));
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

    void keyPress(Key key)
    {
        auto it = keyAtomicMap.find(key);
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

    static void rotate(TrackBallType& t, TrackBallType::ViewAxis axis, Scalar angle = M_PI/6)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(TrackBallType::ARC, Args(axis, angle));
    }

    static void translate(TrackBallType& t, TrackBallType::ViewAxis axis, Scalar distance)
    {
        using Args = typename TrackBallType::TransformArgs;
        t.applyAtomicMotion(TrackBallType::PAN, Args(axis, distance));
    }

    void updateProjMatrix()
    {
        projMatrix =
            vcl::projectionMatrix<vcl::Matrix44<Scalar>>(fov, aspect, near, far, false);
    }
};

} // namespace vcl

#endif // VCL_GUI_DESKTOP_TRACKBALL_H
