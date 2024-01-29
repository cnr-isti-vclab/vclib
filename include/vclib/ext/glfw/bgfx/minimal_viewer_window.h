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

#ifndef VCL_EXT_GLFW_BGFX_MINIMAL_VIEWER_WINDOW_H
#define VCL_EXT_GLFW_BGFX_MINIMAL_VIEWER_WINDOW_H

#include <vclib/ext/bgfx/minimal_viewer.h>

#include <vclib/ext/glfw/gui/input.h>

#include "canvas_window.h"

namespace vcl::bglfwx {

class MinimalViewerWindow :
        public vcl::bglfwx::CanvasWindow,
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
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count) :
            CanvasWindow(width, height, renderType),
            MinimalViewer(v, width, height)
    {
        // register callbacks
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto* self = static_cast<MinimalViewerWindow*>(glfwGetWindowUserPointer(window));
            self->glfwKeyCallback(window, key, scancode, action, mods);
        });
    }

    MinimalViewerWindow(
        uint                     width      = 1024,
        uint                     height     = 768,
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count) :
            MinimalViewerWindow(
                std::make_shared<DrawableObjectVector>(),
                width,
                height,
                renderType)
    {
    }

    MinimalViewerWindow(
        bgfx::RendererType::Enum renderType) :
            MinimalViewerWindow(
                std::make_shared<DrawableObjectVector>(),
                1024,
                768,
                renderType)
    {
    }

    ~MinimalViewerWindow() override = default;

    void draw(uint viewID) override
    {
        MV::draw(viewID);
    }

    void onResize(unsigned int width, unsigned int height) override
    {
        MV::resizeViewer(width, height);
    }

private:
    // callbacks
    void glfwKeyCallback(GLFWwindow*, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS) {
            MV::keyPress(glfw::fromKeyGLFW(key));
        }
    }
};

} // namespace vcl::bglfwx

#endif // VCL_EXT_GLFW_BGFX_MINIMAL_VIEWER_WINDOW_H
