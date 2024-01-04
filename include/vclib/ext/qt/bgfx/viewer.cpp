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

#include "viewer.h"

namespace vcl::qbgf {

Viewer::Viewer(bgfx::RendererType::Enum renderType, QWindow* parent) :
        CanvasWindow(renderType, parent)
{
    DTB::resetTrackBall(1);

    bgfx::setViewTransform(
        0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());
}

void Viewer::onResize(unsigned int width, unsigned int height)
{
    DTB::resizeViewer(width, height);
    bgfx::setViewTransform(
        0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());
}

void Viewer::mouseMoveEvent(QMouseEvent* event)
{
    DTB::moveMouse(event->pos().x(), event->pos().y());

    bgfx::setViewTransform(
        0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());

    update();

    CanvasWindow::mouseMoveEvent(event);
}

void Viewer::mousePressEvent(QMouseEvent* event)
{
    DTB::moveMouse(event->pos().x(), event->pos().y());
    DTB::pressMouse(vcl::qt::fromQt(event->button()));

    CanvasWindow::mousePressEvent(event);
}

void Viewer::mouseReleaseEvent(QMouseEvent* event)
{
    DTB::moveMouse(event->pos().x(), event->pos().y());
    DTB::releaseMouse(vcl::qt::fromQt(event->button()));

    CanvasWindow::mouseReleaseEvent(event);
}

void Viewer::wheelEvent(QWheelEvent* event)
{
    const int WHEEL_STEP = 120;
    float     notchY     = event->angleDelta().y() / float(WHEEL_STEP);

    DTB::wheelMouse(notchY > 0);

    bgfx::setViewTransform(
        0, DTB::viewMatrix().data(), DTB::projectionMatrix().data());

    update();

    CanvasWindow::wheelEvent(event);
}

void Viewer::keyPressEvent(QKeyEvent* event)
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

void Viewer::keyReleaseEvent(QKeyEvent* event)
{
    DTB::setKeyModifiers(vcl::qt::fromQt(event->modifiers()));

    update();
    CanvasWindow::keyReleaseEvent(event);
}

} // namespace vcl::qbgf
