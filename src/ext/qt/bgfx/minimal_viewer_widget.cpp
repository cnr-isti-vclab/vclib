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

#include <vclib/ext/qt/bgfx/minimal_viewer_widget.h>

#include <QMouseEvent>

#include <vclib/ext/qt/gui/input.h>
#include <vclib/ext/qt/gui/screen_shot_dialog.h>

namespace vcl::qbgf {

MinimalViewerWidget::MinimalViewerWidget(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWidget*           parent) :
        MinimalViewerWidget(
            std::make_shared<DrawableObjectVector>(),
            width,
            height,
            windowTitle,
            parent)
{
}

MinimalViewerWidget::MinimalViewerWidget(
    std::shared_ptr<DrawableObjectVector> v,
    uint                                  width,
    uint                                  height,
    const std::string&                    windowTitle,
    QWidget*                              parent) :
        CanvasWidget(windowTitle, width, height, parent),
        vcl::bgf::MinimalViewer(v)
{
}

MinimalViewerWidget::MinimalViewerWidget(QWidget* parent) :
        MinimalViewerWidget("Minimal Viewer", 1024, 768, parent)
{
}

void MinimalViewerWidget::draw()
{
    MV::draw(viewID());
}

void MinimalViewerWidget::onResize(unsigned int width, unsigned int height)
{
    MV::resizeViewer(width, height);
    bgfx::setViewTransform(
        viewID(), MV::viewMatrix().data(), MV::projectionMatrix().data());
}

void MinimalViewerWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() != Qt::NoButton) {
        MV::moveMouse(event->pos().x(), event->pos().y());

        bgfx::setViewTransform(
            viewID(), MV::viewMatrix().data(), MV::projectionMatrix().data());

        update();
    }

    CanvasWidget::mouseMoveEvent(event);
}

void MinimalViewerWidget::mousePressEvent(QMouseEvent* event)
{
    MV::moveMouse(event->pos().x(), event->pos().y());
    MV::pressMouse(vcl::qt::fromQt(event->button()));

    CanvasWidget::mousePressEvent(event);
}

void MinimalViewerWidget::mouseReleaseEvent(QMouseEvent* event)
{
    MV::moveMouse(event->pos().x(), event->pos().y());
    MV::releaseMouse(vcl::qt::fromQt(event->button()));

    CanvasWidget::mouseReleaseEvent(event);
}

void MinimalViewerWidget::wheelEvent(QWheelEvent* event)
{
    const int WHEEL_STEP = 120;
    float     notchY     = event->angleDelta().y() / float(WHEEL_STEP);

    MV::wheelMouse(notchY > 0);

    bgfx::setViewTransform(
        viewID(), MV::viewMatrix().data(), MV::projectionMatrix().data());

    update();

    CanvasWidget::wheelEvent(event);
}

void MinimalViewerWidget::keyPressEvent(QKeyEvent* event)
{
    qt::ScreenShotDialog dialog(this);
    MV::setKeyModifiers(vcl::qt::fromQt(event->modifiers()));

    switch (event->key()) {
    case Qt::Key_C:
        std::cerr << "(" << MV::camera().eye() << ") "
                  << "(" << MV::camera().center() << ") "
                  << "(" << MV::camera().up() << ")\n";
        break;

    case Qt::Key_S:
        if (event->modifiers() & Qt::ControlModifier) {
            if (dialog.exec() == QDialog::Accepted) {
                auto fs = dialog.selectedFiles();
                CanvasWidget::screenShot(fs.first().toStdString());
            }
            // the dialog stealed the focus, so we need to release the modifiers
            KeyModifiers md;
            md[NO_MODIFIER] = true;
            MV::setKeyModifiers(md);
        }
        break;

    default:
        MV::keyPress(vcl::qt::fromQt((Qt::Key) event->key()));
        bgfx::setViewTransform(
            viewID(), MV::viewMatrix().data(), MV::projectionMatrix().data());
        break;
    }

    update();
    CanvasWidget::keyPressEvent(event);
}

void MinimalViewerWidget::keyReleaseEvent(QKeyEvent* event)
{
    MV::setKeyModifiers(vcl::qt::fromQt(event->modifiers()));

    update();
    CanvasWidget::keyReleaseEvent(event);
}

} // namespace vcl::qbgf
