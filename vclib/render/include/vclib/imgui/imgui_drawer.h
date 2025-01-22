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

#ifndef VCL_IMGUI_IMGUI_DRAWER_H
#define VCL_IMGUI_IMGUI_DRAWER_H

#include <vclib/render/drawers/blocker_event_drawer.h>
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
#ifdef VCLIB_WITH_QT
#include <vclib/qt_imgui/imgui_impl_qt.h>
#endif

namespace vcl::imgui {

template<typename DerivedRenderApp>
class ImGuiDrawer : public BlockerEventDrawer<DerivedRenderApp>
{
protected:
    using DRA = DerivedRenderApp;

public:
    ImGuiDrawer()
    {
        static_assert(
            DRA::WINDOW_MANAGER_ID == WindowManagerId::GLFW_WINDOW ||
                DRA::WINDOW_MANAGER_ID == WindowManagerId::QT_WIDGET,
            "ImGuiDrawer supports only GLFW or Qt window managers.");
    }

    ImGuiDrawer(uint, uint) : ImGuiDrawer() {}

    ~ImGuiDrawer()
    {
        // cleanup
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
        ImGui_ImplOpenGL2_Shutdown();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
        ImGui_ImplBgfx_Shutdown();
#endif  // VCLIB_RENDER_BACKEND_*
#ifdef VCLIB_WITH_GLFW
        if constexpr (DRA::WINDOW_MANAGER_ID == WindowManagerId::GLFW_WINDOW) {
            ImGui_ImplGlfw_Shutdown();
        }
#endif // VCLIB_WITH_GLFW
#ifdef VCLIB_WITH_QT
        if constexpr (
            DRA::WINDOW_MANAGER_ID == WindowManagerId::QT_WIDGET) {
            ImGui_ImplQt_Shutdown();
        }
#endif // VCLIB_WITH
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

#ifdef VCLIB_WITH_GLFW
        if constexpr (DRA::WINDOW_MANAGER_ID == WindowManagerId::GLFW_WINDOW) {
            GLFWwindow* mWindow =
                reinterpret_cast<GLFWwindow*>(DRA::DRW::windowPtr(derived()));
            // setup platform/RenderApp backends (GLFW and ImGui)
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
            ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
            ImGui_ImplOpenGL2_Init();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
            ImGui_ImplGlfw_InitForOther(mWindow, true);
            ImGui_ImplBgfx_Init();
#endif // VCLIB_RENDER_BACKEND_*
        }
# endif // VCLIB_WITH_GLFW
#ifdef VCLIB_WITH_QT
        if constexpr (
            DRA::WINDOW_MANAGER_ID == WindowManagerId::QT_WIDGET) {
            QWidget* mWindow =
                reinterpret_cast<QWidget*>(DRA::DRW::windowPtr(derived()));
            // setup platform/RenderApp backends (Qt and ImGui)
            ImGui_ImplQt_Init(mWindow);
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
            ImGui_ImplOpenGL2_Init();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
            ImGui_ImplBgfx_Init();
#endif // VCLIB_RENDER_BACKEND_*
        }
#endif // VCLIB_WITH_QT
    }

    virtual void onDraw(uint)
    {
        // imgui frame
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
        ImGui_ImplOpenGL2_NewFrame();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
        ImGui_ImplBgfx_NewFrame();
#endif // VCLIB_RENDER_BACKEND_*
#ifdef VCLIB_WITH_GLFW
        if constexpr (DRA::WINDOW_MANAGER_ID == WindowManagerId::GLFW_WINDOW) {
            ImGui_ImplGlfw_NewFrame();
        }
#endif // VCLIB_WITH_GLFW
#ifdef VCLIB_WITH_QT
        if constexpr (
            DRA::WINDOW_MANAGER_ID == WindowManagerId::QT_WIDGET) {
            ImGui_ImplQt_NewFrame();
        }
#endif // VCLIB_WITH_QT
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
#endif // VCLIB_RENDER_BACKEND_*
        derived()->update();
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
    bool isWindowMinimized() const { return derived()->isMinimized(); }

private:
    auto* derived() { return static_cast<DRA*>(this); }

    const auto* derived() const { return static_cast<const DRA*>(this); }

    bool wantCapture(bool mouse = true)
    {
        const auto& io = ImGui::GetIO();
        return mouse ? io.WantCaptureMouse : io.WantCaptureKeyboard;
    }
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_IMGUI_DRAWER_H
