// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H
#define VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H

#include <vclib/mesh/requirements.h>
#include <vclib/render/concepts/drawable_object.h>

#ifdef VCLIB_WITH_IMGUI
#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/mesh_viewer_imgui_drawer.h>
#endif

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT
#include <vclib/qt/mesh_viewer.h>
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW
#include <vclib/glfw/viewer_window.h>
#endif

#include <vclib/render/drawable/drawable_mesh.h>

#if defined(VCLIB_RENDER_EXAMPLES_WITH_GLFW) && defined(VCLIB_WITH_IMGUI)
using ImguiMeshViewer = vcl::RenderApp<
    vcl::glfw::WindowManager,
    vcl::Canvas,
    vcl::imgui::ImGuiDrawer,
    vcl::imgui::MeshViewerDrawerImgui>;
#endif

template<vcl::MeshConcept MeshType>
void pushMeshOnVector(
    std::shared_ptr<vcl::DrawableObjectVector>& vector,
    MeshType&&                                  mesh)
{
    if constexpr (vcl::DrawableObjectConcept<MeshType>)
        vector->pushBack(std::forward<MeshType>(mesh));
    else {
        using DrawableMesh = vcl::DrawableMesh<vcl::RemoveRef<MeshType>>;
        vector->pushBack(DrawableMesh(std::forward<MeshType>(mesh)));
    }
}

auto defaultViewer()
{
#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT
    return vcl::qt::MeshViewer();
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW
#ifdef VCLIB_WITH_IMGUI
    return ImguiMeshViewer();
#else  // VCLIB_WITH_IMGUI
    return vcl::glfw::ViewerWindow();
#endif // VCLIB_WITH_IMGUI
#endif
}

template<vcl::MeshConcept... MeshTypes>
void showMeshesOnViewer(
    int    argc,
    char** argv,
    auto&  viewer,
    MeshTypes&&... meshes)
{
    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();

    (pushMeshOnVector(vector, std::forward<MeshTypes>(meshes)), ...);

    viewer.setDrawableObjectVector(vector);

#if VCLIB_RENDER_EXAMPLES_WITH_GLFW
    viewer.fitScene();
#endif

    viewer.show();
}

template<vcl::MeshConcept MeshTypes>
void showMeshesOnViewer(
    int                      argc,
    char**                   argv,
    auto&                    viewer,
    std::vector<MeshTypes>&& meshes,
    bool                     pbrMode  = false,
    const std::string&       panorama = "")
{
    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();

    for (auto&& mesh : meshes)
        (pushMeshOnVector(vector, std::move(mesh)));

    viewer.setDrawableObjectVector(vector);

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

#if VCLIB_RENDER_EXAMPLES_WITH_GLFW
    viewer.fitScene();
#endif

    viewer.show();
}

template<vcl::MeshConcept... MeshTypes>
int showMeshesOnDefaultViewer(int argc, char** argv, MeshTypes&&... meshes)
{
#if VCLIB_RENDER_EXAMPLES_WITH_QT
    auto application = vcl::qt::qAppl(argc, argv);
#endif

    auto viewer = defaultViewer();

    showMeshesOnViewer(argc, argv, viewer, std::forward<MeshTypes>(meshes)...);

#if VCLIB_RENDER_EXAMPLES_WITH_QT
    viewer.showMaximized();
    return application.exec();
#else
    (void) argc; // unused
    (void) argv;
    return 0;
#endif
}

template<vcl::MeshConcept MeshTypes>
int showMeshesOnDefaultViewer(
    int                      argc,
    char**                   argv,
    std::vector<MeshTypes>&& meshes,
    bool                     pbrMode  = false,
    const std::string&       panorama = "")
{
#if VCLIB_RENDER_EXAMPLES_WITH_QT
    auto application = vcl::qt::qAppl(argc, argv);
#endif

    auto viewer = defaultViewer();

    showMeshesOnViewer(
        argc, argv, viewer, std::move(meshes), pbrMode, panorama);

#if VCLIB_RENDER_EXAMPLES_WITH_QT
    viewer.showMaximized();
    return application.exec();
#else
    (void) argc; // unused
    (void) argv;
    return 0;
#endif
}

#endif // VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H
