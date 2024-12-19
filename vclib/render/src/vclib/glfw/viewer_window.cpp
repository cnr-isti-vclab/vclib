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

#include <vclib/glfw/viewer_window.h>

namespace vcl::glfw {

ViewerWindow::ViewerWindow(
    const std::shared_ptr<DrawableObjectVector>& v,
    const std::string&                           windowTitle,
    uint                                         width,
    uint                                         height,
    void*) :
        EventManagerWindow(windowTitle, width, height),
        ViewerCanvas(
            winId(),
            v,
            width * contentScaleX(),
            height * contentScaleY(),
            displayId())
{
}

ViewerWindow::ViewerWindow(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    void*) :
        ViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            windowTitle,
            width,
            height)
{
}

ViewerWindow::ViewerWindow(void*) :
        ViewerWindow(
            std::make_shared<DrawableObjectVector>(),
            "Viewer",
            1024,
            768)
{
}

void ViewerWindow::show()
{
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();
        frame();

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
        // swap buffers
        glfwSwapBuffers(mWindow);
#endif
    }
}

void ViewerWindow::onKeyPress(Key::Enum key)
{
    switch (key) {
    case Key::S:
        if (modifiers()[KeyModifier::CONTROL]) {
            if (!ViewerCanvas::screenshot("screenshot.png"))
                std::cerr << "Failed to save screenshot" << std::endl;
        }
        break;

    default: ViewerCanvas::onKeyPress(key); break;
    }
}

void ViewerWindow::onResize(unsigned int width, unsigned int height)
{
    ViewerCanvas::onResize(width, height);
    frame();
    #ifdef VCLIB_RENDER_BACKEND_OPENGL2
    glfwSwapBuffers(mWindow);
    #endif
}

} // namespace vcl::glfw
