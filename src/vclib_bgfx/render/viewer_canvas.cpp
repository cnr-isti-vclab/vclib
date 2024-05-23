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

#include <vclib_bgfx/render/viewer_canvas.h>

#include <vclib/math/min_max.h>
#include <vclib/render/interfaces/drawable_mesh_i.h>
#include <vclib/space/box.h>

namespace vcl {

ViewerCanvas::ViewerCanvas(void* winId, uint width, uint height) :
        Canvas(winId, width, height),
        DTB(width, height)
{
    mCameraUniforms.updateCamera(DTB::camera());
    mDirectionalLightUniforms.updateLight(DTB::light());
}

ViewerCanvas::ViewerCanvas(
    void*                                 winId,
    std::shared_ptr<DrawableObjectVector> v,
    uint                                  width,
    uint                                  height) :
        ViewerCanvas(winId, width, height)
{
    setDrawableObjectVector(v);
}

const DrawableObjectVector& ViewerCanvas::drawableObjectVector() const
{
    return *mDrawList;
}

void ViewerCanvas::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    mDrawList = v;

    for (DrawableObjectI* obj : *mDrawList) {
        initDrawableObject(*obj);
    }
    fitScene();
}

uint ViewerCanvas::pushDrawableObject(const DrawableObjectI& obj)
{
    mDrawList->pushBack(obj);
    initDrawableObject(mDrawList->back());
    return mDrawList->size() - 1;
}

void ViewerCanvas::fitScene()
{
    Point3f sceneCenter;
    float   sceneRadius = 1;

    Box3d bb = mDrawList->boundingBox();

    if (!bb.isNull()) {
        sceneCenter = bb.center().cast<float>();
        sceneRadius = bb.diagonal() / 2;
    }

    DTB::setTrackBall(sceneCenter, sceneRadius);
}

void ViewerCanvas::draw()
{
    bgfx::setViewTransform(
        viewId(), viewMatrix().data(), projectionMatrix().data());

    mCameraUniforms.updateCamera(DTB::camera());
    mCameraUniforms.bind();

    mDirectionalLightUniforms.bind();

    for (DrawableObjectI* obj : *mDrawList)
        obj->draw(viewId());

    if (mAxis.isVisible()) {
        mAxis.draw(viewId());
    }

    if (mDirectionalLight.isVisible()) {
        mDirectionalLight.draw(viewId());
    }

    if (mTrackBall.isVisible()) {
        mTrackBall.draw(viewId());
    }
}

void ViewerCanvas::onResize(unsigned int width, unsigned int height)
{
    Canvas::onResize(width, height);
    DTB::resizeViewer(width, height);
    updateDrawableTrackball();
    update();
}

void ViewerCanvas::onKeyPress(Key::Enum key)
{
    DTB::setKeyModifiers(modifiers());

    if (modifiers()[KeyModifier::CONTROL] && modifiers()[KeyModifier::SHIFT]) {
        setDirectionalLightVisibility(true);
    }

    switch (key) {
    case Key::C:
        std::cerr << "(" << DTB::camera().eye() << ") "
                  << "(" << DTB::camera().center() << ") "
                  << "(" << DTB::camera().up() << ")\n";
        break;

    case Key::A: toggleAxisVisibility(); break;

    case Key::T: toggleTrackballVisibility(); break;

    default: keyPress(key); break;
    }

    updateDirectionalLight();
    updateDrawableTrackball();

    update();
}

void ViewerCanvas::onKeyRelease(Key::Enum key)
{
    if (isDirectionalLightVisible()) {
        if (!modifiers()[KeyModifier::CONTROL] ||
            !modifiers()[KeyModifier::SHIFT])
        {
            setDirectionalLightVisibility(false);
        }
    }

    setKeyModifiers(modifiers());
    update();
}

void ViewerCanvas::onMouseMove(double x, double y)
{
    DTB::moveMouse(x, y);
    updateDirectionalLight();
    updateDrawableTrackball();

    update();
}

void ViewerCanvas::onMousePress(MouseButton::Enum button)
{
    DTB::pressMouse(button);
    updateDrawableTrackball();
    update();
}

void ViewerCanvas::onMouseRelease(MouseButton::Enum button)
{
    DTB::releaseMouse(button);
    updateDrawableTrackball();
    update();
}

void ViewerCanvas::onMouseScroll(double dx, double dy)
{
    DTB::scroll(dx, dy);
    updateDrawableTrackball();
    update();
}

void ViewerCanvas::initDrawableObject(DrawableObjectI& obj)
{
    obj.init();
}

} // namespace vcl
