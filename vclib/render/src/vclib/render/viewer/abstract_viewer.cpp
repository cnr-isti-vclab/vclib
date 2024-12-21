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

#include <vclib/render/viewer/abstract_viewer.h>

#include <iostream>

namespace vcl {

const DrawableObjectVector& AbstractViewer::drawableObjectVector() const
{
    return *mDrawList;
}

void AbstractViewer::setDrawableObjectVector(
    const std::shared_ptr<DrawableObjectVector>& v)
{
    mDrawList = v;

    for (auto obj : *mDrawList) {
        obj->init();
    }
    fitScene();
}

uint AbstractViewer::pushDrawableObject(const DrawableObject& obj)
{
    mDrawList->pushBack(obj);
    mDrawList->back()->init();
    return mDrawList->size() - 1;
}

void AbstractViewer::fitScene()
{
    Point3f sceneCenter;
    float   sceneRadius = 1;

    Box3d bb = mDrawList->boundingBox();

    if (!bb.isNull()) {
        sceneCenter = bb.center().cast<float>();
        sceneRadius = bb.diagonal();
    }

    DTB::setTrackBall(sceneCenter, sceneRadius);
}

void AbstractViewer::onResize(unsigned int width, unsigned int height)
{
    DTB::resizeViewer(width, height);
    // update(); TODO
}

void AbstractViewer::onKeyPress(Key::Enum key, const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);

    switch (key) {
    case Key::C:
        std::cout << "(" << DTB::camera().eye() << ") "
                  << "(" << DTB::camera().center() << ") "
                  << "(" << DTB::camera().up() << ")\n";
        std::cout << std::flush;
        break;

    case Key::A: toggleAxisVisibility(); break;

    case Key::T: toggleTrackBallVisibility(); break;

    default: break;
    }

    DTB::keyPress(key);
    // update(); TODO
}

void AbstractViewer::onKeyRelease(Key::Enum key, const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::keyRelease(key);
    // update(); TODO
}

void AbstractViewer::onMouseMove(
    double              x,
    double              y,
    const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::moveMouse(x, y);
    // update(); TODO
}

void AbstractViewer::onMousePress(
    MouseButton::Enum   button,
    double              x,
    double              y,
    const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::pressMouse(button);
    // update(); TODO
}

void AbstractViewer::onMouseRelease(
    MouseButton::Enum   button,
    double              x,
    double              y,
    const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::releaseMouse(button);
    // update(); TODO
}

void AbstractViewer::onMouseScroll(
    double              dx,
    double              dy,
    const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::scroll(dx, dy);
    // update(); TODO
}

} // namespace vcl::detail