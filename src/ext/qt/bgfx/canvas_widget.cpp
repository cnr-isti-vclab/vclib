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

#include <QApplication>

#include <vclib/ext/qt/bgfx/canvas_widget.h>

namespace vcl::qbgf {

#ifndef __APPLE__

CanvasWidget::CanvasWidget(bgfx::RendererType::Enum renderType, QWidget* parent)
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
#endif // Q_OS_LINUX

    vcl::bgf::Canvas::init(
        (void*) winId(), width(), height(), displayID, renderType);
}


CanvasWidget::~CanvasWidget()
{
}


void CanvasWidget::draw(uint viewID)
{
}

void CanvasWidget::onResize(unsigned int w, unsigned int h)
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
    vcl::bgf::Canvas::resize(width(), height());
    QWidget::resizeEvent(event);
    onResize(width(), height());
}

void CanvasWidget::paint()
{
    Canvas::frame();
}

#endif // __APPLE__

} // namespace vcl::qbgf
