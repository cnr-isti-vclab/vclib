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


#include <vclib/bgfx/imgui/imgui_impl_bgfx.h>
#include <imgui_impl_glfw.h>
#include <vclib/glfw/viewer_window_imgui.h>

namespace vcl::glfw {

ViewerWindowImgui::ViewerWindowImgui(
    const std::shared_ptr<DrawableObjectVector>& v,
    const std::string&                           windowTitle,
    uint                                         width,
    uint                                         height,
    void*) :
        ViewerWindow(v, windowTitle, width, height)
{
}

ViewerWindowImgui::ViewerWindowImgui(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    void*) :
        vcl::glfw::ViewerWindow(windowTitle, width, height)
{
}

ViewerWindowImgui::ViewerWindowImgui(void* parent) :
        ViewerWindow(parent)
{
}

void ViewerWindowImgui::show()
{
    // TODO: OpenGL 2 version

    // setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // setup ImGui style
    ImGui::StyleColorsDark();

    // setup platform/renderer backends (GLFW and ImGui)
    
    ImGui_ImplGlfw_InitForOther(mWindow, true);
    ImGui_ImplBgfx_Init();

    // main loop
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();
        // slow down rendering if window is minimized
        if (glfwGetWindowAttrib(mWindow, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        frame();
    }

    // cleanup
    ImGui_ImplBgfx_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ViewerWindowImgui::draw()
{
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    ViewerCanvas::draw();
    glfwSwapBuffers(mWindow); // TODO: check
#else // BGFX
    // ImGui frame
    ImGui_ImplBgfx_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ImGui demo window
    ImGui::ShowDemoWindow();

    // actual drawing
    ViewerCanvas::draw();

    // Rendering
    ImGui::Render();
    ImGui_ImplBgfx_RenderDrawData(ImGui::GetDrawData());
#endif
}

using Base = ViewerWindow;

void ViewerWindowImgui::glfwKeyCallback(
    GLFWwindow* win,
    int key,
    int scancode,
    int action,
    int mods)
    {
        const auto & io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard)
            Base::glfwKeyCallback(win, key, scancode, action, mods);
    }

void ViewerWindowImgui::glfwMouseButtonCallback(
    GLFWwindow* win,
    int         button,
    int         action,
    int         mods)
    {
        const auto & io = ImGui::GetIO();
        if (!io.WantCaptureMouse)
            Base::glfwMouseButtonCallback(win, button, action, mods);
    }

void ViewerWindowImgui::glfwCursorPosCallback(GLFWwindow*, double xpos, double ypos)
{
    const auto & io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
        Base::glfwCursorPosCallback(nullptr, xpos, ypos);
}

void ViewerWindowImgui::glfwScrollCallback(
    GLFWwindow*,
    double xoffset,
    double yoffset)
{   
    const auto & io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
        Base::glfwScrollCallback(nullptr, xoffset, yoffset);
}

} // namespace vcl::glfw
