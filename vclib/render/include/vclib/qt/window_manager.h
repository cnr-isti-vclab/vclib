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

#ifndef VCL_QT_WINDOW_MANAGER_H
#define VCL_QT_WINDOW_MANAGER_H

#include "input.h"

#include <vclib/render/concepts/render_app.h>
#include <vclib/render/window_managers.h>
#include <vclib/space/core/point.h>

#include <QGuiApplication>
#include <QMouseEvent>
#include <QWindow>

namespace vcl::qt {

template<typename DerivedRenderApp>
class WindowManager : public QWindow
{
    std::string mTitle;

public:
    using ParentType = QWindow;

    static const uint WINDOW_MANAGER_ID = WindowManagerId::QT_WINDOW;

    explicit WindowManager(QWindow* parent = nullptr) : QWindow(parent)
    {
        static_assert(
            RenderAppConcept<DerivedRenderApp>,
            "The DerivedRenderApp must satisfy the RenderAppConcept.");
        setSurfaceType(QSurface::OpenGLSurface);
    }

    explicit WindowManager(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        QWindow*           parent = nullptr) : WindowManager(parent)
    {
        setGeometry(100, 100, width, height);
        setTitle(QString::fromStdString(windowTitle));
    }

    ~WindowManager() override = default;

    bool isMinimized() const
    {
        return QWindow::visibility() == QWindow::Minimized;
    }

    const std::string& windowTitle() const { return mTitle; }

    void setWindowTitle(const std::string& title)
    {
        mTitle = title;
        setTitle(QString::fromStdString(title));
    }

    Point2f dpiScale() const
    {
        auto* screen = this->screen();
        return Point2f(
            screen ? screen->devicePixelRatio() : 1.0,
            screen ? screen->devicePixelRatio() : 1.0);
    }

    void* displayId() const
    {
        void* displayID = nullptr;
#ifdef Q_OS_LINUX
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

    void* windowPtr() { return reinterpret_cast<void*>(winId()); }

    void update()
    {
        QWindow::requestUpdate();
    }

protected:
    bool event(QEvent *event)
    {
        if (event->type() == QEvent::UpdateRequest) {
            DerivedRenderApp::WM::paint(derived());
            return true;
        }
        return QWindow::event(event);
    }

    void resizeEvent(QResizeEvent* event) override
    {
        DerivedRenderApp::WM::resize(
            derived(), width() * dpiScale().x(), height() * dpiScale().y());
        QWindow::resizeEvent(event);
        update();
    }

    void exposeEvent(QExposeEvent* event) override
    {
        if (isExposed()) {
            DerivedRenderApp::WM::init(derived());
        }
        QWindow::exposeEvent(event);
        update();
    }

    void keyPressEvent(QKeyEvent* event) override
    {
        DerivedRenderApp::WM::setModifiers(
            derived(), vcl::qt::fromQt(event->modifiers()));

        DerivedRenderApp::WM::keyPress(
            derived(),
            vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
        QWindow::keyPressEvent(event);
        update();
    }

    void keyReleaseEvent(QKeyEvent* event) override
    {
        DerivedRenderApp::WM::setModifiers(
            derived(), vcl::qt::fromQt(event->modifiers()));

        DerivedRenderApp::WM::keyRelease(
            derived(),
            vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()));
        QWindow::keyReleaseEvent(event);
        update();
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        DerivedRenderApp::WM::mouseMove(
            derived(),
            event->pos().x() * dpiScale().x(),
            event->pos().y() * dpiScale().y());
        QWindow::mouseMoveEvent(event);
        update();
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        DerivedRenderApp::WM::mousePress(
            derived(),
            vcl::qt::fromQt(event->button()),
            event->pos().x() * dpiScale().x(),
            event->pos().y() * dpiScale().y());
        QWindow::mousePressEvent(event);
        update();
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        DerivedRenderApp::WM::mouseRelease(
            derived(),
            vcl::qt::fromQt(event->button()),
            event->pos().x() * dpiScale().x(),
            event->pos().y() * dpiScale().y());
        QWindow::mouseReleaseEvent(event);
        update();
    }

    void mouseDoubleClickEvent(QMouseEvent* event) override
    {
        DerivedRenderApp::WM::mouseDoubleClick(
            derived(),
            vcl::qt::fromQt(event->button()),
            event->pos().x() * dpiScale().x(),
            event->pos().y() * dpiScale().y());
        QWindow::mouseDoubleClickEvent(event);
        update();
    }

    void wheelEvent(QWheelEvent* event) override
    {
        if (!event->pixelDelta().isNull())
            DerivedRenderApp::WM::mouseScroll(
                derived(), event->pixelDelta().x(), event->pixelDelta().y());
        else
            DerivedRenderApp::WM::mouseScroll(
                derived(), event->angleDelta().x(), event->angleDelta().y());

        QWindow::wheelEvent(event);
        update();
    }

private:
    void paintEvent(QPaintEvent* event) override
    {
        DerivedRenderApp::WM::paint(derived());
        QWindow::paintEvent(event);
    }

    auto* derived() { return static_cast<DerivedRenderApp*>(this); }

    const auto* derived() const
    {
        return static_cast<const DerivedRenderApp*>(this);
    }
};

} // namespace vcl::qt

#endif // VCL_QT_WINDOW_MANAGER_H
