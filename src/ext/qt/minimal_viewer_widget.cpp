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

#include <vclib/ext/qt/minimal_viewer_widget.h>

#include <QMouseEvent>

#include <vclib/ext/qt/gui/screen_shot_dialog.h>
#include <vclib/ext/qt/input.h>

namespace vcl::qt {

MinimalViewerWidget::MinimalViewerWidget(
    std::shared_ptr<DrawableObjectVector> v,
    uint                                  width,
    uint                                  height,
    const std::string&                    windowTitle,
    QWidget*                              parent) :
        CanvasWidget(windowTitle, width, height, parent),
        vcl::MinimalViewer(v, width, height)
{
}

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

MinimalViewerWidget::MinimalViewerWidget(QWidget* parent) :
        MinimalViewerWidget("Minimal Viewer", 1024, 768, parent)
{
}

void MinimalViewerWidget::update()
{
    bgfx::setViewTransform(
        viewId(), MV::viewMatrix().data(), MV::projectionMatrix().data());
    CanvasWidget::update();
}

void MinimalViewerWidget::draw()
{
    MV::draw(viewId());
}

void MinimalViewerWidget::onResize(uint width, uint height)
{
    CanvasWidget::onResize(width, height);
    MV::onResize(width, height);
}

void MinimalViewerWidget::onKeyPress(Key::Enum key)
{
    switch (key) {
    case Key::S:
        if (modifiers()[KeyModifier::CONTROL]) {
            showScreenShotDialog();
        }
        break;

    default: MV::onKeyPress(key); break;
    }
}

void MinimalViewerWidget::showScreenShotDialog()
{
    qt::ScreenShotDialog* dialog = new qt::ScreenShotDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        auto fs = dialog->selectedFiles();
        CanvasWidget::screenShot(fs.first().toStdString());
    }
    // the dialog stealed the focus, so we need to release the modifiers
    MV::setKeyModifiers({KeyModifier::NO_MODIFIER});
    setModifiers({KeyModifier::NO_MODIFIER});
}

} // namespace vcl::qt
