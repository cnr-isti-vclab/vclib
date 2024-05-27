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

#include <QGuiApplication>
#include <QMouseEvent>

#include <vclib/ext/qt/event_manager_widget.h>

#include <vclib/ext/qt/input.h>
#include <vclib/ext/qt/message_hider.h>

namespace vcl::qt {

EventManagerWidget::EventManagerWidget(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWidget*           parent) :
#if defined(VCLIB_RENDER_ENGINE_BGFX)
        QWidget(parent)
#elif defined(VCLIB_RENDER_ENGINE_OPENGL2)
        QOpenGLWidget(parent)
#endif
{
#ifdef VCLIB_RENDER_ENGINE_BGFX
    setAttribute(Qt::WA_PaintOnScreen); // do not remove - needed on macos and x
    // PaintOnScreen is bugged - prints unuseful warning messages
    // we will hide it:
    vcl::qt::MessageHider::activate();
#endif

    setGeometry(100, 100, width, height);
    setWindowTitle(windowTitle.c_str());
}

void EventManagerWidget::resizeEvent(QResizeEvent* event)
{
#if defined(VCLIB_RENDER_ENGINE_BGFX)
    QWidget::resizeEvent(event);
#elif defined(VCLIB_RENDER_ENGINE_OPENGL2)
    QOpenGLWidget::resizeEvent(event);
#endif
    onResize(width() * pixelRatio(), height() * pixelRatio());
}

void EventManagerWidget::keyPressEvent(QKeyEvent* event)
{
    setModifiers(vcl::qt::fromQt(event->modifiers()));

    onKeyPress(vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
    QWidget::keyPressEvent(event);
}

void EventManagerWidget::keyReleaseEvent(QKeyEvent* event)
{
    setModifiers(vcl::qt::fromQt(event->modifiers()));

    onKeyRelease(vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
    QWidget::keyReleaseEvent(event);
}

void EventManagerWidget::mouseMoveEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    QWidget::mouseMoveEvent(event);
}

void EventManagerWidget::mousePressEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    onMousePress(vcl::qt::fromQt(event->button()));
    QWidget::mousePressEvent(event);
}

void EventManagerWidget::mouseReleaseEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    onMouseRelease(vcl::qt::fromQt(event->button()));
    QWidget::mouseReleaseEvent(event);
}

void EventManagerWidget::wheelEvent(QWheelEvent* event)
{
    onMouseScroll(event->angleDelta().x(), event->angleDelta().y());
    QWidget::wheelEvent(event);
}

double EventManagerWidget::pixelRatio()
{
    auto app = qobject_cast<QGuiApplication*>(QCoreApplication::instance());
    return app->devicePixelRatio();
}

} // namespace vcl::qt
