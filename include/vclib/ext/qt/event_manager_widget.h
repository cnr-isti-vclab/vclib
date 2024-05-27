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

#ifndef VCL_EXT_QT_EVENT_MANAGER_WIDGET_H
#define VCL_EXT_QT_EVENT_MANAGER_WIDGET_H

#if defined(VCLIB_RENDER_ENGINE_BGFX)
#include <QWidget>
#elif defined(VCLIB_RENDER_ENGINE_OPENGL2)
#include <QOpenGLWidget>
#endif

#include <vclib/render/interfaces/event_manager_i.h>

namespace vcl::qt {

class EventManagerWidget :
#if defined(VCLIB_RENDER_ENGINE_BGFX)
        public QWidget,
#elif defined(VCLIB_RENDER_ENGINE_OPENGL2)
        public QOpenGLWidget,
#endif
        public virtual vcl::EventManagerI
{
public:
    explicit EventManagerWidget(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768,
        QWidget*           parent = nullptr);

    virtual ~EventManagerWidget() = default;

protected:
    void resizeEvent(QResizeEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

    static double pixelRatio();
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_EVENT_MANAGER_WIDGET_H
