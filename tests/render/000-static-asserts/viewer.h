// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VIEWER_H
#define VIEWER_H

#include <vclib/render/concepts/mesh_viewer.h>
#include <vclib/render/concepts/viewer.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/mesh_viewer.h>
#include <vclib/qt/viewer_widget.h>
#include <vclib/qt/viewer_window.h>
#endif

#ifdef VCLIB_WITH_GLFW
#include <vclib/glfw/viewer_window.h>
#ifdef VCLIB_WITH_IMGUI
#include <vclib/imgui/mesh_viewer.h>
#endif
#endif

void viewerStaticAsserts()
{
    using namespace vcl;

#ifdef VCLIB_WITH_QT
    static_assert(
        ViewerConcept<qt::ViewerWidget>,
        "qt::ViewerWidget does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const qt::ViewerWidget>,
        "const qt::ViewerWidget does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<qt::ViewerWidget&>,
        "qt::ViewerWidget& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const qt::ViewerWidget&>,
        "const qt::ViewerWidget& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<qt::ViewerWidget&&>,
        "qt::ViewerWidget&& does not satisfy the ViewerConcept");

    static_assert(
        ViewerConcept<qt::ViewerWindow>,
        "qt::ViewerWindow does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const qt::ViewerWindow>,
        "const qt::ViewerWindow does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<qt::ViewerWindow&>,
        "qt::ViewerWindow& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<const qt::ViewerWindow&>,
        "const qt::ViewerWindow& does not satisfy the ViewerConcept");
    static_assert(
        ViewerConcept<qt::ViewerWindow&&>,
        "qt::ViewerWindow&& does not satisfy the ViewerConcept");

    static_assert(
        MeshViewerConcept<qt::MeshViewer>,
        "qt::MeshViewer does not satisfy the MeshViewerConcept");
    static_assert(
        MeshViewerConcept<const qt::MeshViewer>,
        "const qt::MeshViewer does not satisfy the MeshViewerConcept");
    static_assert(
        MeshViewerConcept<qt::MeshViewer&>,
        "qt::MeshViewer& does not satisfy the MeshViewerConcept");
    static_assert(
        MeshViewerConcept<const qt::MeshViewer&>,
        "const qt::MeshViewer& does not satisfy the MeshViewerConcept");
    static_assert(
        MeshViewerConcept<qt::MeshViewer&&>,
        "qt::MeshViewer&& does not satisfy the MeshViewerConcept");
#endif

#ifdef VCLIB_WITH_GLFW
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

#ifdef VCLIB_WITH_IMGUI
    static_assert(
        MeshViewerConcept<imgui::MeshViewer>,
        "imgui::MeshViewer does not satisfy the MeshViewerConcept");
    static_assert(
        MeshViewerConcept<const imgui::MeshViewer>,
        "const imgui::MeshViewer does not satisfy the MeshViewerConcept");
    static_assert(
        MeshViewerConcept<imgui::MeshViewer&>,
        "imgui::MeshViewer& does not satisfy the MeshViewerConcept");
    static_assert(
        MeshViewerConcept<const imgui::MeshViewer&>,
        "const imgui::MeshViewer& does not satisfy the MeshViewerConcept");
    static_assert(
        MeshViewerConcept<imgui::MeshViewer&&>,
        "imgui::MeshViewer&& does not satisfy the MeshViewerConcept");
#endif
#endif
}

#endif // VIEWER_H
