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

#include "minimal_viewer_window.h"

#include <iostream>

namespace vcl::qbgf {

MinimalViewerWindow::MinimalViewerWindow(bgfx::RendererType::Enum renderType, QWindow* parent) :
        MinimalViewerWindow(std::make_shared<DrawableObjectVector>(), renderType, parent)
{
}


MinimalViewerWindow::MinimalViewerWindow(
    std::shared_ptr<DrawableObjectVector> v,
    bgfx::RendererType::Enum              renderType,
    QWindow*                              parent) :
        CanvasWindow(renderType, parent), drawList(v)
{
    bgfx::setViewTransform(
        0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());

    for (DrawableObject* d : *drawList) {
        d->init();
    }
}

void MinimalViewerWindow::setDrawableObjectVector(
    std::shared_ptr<DrawableObjectVector> v)
{
    drawList = v;
}


std::shared_ptr<const DrawableObjectVector> MinimalViewerWindow::
    drawableObjectVector() const
{
    return drawList;
}


void MinimalViewerWindow::fitScene()
{
    Box3d   bb          = drawList->boundingBox();
    Point3f sceneCenter = bb.center().cast<float>();
    float   sceneRadius = bb.diagonal() / 2;

    DTB::setTrackBall(sceneCenter, sceneRadius);
}


void MinimalViewerWindow::draw()
{
    // This dummy draw call is here to make sure that view 0 is cleared
    // if no other draw calls are submitted to view 0.
    bgfx::touch(0);

    for (const DrawableObject* obj : *drawList)
        obj->draw();
}

void MinimalViewerWindow::onResize(unsigned int width, unsigned int height)
{
    DTB::resizeViewer(width, height);
    bgfx::setViewTransform(
        0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());
}

void MinimalViewerWindow::mouseMoveEvent(QMouseEvent* event)
{
    DTB::moveMouse(event->pos().x(), event->pos().y());

    bgfx::setViewTransform(
        0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());

    update();

    CanvasWindow::mouseMoveEvent(event);
}

void MinimalViewerWindow::mousePressEvent(QMouseEvent* event)
{
    DTB::moveMouse(event->pos().x(), event->pos().y());
    DTB::pressMouse(vcl::qt::fromQt(event->button()));

    CanvasWindow::mousePressEvent(event);
}

void MinimalViewerWindow::mouseReleaseEvent(QMouseEvent* event)
{
    DTB::moveMouse(event->pos().x(), event->pos().y());
    DTB::releaseMouse(vcl::qt::fromQt(event->button()));

    CanvasWindow::mouseReleaseEvent(event);
}

void MinimalViewerWindow::wheelEvent(QWheelEvent* event)
{
    const int WHEEL_STEP = 120;
    float     notchY     = event->angleDelta().y() / float(WHEEL_STEP);

    DTB::wheelMouse(notchY > 0);

    bgfx::setViewTransform(
        0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());

    update();

    CanvasWindow::wheelEvent(event);
}

void MinimalViewerWindow::keyPressEvent(QKeyEvent* event)
{
    DTB::setKeyModifiers(vcl::qt::fromQt(event->modifiers()));

    switch (event->key()) {
    case Qt::Key_C:
        std::cerr << "(" << DTB::camera().eye() << ") "
                  << "(" << DTB::camera().center() << ") "
                  << "(" << DTB::camera().up() << ")\n";
        break;

    default:
        DTB::keyPress(vcl::qt::fromQt((Qt::Key) event->key()));
        bgfx::setViewTransform(
            0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());
        break;
    }

    update();
    CanvasWindow::keyPressEvent(event);
}

void MinimalViewerWindow::keyReleaseEvent(QKeyEvent* event)
{
    DTB::setKeyModifiers(vcl::qt::fromQt(event->modifiers()));

    update();
    CanvasWindow::keyReleaseEvent(event);
}

} // namespace vcl::qbgf
