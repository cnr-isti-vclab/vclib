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

#ifndef IMGUI_DRAWER_H
#define IMGUI_DRAWER_H

#include <vclib/render/drawers/event_drawer.h>
#include <vclib/render/window_managers.h>

// Include the render backand imgui implementation
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <imgui_impl_opengl2.h>
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
#include <vclib/bgfx_imgui/imgui_impl_bgfx.h>
#endif

// Include the event backend imgui implementations
#ifdef VCLIB_WITH_GLFW
#include <imgui_impl_glfw.h>
#endif

namespace vcl::imgui {

template<typename DerivedRenderer>
class ImGuiDrawer : public EventDrawer<DerivedRenderer, true>
{
protected:
    using DRT = DerivedRenderer;

public:
    ImGuiDrawer()
    {
        static_assert(
            DRT::WINDOW_MANAGER_ID == WindowManagerId::GLFW_WINDOW,
            "ImGuiDrawer supports only GLFW window manager.");
    }

    ImGuiDrawer(uint, uint) : ImGuiDrawer() {}

    ~ImGuiDrawer()
    {
        // cleanup
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
        ImGui_ImplOpenGL2_Shutdown();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
        ImGui_ImplBgfx_Shutdown();
#endif
        if constexpr (DRT::WINDOW_MANAGER_ID == WindowManagerId::GLFW_WINDOW) {
            ImGui_ImplGlfw_Shutdown();
        }
        ImGui::DestroyContext();
    }

    virtual void onInit(uint viewId)
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

        if constexpr (DRT::WINDOW_MANAGER_ID == WindowManagerId::GLFW_WINDOW) {
            GLFWwindow* mWindow =
                reinterpret_cast<GLFWwindow*>(DRT::DRW::windowPtr(derived()));
            // setup platform/renderer backends (GLFW and ImGui)
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
            ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
            ImGui_ImplOpenGL2_Init();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
            ImGui_ImplGlfw_InitForOther(mWindow, true);
            ImGui_ImplBgfx_Init();
#endif
        }
    }

    virtual void onDraw(uint)
    {
        // imgui frame
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
        ImGui_ImplOpenGL2_NewFrame();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
        ImGui_ImplBgfx_NewFrame();
#endif
        if constexpr (
            DRT::WINDOW_MANAGER_ID == WindowManagerId::GLFW_WINDOW) {
            ImGui_ImplGlfw_NewFrame();
        }
        ImGui::NewFrame();
    }

    virtual void onPostDraw()
    {
        // imgui rendering
        ImGui::Render();
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
        ImGui_ImplBgfx_RenderDrawData(ImGui::GetDrawData());
#endif
    }

    virtual bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers)
    {
        return wantCapture(false);
    }

    virtual bool onKeyRelease(Key::Enum key, const KeyModifiers& modifiers)
    {
        return wantCapture(false);
    }

    virtual bool onMouseMove(double x, double y, const KeyModifiers& modifiers)
    {
        return wantCapture();
    }

    virtual bool onMousePress(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return wantCapture();
    }

    virtual bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return wantCapture();
    }

    virtual bool onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return wantCapture();
    }

    virtual bool onMouseScroll(
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return wantCapture();
    }

protected:
    bool isWindowMinimized() const
    {
        return derived()->isMinimized();
    }

private:
    auto* derived() { return static_cast<DRT*>(this); }

    const auto* derived() const { return static_cast<const DRT*>(this); }

    bool wantCapture(bool mouse = true)
    {
        const auto& io = ImGui::GetIO();
        return mouse ? io.WantCaptureMouse : io.WantCaptureKeyboard;
    }
};

} // namespace vcl::imgui

#endif // IMGUI_DRAWER_H
