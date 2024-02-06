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

#include <vclib/ext/qt/gui/screen_shot_dialog.h>

#include <iostream>

namespace vcl::qbgf {

MinimalViewerWindow::MinimalViewerWindow(
    std::shared_ptr<DrawableObjectVector> v,
    const std::string&                    windowTitle,
    uint                                  width,
    uint                                  height,
    QWindow*                              parent) :
        CanvasWindow(windowTitle, width, height, parent),
        vcl::bgf::MinimalViewer(v, width, height)
{
}

MinimalViewerWindow::MinimalViewerWindow(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWindow*           parent) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            windowTitle,
            width,
            height,
            parent)
{
}

MinimalViewerWindow::MinimalViewerWindow(
    uint     width,
    uint     height,
    QWindow* parent) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            "Minimal Viewer",
            width,
            height,
            parent)
{
}

MinimalViewerWindow::MinimalViewerWindow(QWindow* parent) :
        MinimalViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            "Minimal Viewer",
            1024,
            768,
            parent)
{
}

void MinimalViewerWindow::draw()
{
    MV::draw(viewID());
}

void MinimalViewerWindow::onResize(unsigned int width, unsigned int height)
{
    MV::resizeViewer(width, height);
    update();
}

void MinimalViewerWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() != Qt::NoButton) {
        MV::moveMouse(event->pos().x(), event->pos().y());
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

    update();

    CanvasWindow::wheelEvent(event);
}

void MinimalViewerWindow::keyPressEvent(QKeyEvent* event)
{
    int res = 0;
    float screenMultiplier = 1.0f;
    qt::ScreenShotDialog dialog;
    MV::setKeyModifiers(vcl::qt::fromQt(event->modifiers()));

    switch (event->key()) {
    case Qt::Key_C:
        std::cerr << "(" << MV::camera().eye() << ") "
                  << "(" << MV::camera().center() << ") "
                  << "(" << MV::camera().up() << ")\n";
        break;
    case Qt::Key_S:
        // screenshot
        res = dialog.exec();
        break;

    default:
        MV::keyPress(vcl::qt::fromQt((Qt::Key) event->key())); break;
    }

    if (res == QDialog::Accepted) {
        auto sf = dialog.selectedFiles();
        if (!sf.empty()) {
            std::string fn = sf.first().toStdString();
            screenMultiplier = dialog.screenMultiplierValue();
            screenShot(fn, width() * screenMultiplier, height() * screenMultiplier);
        }
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
