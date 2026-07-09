// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H
#define VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H

#include <vclib/mesh.h>
#include <vclib/render/concepts/drawable_object.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/mesh_viewer.h>

#ifdef VCLIB_WITH_QT
#include <QApplication>
#endif

template<vcl::MeshConcept... MeshTypes>
void showMeshesOnViewer(
    int    argc,
    char** argv,
    auto&  viewer,
    MeshTypes&&... meshes)
{
    (viewer.pushMesh(std::forward<MeshTypes>(meshes)), ...);

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

    vcl::MeshViewer viewer;

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

    vcl::MeshViewer viewer;

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
