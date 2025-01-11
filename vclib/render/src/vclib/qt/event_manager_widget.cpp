/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/qt/event_manager_widget.h>

#include <vclib/qt/input.h>
#include <vclib/qt/message_hider.h>

#include <QGuiApplication>
#include <QMouseEvent>

namespace vcl::qt {

EventManagerWidget::EventManagerWidget(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWidget*           parent) :
#if defined(VCLIB_RENDER_BACKEND_BGFX)
        QWidget(parent)
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
        QOpenGLWidget(parent)
#endif
{
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    setAttribute(Qt::WA_PaintOnScreen); // do not remove - needed on macos and x
    // PaintOnScreen is bugged - prints unuseful warning messages
    // we will hide it:
    // vcl::qt::MessageHider::activate(); // TODO: check again if this is needed
    setAttribute(Qt::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WA_NativeWindow);
#endif

    setGeometry(100, 100, width, height);
    setWindowTitle(windowTitle.c_str());
}

void EventManagerWidget::update()
{
    Base::update();
}

QPaintEngine* EventManagerWidget::paintEngine() const
{
    return nullptr;
}

#if defined(VCLIB_RENDER_BACKEND_BGFX)
void EventManagerWidget::resizeEvent(QResizeEvent* event)
{
    Base::resizeEvent(event);
    onResize(width() * pixelRatio(), height() * pixelRatio());
}
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
void EventManagerWidget::resizeGL(int w, int h)
{
    Base::resizeGL(w, h);
    onResize(w * pixelRatio(), h * pixelRatio());
}
#endif

void EventManagerWidget::keyPressEvent(QKeyEvent* event)
{
    setModifiers(vcl::qt::fromQt(event->modifiers()));

    onKeyPress(vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
    Base::keyPressEvent(event);
}

void EventManagerWidget::keyReleaseEvent(QKeyEvent* event)
{
    setModifiers(vcl::qt::fromQt(event->modifiers()));

    onKeyRelease(vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
    Base::keyReleaseEvent(event);
}

void EventManagerWidget::mouseMoveEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    Base::mouseMoveEvent(event);
}

void EventManagerWidget::mousePressEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    onMousePress(vcl::qt::fromQt(event->button()));
    Base::mousePressEvent(event);
}

void EventManagerWidget::mouseReleaseEvent(QMouseEvent* event)
{
    onMouseMove(
        event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio());
    onMouseRelease(vcl::qt::fromQt(event->button()));
    Base::mouseReleaseEvent(event);
}

void EventManagerWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    onMouseDoubleClick(
        vcl::qt::fromQt(event->button()),
        event->pos().x() * pixelRatio(),
        event->pos().y() * pixelRatio());
    Base::mouseDoubleClickEvent(event);
}

void EventManagerWidget::wheelEvent(QWheelEvent* event)
{
    // FIXME: this is not correct, define a proper equivalence
    if (!event->pixelDelta().isNull())
        onMouseScroll(event->pixelDelta().x(), event->pixelDelta().y());
    else
        onMouseScroll(event->angleDelta().x(), event->angleDelta().y());

    Base::wheelEvent(event);
}

double EventManagerWidget::pixelRatio()
{
    auto app = qobject_cast<QGuiApplication*>(QCoreApplication::instance());
    return app->devicePixelRatio();
}

void* EventManagerWidget::displayId() const
{
    void* displayID = nullptr;
#ifdef Q_OS_LINUX
    /// THIS WORKS ONLY IF QT_QPA_PLATFORM = xcb
    QNativeInterface::QX11Application* x11AppInfo =
        qApp->nativeInterface<QNativeInterface::QX11Application>();
    if (x11AppInfo) {
        displayID = x11AppInfo->display();
    }
    else {
        QNativeInterface::QWaylandApplication* wayAppInfo =
            qApp->nativeInterface<QNativeInterface::QWaylandApplication>();
        if (wayAppInfo) {
            displayID = wayAppInfo->display();
        }
        else {
            exit(-1);
        }
    }
#endif
    return displayID;
}

} // namespace vcl::qt
