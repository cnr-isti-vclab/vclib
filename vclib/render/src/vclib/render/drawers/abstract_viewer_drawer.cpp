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

#include <vclib/render/drawers/abstract_viewer_drawer.h>

#include <iostream>

namespace vcl {

const DrawableObjectVector& AbstractViewerDrawer::drawableObjectVector() const
{
    return *mDrawList;
}

void AbstractViewerDrawer::setDrawableObjectVector(
    const std::shared_ptr<DrawableObjectVector>& v)
{
    mDrawList = v;

    for (auto obj : *mDrawList) {
        obj->init();
    }
    fitScene();
}

uint AbstractViewerDrawer::pushDrawableObject(const DrawableObject& obj)
{
    mDrawList->pushBack(obj);
    mDrawList->back()->init();
    return mDrawList->size() - 1;
}

void AbstractViewerDrawer::fitScene()
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

void AbstractViewerDrawer::onResize(unsigned int width, unsigned int height)
{
    DTB::resizeViewer(width, height);
    // update(); TODO
}

void AbstractViewerDrawer::onKeyPress(Key::Enum key, const KeyModifiers& modifiers)
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

void AbstractViewerDrawer::onKeyRelease(Key::Enum key, const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::keyRelease(key);
    // update(); TODO
}

void AbstractViewerDrawer::onMouseMove(
    double              x,
    double              y,
    const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::moveMouse(x, y);
    // update(); TODO
}

void AbstractViewerDrawer::onMousePress(
    MouseButton::Enum   button,
    double              x,
    double              y,
    const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::moveMouse(x, y);
    DTB::pressMouse(button);
    // update(); TODO
}

void AbstractViewerDrawer::onMouseRelease(
    MouseButton::Enum   button,
    double              x,
    double              y,
    const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::moveMouse(x, y);
    DTB::releaseMouse(button);
    // update(); TODO
}

void AbstractViewerDrawer::onMouseScroll(
    double              dx,
    double              dy,
    const KeyModifiers& modifiers)
{
    DTB::setKeyModifiers(modifiers);
    DTB::scroll(dx, dy);
    // update(); TODO
}

} // namespace vcl::detail
