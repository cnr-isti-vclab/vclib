// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_MESH_VIEWER_H
#define VCL_RENDER_MESH_VIEWER_H

#ifdef VCLIB_WITH_QT
#include <vclib/qt/mesh_viewer.h>
#elif VCLIB_WITH_GLFW && VCLIB_WITH_IMGUI
#include <vclib/imgui/mesh_viewer.h>
#endif

namespace vcl {

/**
 * @brief The MeshViewer class is an alias to the default mesh viewer class
 * used in VCLib, providing a complete GUI for manipulating and rendering meshes.
 *
 * Depending on the available window managers and GUI libraries, the MeshViewer
 * class is aliased to one of the following classes:
 * - If Qt is available, it is aliased to `vcl::qt::MeshViewer`.
 * - Else if GLFW is available and ImGui is enabled, it is aliased to
 *   `vcl::imgui::MeshViewer`.
 * - Else, a compilation error is triggered if no GUI toolkit is available
 *   and a MeshViewer instance is created.
 */

#ifdef VCLIB_WITH_QT

using MeshViewer = qt::MeshViewer;

#elif VCLIB_WITH_GLFW && VCLIB_WITH_IMGUI

using MeshViewer = imgui::MeshViewer;

#else // No Qt and no ImGui

// define an unusable MeshViewer to trigger a compilation error
struct MeshViewer
{
    MeshViewer()
    {
        static_assert(
            false,
            "No GUI library available for MeshViewer type definition. "
            "Please allow usage of Qt or GLFW+ImGui.");
    }
};

#endif // VCLIB_WITH_QT

} // namespace vcl

#endif // VCL_RENDER_MESH_VIEWER_H
