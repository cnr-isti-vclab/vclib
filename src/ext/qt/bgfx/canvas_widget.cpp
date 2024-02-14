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
#include <QMouseEvent>

#include <vclib/ext/qt/bgfx/canvas_widget.h>
#include <vclib/ext/qt/gui/input.h>

#ifdef __APPLE__
#include <vclib/ext/qt/message_hider.h>
#endif // __APPLE__

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

#ifdef __APPLE__
    setAttribute(Qt::WA_PaintOnScreen); // needed on macOS - do not remove
    // however this is bugged - prints unuseful warning messages
    // we will hide it:
    vcl::qt::MessageHider::activate();
#endif // __APPLE__

    vcl::bgf::Canvas::init((void*) winId(), width, height);
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

void CanvasWidget::onResize(unsigned int, unsigned int)
{
}

void CanvasWidget::update()
{
    paint();
    QWidget::update();
}

void CanvasWidget::keyPressEvent(QKeyEvent* event)
{
    onKeyPress(
        vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()),
        vcl::qt::fromQt(event->modifiers()));
    QWidget::keyPressEvent(event);
}

void CanvasWidget::keyReleaseEvent(QKeyEvent* event)
{
    onKeyRelease(
        vcl::qt::fromQt((Qt::Key) event->key(), event->modifiers()),
        vcl::qt::fromQt(event->modifiers()));
    QWidget::keyReleaseEvent(event);
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
    Canvas::resize(width(), height());
    QWidget::resizeEvent(event);
    onResize(width(), height());
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* event)
{
    onMouseMove(event->pos().x(), event->pos().y());
    QWidget::mouseMoveEvent(event);
}

void CanvasWidget::mousePressEvent(QMouseEvent* event)
{
    onMouseMove(event->pos().x(), event->pos().y());
    onMousePress(vcl::qt::fromQt(event->button()));
    QWidget::mousePressEvent(event);
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* event)
{
    onMouseMove(event->pos().x(), event->pos().y());
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

} // namespace vcl::qbgf
