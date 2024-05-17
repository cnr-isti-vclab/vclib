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

#ifndef VCL_EXT_QT_BGFX_CANVAS_WIDGET_H
#define VCL_EXT_QT_BGFX_CANVAS_WIDGET_H

#include <QVBoxLayout>
#include <QWidget>

#include <vclib/ext/bgfx/canvas.h>
#include <vclib/gui/event_manager_interface.h>

namespace vcl::qbgf {

class CanvasWidget :
        public QWidget,
        public vcl::bgf::Canvas,
        public virtual vcl::EventManagerInterface
{
    using Canvas = vcl::bgf::Canvas;

public:
    explicit CanvasWidget(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        QWidget*           parent = nullptr);

    explicit CanvasWidget(
        uint     width  = 1024,
        uint     height = 768,
        QWidget* parent = nullptr);

    explicit CanvasWidget(QWidget* parent);

    virtual ~CanvasWidget();

    void update() override;

protected:
    virtual void draw() override;

private:
    bool event(QEvent* event) override;

    void paintEvent(QPaintEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

    void paint();

    static double pixelRatio();
};

} // namespace vcl::qbgf

#endif // VCL_EXT_QT_BGFX_CANVAS_WIDGET_H
