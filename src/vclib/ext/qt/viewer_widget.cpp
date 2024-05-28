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

#include <vclib/ext/qt/viewer_widget.h>

#include <QMouseEvent>

#include <vclib/ext/qt/gui/screen_shot_dialog.h>
#include <vclib/ext/qt/input.h>

namespace vcl::qt {

ViewerWidget::ViewerWidget(
    std::shared_ptr<DrawableObjectVector> v,
    uint                                  width,
    uint                                  height,
    const std::string&                    windowTitle,
    QWidget*                              parent) :
        EventManagerWidget(windowTitle, width, height, parent),
        ViewerCanvas((void*) winId(), v, width, height)
{
}

ViewerWidget::ViewerWidget(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWidget*           parent) :
        ViewerWidget(
            std::make_shared<DrawableObjectVector>(),
            width,
            height,
            windowTitle,
            parent)
{
}

ViewerWidget::ViewerWidget(QWidget* parent) :
        ViewerWidget("Viewer", 1024, 768, parent)
{
}

#if defined(VCLIB_RENDER_ENGINE_BGFX)
void ViewerWidget::update()
{
    frame();
    EventManagerWidget::update();
}
#endif

void ViewerWidget::onKeyPress(Key::Enum key)
{
    switch (key) {
    case Key::S:
        if (modifiers()[KeyModifier::CONTROL]) {
            showScreenShotDialog();
        }
        break;

    default: ViewerCanvas::onKeyPress(key); break;
    }
}

#if defined(VCLIB_RENDER_ENGINE_BGFX)
bool ViewerWidget::event(QEvent* event)
{
    if (event->type() == QEvent::UpdateRequest) {
        frame();
        return true;
    }
    return EventManagerWidget::event(event);
}
#endif

void ViewerWidget::paintEvent(QPaintEvent* event)
{
    frame();
    QWidget::paintEvent(event);
}

void ViewerWidget::showScreenShotDialog()
{
    qt::ScreenShotDialog* dialog = new qt::ScreenShotDialog(this);
    if (dialog->exec() == QDialog::Accepted) {
        auto fs = dialog->selectedFiles();
        ViewerCanvas::screenShot(fs.first().toStdString());
    }
    // the dialog stealed the focus, so we need to release the modifiers
    ViewerCanvas::setKeyModifiers({KeyModifier::NO_MODIFIER});
    setModifiers({KeyModifier::NO_MODIFIER});
}

} // namespace vcl::qt
