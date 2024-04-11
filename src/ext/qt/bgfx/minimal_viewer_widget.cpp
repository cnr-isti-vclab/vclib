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
    MV::updateDrawableTrackball();
}

void MinimalViewerWidget::onKeyPress(Key::Enum key)
{
    qt::ScreenShotDialog dialog(this);
    MV::setKeyModifiers(modifiers());

    if (modifiers()[KeyModifier::CONTROL] && modifiers()[KeyModifier::SHIFT]) {
        MV::setDirectionalLightVisibility(true);
    }

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

    case Key::A: MV::toggleAxisVisibility(); break;

    case Key::T: MV::toggleTrackballVisibility(); break;

    default:
        MV::keyPress(key);
        bgfx::setViewTransform(
            viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
        break;
    }

    MV::updateDirectionalLight();
    MV::updateDrawableTrackball();
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
    MV::updateDirectionalLight();
    MV::updateDrawableTrackball();

    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());

    update();
}

void MinimalViewerWidget::onMousePress(MouseButton::Enum button)
{
    MV::pressMouse(button);
    MV::updateDrawableTrackball();
    update();
}

void MinimalViewerWidget::onMouseRelease(MouseButton::Enum button)
{
    MV::releaseMouse(button);
    MV::updateDrawableTrackball();
    update();
}

void MinimalViewerWidget::onMouseScroll(double dx, double dy)
{
    // const int WHEEL_STEP = 120;
    // float     notchY     = dy / float(WHEEL_STEP);

    MV::scroll(dx, dy);

    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());

    update();
}

} // namespace vcl::qbgf
