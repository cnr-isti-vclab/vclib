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

#include <vclib/math/min_max.h>
#include <vclib/space/box.h>

#include <vclib/gui/desktop_trackball.h>
#include <vclib/render/drawable_object_vector.h>

#include <vclib/ext/qt/bgfx/canvas_window.h>
#include <vclib/ext/qt/gui/input.h>

namespace vcl::qbgf {

class MinimalViewerWindow :
        public vcl::qbgf::CanvasWindow,
        public vcl::DesktopTrackBall<float>
{
    // this Viewer does not normally own this drawList
    std::shared_ptr<DrawableObjectVector> drawList;

protected:
    using DTB = vcl::DesktopTrackBall<float>;

public:
    MinimalViewerWindow(
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count,
        QWindow*                 parent     = nullptr);

    MinimalViewerWindow(
        std::shared_ptr<DrawableObjectVector> v,
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count,
        QWindow*                 parent     = nullptr);

    virtual ~MinimalViewerWindow() {};

    void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v);

    std::shared_ptr<const DrawableObjectVector> drawableObjectVector() const;

    void fitScene();

    void draw() override;

    using CanvasWindow::height;
    using CanvasWindow::width;

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
