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

#include "input.h"

//#include <vclib/render/interfaces/event_manager_i.h>
#include <vclib/render/concepts/renderer.h>
#include <vclib/space/core/point.h>

#if defined(VCLIB_RENDER_BACKEND_BGFX)
#include <QWidget>
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
#include <QOpenGLWidget>
#endif

#include <QGuiApplication>
#include <QMouseEvent>

namespace vcl::qt {

template<typename DerivedRenderer>
class WidgetManager :
#if defined(VCLIB_RENDER_BACKEND_BGFX)
        public QWidget
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
        public QOpenGLWidget
#endif
{
    using DRT = DerivedRenderer;
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    using Base = QWidget;
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    using Base = QOpenGLWidget;
#endif

public:
    WidgetManager(QWidget* parent = nullptr) : Base(parent)
    {
        static_assert(
            RendererConcept<DRT>,
            "The DerivedRenderer must satisfy the RendererConcept.");
#if defined(VCLIB_RENDER_BACKEND_BGFX)
        setAttribute(
            Qt::WA_PaintOnScreen); // do not remove - needed on macos and x
        setAttribute(Qt::WA_DontCreateNativeAncestors);
        setAttribute(Qt::WA_NativeWindow);
#endif
    }

    explicit WidgetManager(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        QWidget*           parent = nullptr) : WidgetManager(parent)
    {
        setGeometry(100, 100, width, height);
        setWindowTitle(windowTitle.c_str());
    }

    ~WidgetManager() = default;

    const std::string& windowTitle() const
    {
        return QWidget::windowTitle().toStdString();
    }

    void setWindowTitle(const std::string& title)
    {
        QWidget::setWindowTitle(title.c_str());
    }

    Point2f dpiScale() const { return Point2f(pixelRatio(), pixelRatio()); }

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

    QPaintEngine* paintEngine() const override
    {
        return nullptr;
    }

protected:
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void resizeEvent(QResizeEvent* event) override
    {
        Base::resizeEvent(event);
        DRT::WM::resize(
            derived(), width() * pixelRatio(), height() * pixelRatio());
    }
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void resizeGL(int w, int h) override
    {
        Base::resizeGL(w, h);
        DRT::WM::resize(derived(), w * pixelRatio(), h * pixelRatio());
    }
#endif

#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void showEvent(QShowEvent* event) override
    {
        Base::showEvent(event);
        DRT::WM::init(derived());
    }
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void initializeGL() override
    {
        DRT::WM::init(derived());
    }
#endif


    void keyPressEvent(QKeyEvent* event) override
    {
        DRT::WM::setModifiers(derived(), vcl::qt::fromQt(event->modifiers()));

        DRT::WM::keyPress(
            derived(),
            vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
        Base::keyPressEvent(event);
        update();
    }

    void keyReleaseEvent(QKeyEvent* event) override
    {
        DRT::WM::setModifiers(derived(), vcl::qt::fromQt(event->modifiers()));

        DRT::WM::keyRelease(
            derived(),
            vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
        Base::keyReleaseEvent(event);
        update();
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        DRT::WM::mouseMove(
            derived(),
            event->pos().x() * pixelRatio(),
            event->pos().y() * pixelRatio());
        Base::mouseMoveEvent(event);
        update();
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        DRT::WM::mousePress(
            derived(),
            vcl::qt::fromQt(event->button()),
            event->pos().x() * pixelRatio(),
            event->pos().y() * pixelRatio());
        Base::mousePressEvent(event);
        update();
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        DRT::WM::mouseRelease(
            derived(),
            vcl::qt::fromQt(event->button()),
            event->pos().x() * pixelRatio(),
            event->pos().y() * pixelRatio());
        Base::mouseReleaseEvent(event);
        update();
    }

    void mouseDoubleClickEvent(QMouseEvent* event) override
    {
        DRT::WM::mouseDoubleClick(
            derived(),
            vcl::qt::fromQt(event->button()),
            event->pos().x() * pixelRatio(),
            event->pos().y() * pixelRatio());
        Base::mouseDoubleClickEvent(event);
        update();
    }

    void wheelEvent(QWheelEvent* event) override
    {
        // FIXME: this is not correct, define a proper equivalence
        if (!event->pixelDelta().isNull())
            DRT::WM::mouseScroll(
                derived(), event->pixelDelta().x(), event->pixelDelta().y());
        else
            DRT::WM::mouseScroll(
                derived(), event->angleDelta().x(), event->angleDelta().y());

        Base::wheelEvent(event);
        update();
    }

    static double pixelRatio()
    {
        auto app = qobject_cast<QGuiApplication*>(QCoreApplication::instance());
        return app->devicePixelRatio();
    }

private:
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void paintEvent(QPaintEvent* event) override
    {
        DRT::WM::paint(derived());
        QWidget::paintEvent(event);
    }
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void paintGL() override { DRT::WM::paint(derived()); }
#endif

    auto* derived() { return static_cast<DRT*>(this); }

    const auto* derived() const { return static_cast<const DRT*>(this); }
};

} // namespace vcl::qt

#endif // VCL_QT_WIDGET_MANAGER_H
