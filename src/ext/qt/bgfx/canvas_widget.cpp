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

#include <vclib/ext/qt/bgfx/canvas_widget.h>
#include <vclib/ext/qt/gui/input.h>

#include <vclib/ext/qt/message_hider.h>

namespace vcl::qbgf {

CanvasWidget::CanvasWidget(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWidget*           parent) :
        QWidget(parent)
{
    setGeometry(100, 100, width, height);
    setWindowTitle(windowTitle.c_str());

    setAttribute(Qt::WA_PaintOnScreen); // do not remove - needed on macos and x
    // PaintOnScreen is bugged - prints unuseful warning messages
    // we will hide it:
    vcl::qt::MessageHider::activate();

    Canvas::init((void*) winId(), width * pixelRatio(), height * pixelRatio());
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

void CanvasWidget::update()
{
    paint();
    QWidget::update();
}

bool CanvasWidget::event(QEvent* event)
{
    if (event->type() == QEvent::UpdateRequest) {
        paint();
        return true;
    }
    return QWidget::event(event);
}

void CanvasWidget::paintEvent(QPaintEvent* event)
{
    paint();
    QWidget::paintEvent(event);
}

void CanvasWidget::resizeEvent(QResizeEvent* event)
{
    Canvas::resize(width() * pixelRatio(), height() * pixelRatio());
    QWidget::resizeEvent(event);
    onResize(width() * pixelRatio(), height() * pixelRatio());
}

void CanvasWidget::keyPressEvent(QKeyEvent* event)
{
    setModifiers(vcl::qt::fromQt(event->modifiers()));

    onKeyPress(vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
    QWidget::keyPressEvent(event);
}

void CanvasWidget::keyReleaseEvent(QKeyEvent* event)
{
    setModifiers(vcl::qt::fromQt(event->modifiers()));

    onKeyRelease(vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
    QWidget::keyReleaseEvent(event);
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    QWidget::mouseMoveEvent(event);
}

void CanvasWidget::mousePressEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    onMousePress(vcl::qt::fromQt(event->button()));
    QWidget::mousePressEvent(event);
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    onMouseRelease(vcl::qt::fromQt(event->button()));
    QWidget::mouseReleaseEvent(event);
}

void CanvasWidget::wheelEvent(QWheelEvent* event)
{
    onMouseScroll(event->angleDelta().x(), event->angleDelta().y());
    QWidget::wheelEvent(event);
}

void CanvasWidget::paint()
{
    Canvas::frame();
}

double CanvasWidget::pixelRatio()
{
    auto app = qobject_cast<QGuiApplication*>(QCoreApplication::instance());
    return app->devicePixelRatio();
}

} // namespace vcl::qbgf
