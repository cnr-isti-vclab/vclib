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

#include <vclib_opengl2/render/viewer_canvas.h>

#include <vclib/math/min_max.h>
#include <vclib/render/interfaces/drawable_mesh_i.h>
#include <vclib/space/box.h>

namespace vcl {

ViewerCanvas::ViewerCanvas(void* winId, uint width, uint height) :
        Canvas(winId, width, height), DTB(width, height)
{
    init(width, height);
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

void ViewerCanvas::init(uint width, uint height)
{
    Canvas::init(width, height);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    float lightAmbient[] = {.2f, .2f, .2f, 1.0f};
    float lightDiffuse[] = {.05f, .05f, .05f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto         tmp = light().direction();
    vcl::Point4f lPos(tmp.x(), tmp.y(), tmp.z(), 0.0f);

    glLightfv(GL_LIGHT0, GL_POSITION, lPos.data());

    glPushMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMultMatrixf(projectionMatrix().data());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixf(viewMatrix().data());

    for (DrawableObjectI* obj : *mDrawList)
        obj->draw(0);

    glPopMatrix();
}

void ViewerCanvas::onResize(unsigned int width, unsigned int height)
{
    Canvas::onResize(width, height);
    DTB::resizeViewer(width, height);
    update();
}

void ViewerCanvas::onKeyPress(Key::Enum key)
{
    DTB::setKeyModifiers(modifiers());

    switch (key) {
    case Key::C:
        std::cerr << "(" << DTB::camera().eye() << ") "
                  << "(" << DTB::camera().center() << ") "
                  << "(" << DTB::camera().up() << ")\n";
        break;

    default: keyPress(key); break;
    }

    update();
}

void ViewerCanvas::onKeyRelease(Key::Enum key)
{
    setKeyModifiers(modifiers());
    update();
}

void ViewerCanvas::onMouseMove(double x, double y)
{
    DTB::moveMouse(x, y);

    update();
}

void ViewerCanvas::onMousePress(MouseButton::Enum button)
{
    DTB::pressMouse(button);
    update();
}

void ViewerCanvas::onMouseRelease(MouseButton::Enum button)
{
    DTB::releaseMouse(button);
    update();
}

void ViewerCanvas::onMouseScroll(double dx, double dy)
{
    DTB::scroll(dx, dy);
    update();
}

void ViewerCanvas::initDrawableObject(DrawableObjectI& obj)
{
    obj.init();
}

} // namespace vcl
