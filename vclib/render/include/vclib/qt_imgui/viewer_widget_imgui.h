/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_EXT_VIEWER_WIDGET_IMGUI_H
#define VCL_EXT_VIEWER_WIDGET_IMGUI_H

#include <vclib/qt/viewer_widget.h>

namespace vcl::qt {

class ViewerWidgetImgui : public ViewerWidget
{
public:
    ViewerWidgetImgui(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width       = 1024,
        uint                                         height      = 768,
        const std::string&                           windowTitle = "",
        QWidget*                                     parent      = nullptr);

    ViewerWidgetImgui(
        const std::string& windowTitle = "Minimal Viewer",
        uint               width       = 1024,
        uint               height      = 768,
        QWidget*           parent      = nullptr);

    ViewerWidgetImgui(QWidget* parent);

    ~ViewerWidgetImgui();

#if defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void initializeGL() override;
#endif

protected:
    virtual void initImGui();
    virtual void shutdownImGui();

#if defined(VCLIB_RENDER_BACKEND_BGFX)
    void paintEvent(QPaintEvent* event) override;
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
    void paintGL() override;
#endif
};

} // namespace vcl::qt

#endif // VCL_EXT_VIEWER_WIDGET_IMGUI_H