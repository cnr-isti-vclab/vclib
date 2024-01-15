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

#include <vclib/ext/qt/bgfx/minimal_viewer_window.h>

#include <iostream>

namespace vcl::qbgf {

MinimalViewerWindow::MinimalViewerWindow(
    bgfx::RendererType::Enum renderType,
    QWindow*                 parent) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            renderType,
            parent)
{
}

MinimalViewerWindow::MinimalViewerWindow(QWindow* parent) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            bgfx::RendererType::Count,
            parent)
{
}

MinimalViewerWindow::MinimalViewerWindow(
    std::shared_ptr<DrawableObjectVector> v,
    bgfx::RendererType::Enum              renderType,
    QWindow*                              parent) :
        CanvasWindow(renderType, parent),
        vcl::bgf::MinimalViewer(v)
{
}

void MinimalViewerWindow::draw()
{
    MV::draw();
}

void MinimalViewerWindow::onResize(unsigned int width, unsigned int height)
{
    MV::resizeViewer(width, height);
    bgfx::setViewTransform(
        0, MV::viewMatrix().data(), MV::projectionMatrix().data());
}

void MinimalViewerWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() != Qt::NoButton) {
        MV::moveMouse(event->pos().x(), event->pos().y());

        bgfx::setViewTransform(
            0, MV::viewMatrix().data(), MV::projectionMatrix().data());

        update();
    }

    CanvasWindow::mouseMoveEvent(event);
}

void MinimalViewerWindow::mousePressEvent(QMouseEvent* event)
{
    MV::moveMouse(event->pos().x(), event->pos().y());
    MV::pressMouse(vcl::qt::fromQt(event->button()));

    CanvasWindow::mousePressEvent(event);
}

void MinimalViewerWindow::mouseReleaseEvent(QMouseEvent* event)
{
    MV::moveMouse(event->pos().x(), event->pos().y());
    MV::releaseMouse(vcl::qt::fromQt(event->button()));

    CanvasWindow::mouseReleaseEvent(event);
}

void MinimalViewerWindow::wheelEvent(QWheelEvent* event)
{
    const int WHEEL_STEP = 120;
    float     notchY     = event->angleDelta().y() / float(WHEEL_STEP);

    MV::wheelMouse(notchY > 0);

    bgfx::setViewTransform(
        0, MV::viewMatrix().data(), MV::projectionMatrix().data());

    update();

    CanvasWindow::wheelEvent(event);
}

void MinimalViewerWindow::keyPressEvent(QKeyEvent* event)
{
    MV::setKeyModifiers(vcl::qt::fromQt(event->modifiers()));

    switch (event->key()) {
    case Qt::Key_C:
        std::cerr << "(" << MV::camera().eye() << ") "
                  << "(" << MV::camera().center() << ") "
                  << "(" << MV::camera().up() << ")\n";
        break;

    default:
        MV::keyPress(vcl::qt::fromQt((Qt::Key) event->key()));
        bgfx::setViewTransform(
            0, MV::viewMatrix().data(), MV::projectionMatrix().data());
        break;
    }

    update();
    CanvasWindow::keyPressEvent(event);
}

void MinimalViewerWindow::keyReleaseEvent(QKeyEvent* event)
{
    MV::setKeyModifiers(vcl::qt::fromQt(event->modifiers()));

    update();
    CanvasWindow::keyReleaseEvent(event);
}

} // namespace vcl::qbgf
