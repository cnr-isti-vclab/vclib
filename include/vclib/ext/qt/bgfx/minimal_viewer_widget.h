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

#include <vclib/ext/bgfx/minimal_viewer.h>

namespace vcl::qbgf {

class MinimalViewerWidget : public CanvasWidget, public vcl::bgf::MinimalViewer
{
protected:
    using MV = vcl::bgf::MinimalViewer;

public:
    using CanvasWidget::height;
    using CanvasWidget::width;

    MinimalViewerWidget(
        const std::string& windowTitle = "Minimal Viewer",
        uint               width       = 1024,
        uint               height      = 768,
        QWidget*           parent      = nullptr);

    MinimalViewerWidget(
        std::shared_ptr<DrawableObjectVector> v,
        uint                                  width       = 1024,
        uint                                  height      = 768,
        const std::string&                    windowTitle = "",
        QWidget*                              parent      = nullptr);

    MinimalViewerWidget(QWidget* parent);

    virtual ~MinimalViewerWidget() = default;

    void draw() override;

    void onResize(unsigned int width, unsigned int height) override;

    void onKeyPress(Key::Enum key, KeyModifiers modifiers) override;

    void onKeyRelease(Key::Enum key, KeyModifiers modifiers) override;

    void onMouseMove(double x, double y) override;

    void onMousePress(MouseButton::Enum button) override;

    void onMouseRelease(MouseButton::Enum button) override;

    void onMouseScroll(double dx, double dy) override;
};

} // namespace vcl::qbgf

#endif // VCL_EXT_QT_BGFX_MINIMAL_VIEWER_WIDGET_H
