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

#include <vclib/glfw/canvas_window.h>

namespace vcl::glfw {

CanvasWindow::CanvasWindow(
    const std::string& windowTitle,
    uint               width,
    uint               height) :
        EventManagerWindow(windowTitle, width, height),
        Canvas(winId(), width, height, displayId())
{
}

CanvasWindow::CanvasWindow(uint width, uint height) :
        vcl::glfw::CanvasWindow("GLFW Canvas", width, height)
{
}

CanvasWindow::~CanvasWindow()
{
}

void CanvasWindow::show()
{
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();
        frame();
    }
}

} // namespace vcl::glfw
