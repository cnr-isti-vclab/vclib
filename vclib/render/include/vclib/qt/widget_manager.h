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

#if defined(VCLIB_RENDER_BACKEND_BGFX)
#include <QWidget>
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
#include <QOpenGLWidget>
#endif

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
        QWidget*           parent = nullptr);

    ~WidgetManager() = default;

    void* displayId() const;

    void update();

    QPaintEngine* paintEngine() const override;

protected:
#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void resizeEvent(QResizeEvent* event) override;
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void resizeGL(int w, int h) override;
#endif

    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseDoubleClickEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

    static double pixelRatio();
};

} // namespace vcl::qt

#endif // VCL_QT_WIDGET_MANAGER_H
