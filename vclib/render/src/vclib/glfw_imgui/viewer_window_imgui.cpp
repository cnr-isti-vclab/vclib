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

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <imgui_impl_opengl2.h>
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
#include <vclib/bgfx_imgui/imgui_impl_bgfx.h>
#endif
#include <imgui_impl_glfw.h>
#include <vclib/glfw_imgui/viewer_window_imgui.h>

namespace vcl::glfw {

using Base = ViewerWindow;

ViewerWindowImgui::ViewerWindowImgui(
    const std::shared_ptr<DrawableObjectVector>& v,
    const std::string&                           windowTitle,
    uint                                         width,
    uint                                         height,
    void*) :
        Base(v, windowTitle, width, height)
{
}

ViewerWindowImgui::ViewerWindowImgui(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    void*) :
        Base(windowTitle, width, height)
{
}

ViewerWindowImgui::ViewerWindowImgui(void* parent) :
        Base(parent)
{
}

void ViewerWindowImgui::show()
{
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
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL2_Init();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
    ImGui_ImplGlfw_InitForOther(mWindow, true);
    ImGui_ImplBgfx_Init();
#endif

    // main loop
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();
        // slow down rendering if window is minimized
        if (glfwGetWindowAttrib(mWindow, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        frame();

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
        // swap buffers
        glfwSwapBuffers(mWindow);
#endif
    }

    // cleanup
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    ImGui_ImplOpenGL2_Shutdown();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
    ImGui_ImplBgfx_Shutdown();
#endif
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ViewerWindowImgui::frame()
{
    // imgui frame
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    ImGui_ImplOpenGL2_NewFrame();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
    ImGui_ImplBgfx_NewFrame();
#endif
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // actual drawing
    Base::frame();

    // imgui rendering
    ImGui::Render();
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
    ImGui_ImplBgfx_RenderDrawData(ImGui::GetDrawData());
#endif
}

void ViewerWindowImgui::glfwKeyCallback(
    GLFWwindow* win,
    int         key,
    int         scancode,
    int         action,
    int         mods)
{
    const auto& io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard)
        Base::glfwKeyCallback(win, key, scancode, action, mods);
}

void ViewerWindowImgui::glfwMouseButtonCallback(
    GLFWwindow* win,
    int         button,
    int         action,
    int         mods)
{
    const auto& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
        Base::glfwMouseButtonCallback(win, button, action, mods);
}

void ViewerWindowImgui::glfwCursorPosCallback(
    GLFWwindow*,
    double xpos,
    double ypos)
{
    const auto& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
        Base::glfwCursorPosCallback(nullptr, xpos, ypos);
}

void ViewerWindowImgui::glfwScrollCallback(
    GLFWwindow*,
    double xoffset,
    double yoffset)
{
    const auto& io = ImGui::GetIO();
    if (!io.WantCaptureMouse)
        Base::glfwScrollCallback(nullptr, xoffset, yoffset);
}

} // namespace vcl::glfw
