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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <vclib/ext/qt/bgfx/canvas_window.h>

namespace vcl::qbgf {

CanvasWindow::CanvasWindow(
    bgfx::RendererType::Enum renderType,
    QWindow*                 parent) :
        QWindow(parent)
{
    setGeometry(100, 100, 1024, 768);

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

    vcl::bgf::Canvas::init(
        (void*) winId(), width(), height(), displayID, renderType);
}

CanvasWindow::~CanvasWindow()
{
}

void CanvasWindow::draw()
{
}

void CanvasWindow::onResize(unsigned int w, unsigned int h)
{
}

void CanvasWindow::update()
{
    QWindow::requestUpdate();
}

bool CanvasWindow::event(QEvent* event)
{
    if (event->type() == QEvent::UpdateRequest) {
        paint();
        return true;
    }
    return QWindow::event(event);
}

void CanvasWindow::paintEvent(QPaintEvent* event)
{
    paint();
    QWindow::paintEvent(event);
}

void CanvasWindow::resizeEvent(QResizeEvent* event)
{
    vcl::bgf::Canvas::resize(width(), height());
    QWindow::resizeEvent(event);
    onResize(width(), height());
}

void CanvasWindow::paint()
{
    draw();
    vcl::bgf::Canvas::frame();
}

} // namespace vcl::qbgf
