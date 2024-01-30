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

#ifndef VCL_EXT_QT_BGFX_MINIMAL_VIEWER_WINDOW_H
#define VCL_EXT_QT_BGFX_MINIMAL_VIEWER_WINDOW_H

#include <QMouseEvent>

#include <vclib/ext/bgfx/minimal_viewer.h>
#include <vclib/ext/qt/gui/input.h>

#include "canvas_window.h"

namespace vcl::qbgf {

class MinimalViewerWindow :
        public vcl::qbgf::CanvasWindow,
        public vcl::bgf::MinimalViewer
{
protected:
    using MV = vcl::bgf::MinimalViewer;

public:
    using CanvasWindow::height;
    using CanvasWindow::width;

    MinimalViewerWindow(
        std::shared_ptr<DrawableObjectVector> v,
        uint                                  width  = 1024,
        uint                                  height = 768,
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count,
        QWindow*                 parent     = nullptr);

    MinimalViewerWindow(
        uint                     width      = 1024,
        uint                     height     = 768,
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count,
        QWindow*                 parent     = nullptr);

    MinimalViewerWindow(
        bgfx::RendererType::Enum renderType,
        QWindow*                 parent = nullptr);

    MinimalViewerWindow(QWindow* parent);

    virtual ~MinimalViewerWindow() = default;

    void draw(uint viewID) override;

    void onResize(unsigned int width, unsigned int height) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;
};

} // namespace vcl::qbgf

#endif // VCL_EXT_QT_BGFX_MINIMAL_VIEWER_WINDOW_H
