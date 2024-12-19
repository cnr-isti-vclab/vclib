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

#ifndef VCL_QT_WIDGET_MANAGER_H
#define VCL_QT_WIDGET_MANAGER_H

//#include <vclib/render/interfaces/event_manager_i.h>
#include <vclib/qt/input.h>

#if defined(VCLIB_RENDER_BACKEND_BGFX)
#include <QWidget>
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
#include <QOpenGLWidget>
#endif

#include <QGuiApplication>
#include <QMouseEvent>

namespace vcl::qt {

class WidgetManager :
#if defined(VCLIB_RENDER_BACKEND_BGFX)
        public QWidget
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
        public QOpenGLWidget
#endif
{
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    using Base = QWidget;
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    using Base = QOpenGLWidget;
#endif

public:
    explicit WidgetManager(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        QWidget*           parent = nullptr) :
#if defined(VCLIB_RENDER_BACKEND_BGFX)
            QWidget(parent)
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
            QOpenGLWidget(parent)
#endif
    {
#if defined(VCLIB_RENDER_BACKEND_BGFX)
        setAttribute(
            Qt::WA_PaintOnScreen); // do not remove - needed on macos and x
        setAttribute(Qt::WA_DontCreateNativeAncestors);
        setAttribute(Qt::WA_NativeWindow);
#endif

        setGeometry(100, 100, width, height);
        setWindowTitle(windowTitle.c_str());
    }

    ~WidgetManager() = default;

    void* displayId() const
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

    void update()
    {
        Base::update();
    }

    QPaintEngine* paintEngine() const override
    {
        return nullptr;
    }

protected:
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void resizeEvent(QResizeEvent* event) override
    {
        Base::resizeEvent(event);
        // onResize(width() * pixelRatio(), height() * pixelRatio()); TODO
    }
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void resizeGL(int w, int h) override
    {
        Base::resizeGL(w, h);
        // onResize(w * pixelRatio(), h * pixelRatio()); TODO
    }
#endif

    void keyPressEvent(QKeyEvent* event) override
    {
        // setModifiers(vcl::qt::fromQt(event->modifiers())); TODO

        // onKeyPress(vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers())); TODO
        Base::keyPressEvent(event);
    }

    void keyReleaseEvent(QKeyEvent* event) override
    {
        // setModifiers(vcl::qt::fromQt(event->modifiers())); TODO

        // onKeyRelease(vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers())); TODO
        Base::keyReleaseEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        // onMouseMove(
        //     event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio()); TODO
        Base::mouseMoveEvent(event);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        // onMouseMove(
        //     event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio()); TODO
        // onMousePress(vcl::qt::fromQt(event->button())); TODO
        Base::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        // onMouseMove(
        //     event->pos().x() * pixelRatio(), event->pos().y() * pixelRatio()); TODO
        // onMouseRelease(vcl::qt::fromQt(event->button())); TODO
        Base::mouseReleaseEvent(event);
    }

    void mouseDoubleClickEvent(QMouseEvent* event) override
    {
        // onMouseDoubleClick(
        //     vcl::qt::fromQt(event->button()),
        //     event->pos().x() * pixelRatio(),
        //     event->pos().y() * pixelRatio()); TODO
        Base::mouseDoubleClickEvent(event);
    }

    void wheelEvent(QWheelEvent* event) override
    {
        // FIXME: this is not correct, define a proper equivalence
        if (!event->pixelDelta().isNull())
            ; // onMouseScroll(event->pixelDelta().x(), event->pixelDelta().y()); TODO
        else
            ; // onMouseScroll(event->angleDelta().x(), event->angleDelta().y()); TODO

        Base::wheelEvent(event);
    }

    static double pixelRatio()
    {
        auto app = qobject_cast<QGuiApplication*>(QCoreApplication::instance());
        return app->devicePixelRatio();
    }
};

} // namespace vcl::qt

#endif // VCL_QT_WIDGET_MANAGER_H
