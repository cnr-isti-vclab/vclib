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

#ifndef VCL_EXT_GLFW_BGFX_CANVAS_WINDOW_H
#define VCL_EXT_GLFW_BGFX_CANVAS_WINDOW_H

#include <vclib/ext/bgfx/canvas.h>

#include <vclib/gui/event_manager_interface.h>

#include <GLFW/glfw3.h>

namespace vcl::bglfwx {

class CanvasWindow : public vcl::bgf::Canvas, public virtual vcl::EventManagerInterface
{
    using Canvas = vcl::bgf::Canvas;

    std::string mTitle;

protected:
    GLFWwindow* mWindow = nullptr;

public:
    CanvasWindow(
        const std::string& windowTitle,
        uint               width  = 1024,
        uint               height = 768);

    CanvasWindow(uint width = 1024, uint height = 768);

    virtual ~CanvasWindow();

    const std::string& windowTitle() const;

    void setWindowTitle(const std::string& title);

    uint width() const;

    uint height() const;

    void show();

protected:
    virtual void draw() override {}

    virtual void onResize(unsigned int w, unsigned int h) {}

private:
    void setCallbacks();

    // callbacks
    void glfwWindowSizeCallback(GLFWwindow*, int width, int height);

    // callbacks
    void glfwKeyCallback(GLFWwindow*, int key, int, int action, int mods);

    void glfwMouseButtonCallback(GLFWwindow*, int button, int action, int mods);

    void glfwCursorPosCallback(GLFWwindow*, double xpos, double ypos);

    void glfwScrollCallback(GLFWwindow*, double xoffset, double yoffset);
};

} // namespace vcl::bglfwx

#endif // VCL_EXT_GLFW_BGFX_CANVAS_WINDOW_H
