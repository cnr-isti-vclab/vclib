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

#ifndef VCL_EXT_QT_VIEWER_WIDGET_H
#define VCL_EXT_QT_VIEWER_WIDGET_H

#include "event_manager_widget.h"

#include <vclib/render/viewer_canvas.h>

namespace vcl::qt {

class ViewerWidget : public EventManagerWidget, public vcl::ViewerCanvas
{
public:
    ViewerWidget(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width       = 1024,
        uint                                         height      = 768,
        const std::string&                           windowTitle = "",
        QWidget*                                     parent      = nullptr);

    ViewerWidget(
        const std::string& windowTitle = "Minimal Viewer",
        uint               width       = 1024,
        uint               height      = 768,
        QWidget*           parent      = nullptr);

    ViewerWidget(QWidget* parent);

    virtual ~ViewerWidget() = default;

#if defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void initializeGL() override;
#endif

    void onKeyPress(Key::Enum key) override;

private:

#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void paintEvent(QPaintEvent* event) override;
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void paintGL() override;
#endif

    void showScreenShotDialog();
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_VIEWER_WIDGET_H
