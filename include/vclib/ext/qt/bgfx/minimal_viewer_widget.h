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

#ifndef VCL_EXT_QT_BGFX_MINIMAL_VIEWER_WIDGET_H
#define VCL_EXT_QT_BGFX_MINIMAL_VIEWER_WIDGET_H

#include "canvas_widget.h"
#include "minimal_viewer_window.h"

namespace vcl::qbgf {

#ifndef __APPLE__

class MinimalViewerWidget : public CanvasWidget, public vcl::bgf::MinimalViewer
{
protected:
    using MV = vcl::bgf::MinimalViewer;

public:
    using CanvasWidget::height;
    using CanvasWidget::width;

    MinimalViewerWidget(QWidget* parent = nullptr);

    MinimalViewerWidget(
        std::shared_ptr<DrawableObjectVector> v,
        QWidget*                              parent = nullptr);

    virtual ~MinimalViewerWidget() = default;

    void draw(uint viewID) override;

    void onResize(unsigned int width, unsigned int height) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;
};

#else // __APPLE__

class MinimalViewerWidget : public CanvasWidget<MinimalViewerWindow>
{
public:
    MinimalViewerWidget(QWidget* parent = nullptr) :
            CanvasWidget<MinimalViewerWindow>(parent)
    {
    }

    MinimalViewerWidget(
        std::shared_ptr<DrawableObjectVector> v,
        QWidget*                              parent = nullptr) :
            CanvasWidget<MinimalViewerWindow>(parent)
    {
        setDrawableObjectVector(v);
    }

    virtual ~MinimalViewerWidget() = default;

    void setDrawableObjectVector(std::shared_ptr<DrawableObjectVector> v)
    {
        canvasWindow->setDrawableObjectVector(v);
    }

    void fitScene() { canvasWindow->fitScene(); }
};

#endif // __APPLE__

} // namespace vcl::qbgf

#endif // VCL_EXT_QT_BGFX_MINIMAL_VIEWER_WIDGET_H
