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
    MV::draw(viewId());
}

void MinimalViewerWidget::onResize(unsigned int width, unsigned int height)
{
    MV::resizeViewer(width, height);
    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
}

void MinimalViewerWidget::onKeyPress(Key::Enum key)
{
    qt::ScreenShotDialog dialog(this);
    MV::setKeyModifiers(modifiers());

    switch (key) {
    case Key::C:
        std::cerr << "(" << MV::camera().eye() << ") "
                  << "(" << MV::camera().center() << ") "
                  << "(" << MV::camera().up() << ")\n";
        break;

    case Key::S:
        if (modifiers()[KeyModifier::CONTROL]) {
            if (dialog.exec() == QDialog::Accepted) {
                auto fs = dialog.selectedFiles();
                CanvasWidget::screenShot(fs.first().toStdString());
            }
            // the dialog stealed the focus, so we need to release the modifiers
            MV::setKeyModifiers({KeyModifier::NO_MODIFIER});
            setModifiers({KeyModifier::NO_MODIFIER});
        }
        break;

    case Key::A:
        MV::toggleAxisVisibility();
        break;

    default:
        MV::keyPress(key);
        bgfx::setViewTransform(
            viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
        break;
    }

    update();
}

void MinimalViewerWidget::onKeyRelease(Key::Enum key)
{
    if (isDirectionalLightVisible()) {
        if (!modifiers()[KeyModifier::CONTROL] ||
            !modifiers()[KeyModifier::SHIFT])
        {
            MV::setDirectionalLightVisibility(false);
        }
    }

    MV::setKeyModifiers(modifiers());
    update();
}

void MinimalViewerWidget::onMouseMove(double x, double y)
{
    MV::moveMouse(x, y);

    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());

    update();
}

void MinimalViewerWidget::onMousePress(MouseButton::Enum button)
{
    if (modifiers()[KeyModifier::CONTROL] && modifiers()[KeyModifier::SHIFT] &&
        button == MouseButton::LEFT)
    {
        MV::setDirectionalLightVisibility(true);
    }

    MV::pressMouse(button);
    update();
}

void MinimalViewerWidget::onMouseRelease(MouseButton::Enum button)
{
    if (isDirectionalLightVisible() && button == MouseButton::LEFT) {
        MV::setDirectionalLightVisibility(false);
    }

    MV::releaseMouse(button);
    update();
}

void MinimalViewerWidget::onMouseScroll(double dx, double dy)
{
    const int WHEEL_STEP = 120;
    float     notchY     = dy / float(WHEEL_STEP);

    MV::wheelMouse(notchY > 0);

    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());

    update();
}

} // namespace vcl::qbgf
