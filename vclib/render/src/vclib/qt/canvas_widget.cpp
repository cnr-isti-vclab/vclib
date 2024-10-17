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

#include <QApplication>
#include <QMouseEvent>

#include <vclib/qt/canvas_widget.h>
#include <vclib/qt/input.h>

#include <vclib/qt/message_hider.h>

namespace vcl::qt {

CanvasWidget::CanvasWidget(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWidget*           parent) :
        EventManagerWidget(windowTitle, width, height, parent),
        Canvas(
            (void*) winId(),
            width * EventManagerWidget::pixelRatio(),
            height * EventManagerWidget::pixelRatio(),
            displayId())
{
}

CanvasWidget::CanvasWidget(uint width, uint height, QWidget* parent) :
        CanvasWidget("Canvas Widget", width, height, parent)
{
}

CanvasWidget::CanvasWidget(QWidget* parent) :
        CanvasWidget("Canvas Widget", 1024, 768, parent)
{
}

CanvasWidget::~CanvasWidget()
{
}

void CanvasWidget::draw()
{
}

#if defined(VCLIB_RENDER_BACKEND_BGFX)
void CanvasWidget::update()
{
    //frame();
    EventManagerWidget::update();
}
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
void CanvasWidget::initializeGL()
{
    Canvas::init(width(), height());
}
#endif

#if defined(VCLIB_RENDER_BACKEND_BGFX)
bool CanvasWidget::event(QEvent* event)
{
    // if (event->type() == QEvent::UpdateRequest) {
    //     frame();
    //     return true;
    // }
    return EventManagerWidget::event(event);
}
#endif

#if defined(VCLIB_RENDER_BACKEND_BGFX)
void CanvasWidget::paintEvent(QPaintEvent* event)
{
    frame();
    QWidget::paintEvent(event);
}
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
void CanvasWidget::paintGL()
{
    frame();
}
#endif

} // namespace vcl::qt
