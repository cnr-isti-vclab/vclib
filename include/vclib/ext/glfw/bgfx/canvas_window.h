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

#ifndef VCL_EXT_GLFW_BGFX_CANVAS_WINDOW_H
#define VCL_EXT_GLFW_BGFX_CANVAS_WINDOW_H

#include <vclib/ext/bgfx/canvas.h>

#include <GLFW/glfw3.h>

namespace vcl::bglfwx {

class CanvasWindow : public vcl::bgf::Canvas
{
    using Canvas = vcl::bgf::Canvas;

    std::string title;

protected:
    GLFWwindow* window = nullptr;

public:
    CanvasWindow(const std::string& windowTitle, uint width = 1024, uint height = 768);

    CanvasWindow(uint width = 1024, uint height = 768);

    virtual ~CanvasWindow();

    const std::string& windowTitle() const;

    void setWindowTitle(const std::string& title);

    uint width() const;

    uint height() const;

    void show();

    virtual void draw(uint viewID) override {}

    virtual void onResize(unsigned int w, unsigned int h) {}

private:
    // callbacks
    void glfwWindowSizeCallback(GLFWwindow*, int width, int height);
};

} // namespace vcl::bglfwx

#endif // VCL_EXT_GLFW_BGFX_CANVAS_WINDOW_H
