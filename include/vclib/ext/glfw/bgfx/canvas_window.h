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

#include <iostream>

#include <vclib/ext/bgfx/canvas.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace vcl::bglfwx {

namespace detail {

inline void glfwErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW error: " << error << ": " << description<< std::endl;
}

} // namespace detail

class CanvasWindow : public vcl::bgf::Canvas
{
    using Canvas = vcl::bgf::Canvas;

    GLFWwindow* window = nullptr;

    uint w = 1024, h = 768;

public:
    explicit CanvasWindow(
        bgfx::RendererType::Enum renderType = bgfx::RendererType::Count)
    {
        glfwSetErrorCallback(detail::glfwErrorCallback);
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(w, h, "VCL", nullptr, nullptr);
        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        void* ndt = nullptr;
        void* nwh = nullptr;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
        ndt = glfwGetX11Display();
        nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
        nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
        nwh = glfwGetWin32Window(window);
#endif
        Canvas::init(nwh, w, h, ndt, renderType);
    }

    virtual ~CanvasWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void show()
    {
        run();
    }

    virtual void draw(uint viewID) override {}

    virtual void onResize(unsigned int w, unsigned int h) {}

private:
    void run()
    {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            frame();
        }
    }
};

} // namespace vcl::bglfwx

#endif // VCL_EXT_GLFW_BGFX_CANVAS_WINDOW_H
