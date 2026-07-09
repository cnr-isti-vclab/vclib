// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VIEWER_H
#define VIEWER_H

#include <vclib/render/concepts/viewer.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/mesh_viewer.h>
#endif

#ifdef VCLIB_WITH_GLFW
#ifdef VCLIB_WITH_IMGUI
#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/mesh_viewer_imgui_drawer.h>
#endif
#include <vclib/glfw/viewer_window.h>
#endif

void viewerStaticAsserts()
{
    using namespace vcl;

#ifdef VCLIB_WITH_QT
    static_assert(
        ViewerConcept<qt::MeshViewer>,
        "qt::MeshViewer does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const qt::MeshViewer>,
        "const qt::MeshViewer does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<qt::MeshViewer&>,
        "qt::MeshViewer& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const qt::MeshViewer&>,
        "const qt::MeshViewer& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<qt::MeshViewer&&>,
        "qt::MeshViewer&& does not satisfy the ViewerConcept");
#endif

#ifdef VCLIB_WITH_GLFW

#ifdef VCLIB_WITH_IMGUI
    using ImguiViewer = RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::imgui::MeshViewerDrawerImgui>;

    static_assert(
        ViewerConcept<ImguiViewer>,
        "ImguiViewer does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const ImguiViewer>,
        "const ImguiViewer does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<ImguiViewer&>,
        "ImguiViewer& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const ImguiViewer&>,
        "const ImguiViewer& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<ImguiViewer&&>,
        "ImguiViewer&& does not satisfy the ViewerConcept");
#endif

    static_assert(
        ViewerConcept<glfw::ViewerWindow>,
        "glfw::ViewerWindow does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const glfw::ViewerWindow>,
        "const glfw::ViewerWindow does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<glfw::ViewerWindow&>,
        "glfw::ViewerWindow& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const glfw::ViewerWindow&>,
        "const glfw::ViewerWindow& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<glfw::ViewerWindow&&>,
        "glfw::ViewerWindow&& does not satisfy the ViewerConcept");
#endif

}

#endif // VIEWER_H
