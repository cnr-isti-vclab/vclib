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

#ifndef VCL_QT_WIDGET_MANAGER_H
#define VCL_QT_WIDGET_MANAGER_H

#include "input.h"

#include <vclib/render/concepts/render_app.h>
#include <vclib/render/window_managers.h>
#include <vclib/space/core/point.h>

#if defined(VCLIB_RENDER_BACKEND_BGFX)
#include <QWidget>
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
#include <QOpenGLWidget>
#endif

#include <QGuiApplication>
#include <QMouseEvent>

namespace vcl::qt {

template<typename DerivedRenderApp>
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
    /**
     * @brief The ParentType is the type of the parent class. It is used to
     * initialize the base class (if any). In the qt::WidgetManager, the parent
     * class is the QWidget class.
     */
    using ParentType = QWidget;

    /**
     * @brief The WINDOW_MANAGER_ID is the ID of the window manager. It is used
     * to identify the window manager implementation (if necessary) by the
     * DerivedRenderApp class.
     */
    static const uint WINDOW_MANAGER_ID = WindowManagerId::QT_WIDGET;

    WidgetManager(QWidget* parent = nullptr) : Base(parent)
    {
        static_assert(
            RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");
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

    QPaintEngine* paintEngine() const override { return nullptr; }

protected:
    void* windowPtr() { return reinterpret_cast<void*>(this); }

#ifdef Q_OS_MACOS
    // current pixel ratio
    // values > 0 are used to detect changes in pixel ratio
    double mCurrentPixelRatio = -1.0; 

    bool event(QEvent *event) override
    {
        if (event->type() == QEvent::DevicePixelRatioChange)
        {
            std::cerr << "DevicePixelRatioChange " << pixelRatio() << std::endl;
            std::cerr << "size " << size().width() << " x " << size().height() << std::endl;
            // save current ratio
            mCurrentPixelRatio = pixelRatio();
            // send update event
            this->update();
        }

        if (event->type() == QEvent::UpdateRequest)
        {
            std::cerr << "UpdateRequest " << pixelRatio() << std::endl;
            if (mCurrentPixelRatio > 0 && mCurrentPixelRatio != pixelRatio())
            {
                const double ratio = pixelRatio();
                
                // ratio changed
                std::cerr << "Ratio changed " << mCurrentPixelRatio << " -> "
                          << ratio << std::endl;

                // reset current ratio
                mCurrentPixelRatio = -1.0;

                // send resize event
                QResizeEvent resizeEvent(size(), size());
                auto app = qobject_cast<QGuiApplication*>(QCoreApplication::instance());
                app->sendEvent(this, &resizeEvent);
            }
        }

        return QWidget::event(event);
    }
#endif

#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void resizeEvent(QResizeEvent* event) override
    {
        std::cerr << "Resize: ratio " << pixelRatio() << std::endl;
        Base::resizeEvent(event);
        DerivedRenderApp::WM::resize(
            derived(), width() * pixelRatio(), height() * pixelRatio());
    }
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void resizeGL(int w, int h) override
    {
        Base::resizeGL(w, h);
        DerivedRenderApp::WM::resize(
            derived(), w * pixelRatio(), h * pixelRatio());
    }
#endif

#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void showEvent(QShowEvent* event) override
    {
        Base::showEvent(event);
        DerivedRenderApp::WM::init(derived());
    }
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void initializeGL() override { DerivedRenderApp::WM::init(derived()); }
#endif

    void keyPressEvent(QKeyEvent* event) override
    {
        DerivedRenderApp::WM::setModifiers(
            derived(), vcl::qt::fromQt(event->modifiers()));

        DerivedRenderApp::WM::keyPress(
            derived(),
            vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
        Base::keyPressEvent(event);
        update();
    }

    void keyReleaseEvent(QKeyEvent* event) override
    {
        DerivedRenderApp::WM::setModifiers(
            derived(), vcl::qt::fromQt(event->modifiers()));

        DerivedRenderApp::WM::keyRelease(
            derived(),
            vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
        Base::keyReleaseEvent(event);
        update();
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        DerivedRenderApp::WM::mouseMove(
            derived(),
            event->pos().x() * pixelRatio(),
            event->pos().y() * pixelRatio());
        Base::mouseMoveEvent(event);
        update();
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        // on right press print device pixel ratio
        if (event->button() == Qt::RightButton)
        {
            std::cerr << "DevicePixelRatio: " << pixelRatio() << std::endl;
        }

        DerivedRenderApp::WM::mousePress(
            derived(),
            vcl::qt::fromQt(event->button()),
            event->pos().x() * pixelRatio(),
            event->pos().y() * pixelRatio());
        Base::mousePressEvent(event);
        update();
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        DerivedRenderApp::WM::mouseRelease(
            derived(),
            vcl::qt::fromQt(event->button()),
            event->pos().x() * pixelRatio(),
            event->pos().y() * pixelRatio());
        Base::mouseReleaseEvent(event);
        update();
    }

    void mouseDoubleClickEvent(QMouseEvent* event) override
    {
        DerivedRenderApp::WM::mouseDoubleClick(
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
            DerivedRenderApp::WM::mouseScroll(
                derived(), event->pixelDelta().x(), event->pixelDelta().y());
        else
            DerivedRenderApp::WM::mouseScroll(
                derived(), event->angleDelta().x(), event->angleDelta().y());

        Base::wheelEvent(event);
        update();
    }

    double pixelRatio() const
    {
        auto * screen = this->screen();
        return double(screen ? screen->devicePixelRatio() : 1.0);
    }

private:
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void paintEvent(QPaintEvent* event) override
    {
        DerivedRenderApp::WM::paint(derived());
        QWidget::paintEvent(event);
    }
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void paintGL() override { DerivedRenderApp::WM::paint(derived()); }
#endif

    auto* derived() { return static_cast<DerivedRenderApp*>(this); }

    const auto* derived() const
    {
        return static_cast<const DerivedRenderApp*>(this);
    }
};

} // namespace vcl::qt

#endif // VCL_QT_WIDGET_MANAGER_H
