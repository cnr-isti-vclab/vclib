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

#include "application.h"
#include "concepts/mesh_viewer.h"

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

template<vcl::MeshConcept... MeshTypes>
void showOnMeshViewer(
    int                     argc,
    char**                  argv,
    MeshViewerConcept auto& viewer,
    MeshTypes&&... meshes)
{
    (viewer.pushMesh(std::forward<MeshTypes>(meshes)), ...);

    viewer.fitScene();

    viewer.showMaximized();
}

template<vcl::MeshConcept MeshTypes>
void showOnMeshViewer(
    int                      argc,
    char**                   argv,
    MeshViewerConcept auto&  viewer,
    std::vector<MeshTypes>&& meshes,
    bool                     pbrMode  = false,
    const std::string&       panorama = "")
{
    for (auto&& mesh : meshes)
        viewer.pushMesh(std::move(mesh));

#ifdef VCLIB_RENDER_BACKEND_BGFX
    auto sts = viewer.pbrSettings();

    if (!panorama.empty()) {
        viewer.setPanorama(panorama);
        sts.imageBasedLighting       = true;
        sts.renderBackgroundPanorama = true;
    }
    sts.pbrMode = pbrMode;
    viewer.setPbrSettings(sts);
#endif

    viewer.fitScene();

    viewer.showMaximized();
}

template<vcl::MeshConcept... MeshTypes>
int showOnMeshViewer(int argc, char** argv, MeshTypes&&... meshes)
{
    vcl::Application app(argc, argv);

    vcl::MeshViewer viewer;

    showOnMeshViewer(argc, argv, viewer, std::forward<MeshTypes>(meshes)...);

    return app.exec();
}

template<vcl::MeshConcept MeshTypes>
int showOnMeshViewer(
    int                      argc,
    char**                   argv,
    std::vector<MeshTypes>&& meshes,
    bool                     pbrMode  = false,
    const std::string&       panorama = "")
{
    vcl::Application app(argc, argv);

    vcl::MeshViewer viewer;

    showOnMeshViewer(
        argc, argv, viewer, std::move(meshes), pbrMode, panorama);

    return app.exec();
}

} // namespace vcl

#endif // VCL_RENDER_MESH_VIEWER_H
