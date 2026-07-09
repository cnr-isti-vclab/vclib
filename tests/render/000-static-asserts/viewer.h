// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VIEWER_H
#define VIEWER_H

#include <vclib/render/concepts/mesh_viewer.h>

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

}

#endif // VIEWER_H
