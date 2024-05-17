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

#ifndef VCL_EXT_GLFW_BGFX_MINIMAL_VIEWER_WINDOW_H
#define VCL_EXT_GLFW_BGFX_MINIMAL_VIEWER_WINDOW_H

#include <vclib/ext/bgfx/minimal_viewer.h>

#include "canvas_window.h"

namespace vcl::bglfwx {

class MinimalViewerWindow : public CanvasWindow, public vcl::bgf::MinimalViewer
{
protected:
    using MV = vcl::bgf::MinimalViewer;

public:
    using CanvasWindow::height;
    using CanvasWindow::width;

    MinimalViewerWindow(
        std::shared_ptr<DrawableObjectVector> v,
        const std::string&                    windowTitle = "Minimal Viewer",
        uint                                  width       = 1024,
        uint                                  height      = 768,
        void* parent = nullptr);

    MinimalViewerWindow(
        const std::string& windowTitle = "Minimal Viewer",
        uint               width       = 1024,
        uint               height      = 768,
        void*              parent      = nullptr);

    MinimalViewerWindow(void* parent);

    ~MinimalViewerWindow() override = default;

    void draw() override;

    void onResize(unsigned int width, unsigned int height) override;

    void onKeyPress(Key::Enum key) override;

    void onKeyRelease(Key::Enum key) override;

    void onMouseMove(double x, double y) override;

    void onMousePress(MouseButton::Enum button) override;

    void onMouseRelease(MouseButton::Enum button) override;

    void onMouseScroll(double dx, double dy) override;
};

} // namespace vcl::bglfwx

#endif // VCL_EXT_GLFW_BGFX_MINIMAL_VIEWER_WINDOW_H
