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

#include <vclib/qt_imgui/imgui_impl_qt.h>
#include <vclib/qt_imgui/viewer_widget_imgui.h>

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <imgui_impl_opengl2.h>
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
#include <vclib/bgfx_imgui/imgui_impl_bgfx.h>
#endif

#include <imgui.h>

namespace vcl::qt {

ViewerWidgetImgui::ViewerWidgetImgui(
    const std::shared_ptr<DrawableObjectVector>& v,
    uint                                         width,
    uint                                         height,
    const std::string&                           windowTitle,
    QWidget* parent) : ViewerWidget(v, width, height, windowTitle, parent)
{
#ifdef VCLIB_RENDER_BACKEND_BGFX
    initImGui();
#endif
}

ViewerWidgetImgui::ViewerWidgetImgui(
    const std::string& windowTitle,
    uint               width,
    uint               height,
    QWidget* parent) : ViewerWidget(windowTitle, width, height, parent)
{
#ifdef VCLIB_RENDER_BACKEND_BGFX
    initImGui();
#endif
}

ViewerWidgetImgui::ViewerWidgetImgui(QWidget* parent) : ViewerWidget(parent)
{
#ifdef VCLIB_RENDER_BACKEND_BGFX
    initImGui();
#endif
}

ViewerWidgetImgui::~ViewerWidgetImgui()
{
    shutdownImGui();
}

#if defined(VCLIB_RENDER_BACKEND_OPENGL2)
void ViewerWidgetImgui::initializeGL()
{
    ViewerWidget::initializeGL();
    initImGui();
}
#endif

void ViewerWidgetImgui::initImGui()
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

    // setup platform/renderer backends (Qt and ImGui)
    ImGui_ImplQt_Init(this);
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    ImGui_ImplOpenGL2_Init();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
    ImGui_ImplBgfx_Init();
#endif
}

void ViewerWidgetImgui::shutdownImGui()
{
#ifdef VCLIB_RENDER_BACKEND_OPENGL2
    ImGui_ImplOpenGL2_Shutdown();
#elif defined(VCLIB_RENDER_BACKEND_BGFX)
    ImGui_ImplBgfx_Shutdown();
#endif
    ImGui_ImplQt_Shutdown();
    ImGui::DestroyContext();
}

#ifdef VCLIB_RENDER_BACKEND_BGFX
void ViewerWidgetImgui::paintEvent(QPaintEvent* event)
{
    // imgui frame
    ImGui_ImplBgfx_NewFrame();
    ImGui_ImplQt_NewFrame();
    ImGui::NewFrame();

    ViewerWidget::paintEvent(event);

    ImGui::Render();
    ImGui_ImplBgfx_RenderDrawData(ImGui::GetDrawData());

    // request widget update continuosly (to keep the imgui ui updated)
    this->update();
}
#elif defined(VCLIB_RENDER_BACKEND_OPENGL2)
void ViewerWidgetImgui::paintGL()
{
    // imgui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplQt_NewFrame();
    ImGui::NewFrame();

    ViewerWidget::paintGL();

    ImGui::Render();
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

    // request widget update continuosly (to keep the imgui ui updated)
    this->update();
}
#endif

} // namespace vcl::qt
