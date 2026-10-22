// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_MESH_VIEWER_H
#define VCL_RENDER_MESH_VIEWER_H

#include "application.h"
#include "concepts/mesh_viewer.h"
#include "editors.h"

#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/headless_mesh_viewer.h>
#include <vclib/render/settings/render_mode.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/mesh_viewer.h>
#endif
#if VCLIB_WITH_GLFW && VCLIB_WITH_IMGUI
#include <vclib/imgui/mesh_viewer.h>
#endif

#include <type_traits>

namespace vcl {

enum class MeshViewerType { QT, IMGUI_GLFW, HEADLESS, DEFAULT };

namespace detail {

template<MeshViewerType VwrType>
consteval auto meshViewerType()
{
    if constexpr (VwrType == MeshViewerType::QT) {
#ifdef VCLIB_WITH_QT
        return std::type_identity<qt::MeshViewer> {};
#else
        static_assert(
            false,
            "Qt is not available. Please enable Qt support to use MeshViewer.");
#endif
    }
    else if constexpr (VwrType == MeshViewerType::IMGUI_GLFW) {
#if VCLIB_WITH_GLFW && VCLIB_WITH_IMGUI
        return std::type_identity<imgui::MeshViewer> {};
#else
        static_assert(
            false,
            "GLFW+ImGui is not available. Please enable GLFW and ImGui support "
            "to use MeshViewer.");
#endif
    }
    else if constexpr (VwrType == MeshViewerType::HEADLESS) {
        return std::type_identity<HeadlessMeshViewer> {};
    }
    else if constexpr (VwrType == MeshViewerType::DEFAULT) {
#ifdef VCLIB_WITH_QT
        return std::type_identity<qt::MeshViewer> {};
#elif VCLIB_WITH_GLFW && VCLIB_WITH_IMGUI
        return std::type_identity<imgui::MeshViewer> {};
#else
        return std::type_identity<HeadlessMeshViewer> {};
#endif
    }
}

// given a MeshViewerType, return the corresponding ApplicationType
constexpr ApplicationType toApplicationType(MeshViewerType vwr)
{
    switch (vwr) {
    case MeshViewerType::QT: return ApplicationType::QT;
    case MeshViewerType::IMGUI_GLFW: return ApplicationType::GLFW;
    case MeshViewerType::HEADLESS: return ApplicationType::HEADLESS;
    case MeshViewerType::DEFAULT: return ApplicationType::DEFAULT;
    }
}

} // namespace detail

template<MeshViewerType VwrType = MeshViewerType::DEFAULT>
using MeshViewerT = typename decltype(detail::meshViewerType<VwrType>())::type;

/**
 * @brief The MeshViewer class is an alias to the default mesh viewer class
 * used in VCLib, providing a complete GUI for manipulating and rendering
 * meshes.
 *
 * Depending on the available window managers and GUI libraries, the MeshViewer
 * class is aliased to one of the following classes:
 * - If Qt is available, it is aliased to `vcl::qt::MeshViewer`.
 * - Else if GLFW is available and ImGui is enabled, it is aliased to
 *   `vcl::imgui::MeshViewer`.
 * - Else, a compilation error is triggered if no GUI toolkit is available
 *   and a MeshViewer instance is created.
 */
using MeshViewer = MeshViewerT<MeshViewerType::DEFAULT>;

inline void pushDefaultEditors(MeshViewerConcept auto& viewer)
{
    viewer.template pushEditor<MeshSelectorEditor>(true);
    viewer.template pushEditor<BoundingBoxEditor>();
    viewer.template pushEditor<InfoEditor>();
    viewer.template pushEditor<SelectionEditor>();
    viewer.template pushEditor<ShowNormalsEditor>();
}

template<MeshConcept... MeshTypes>
void showOnMeshViewer(
    int                     argc,
    char**                  argv,
    MeshViewerConcept auto& viewer,
    MeshTypes&&... meshes)
{
    auto prepare = [](auto&& m) -> decltype(auto) {
        using MType = std::remove_cvref_t<decltype(m)>;
        if constexpr (std::is_base_of_v<vcl::AbstractDrawableMesh, MType>) {
            return std::forward<decltype(m)>(m);
        }
        else {
            return vcl::makeDrawable(std::forward<decltype(m)>(m));
        }
    };

    (viewer.pushDrawableObject(prepare(std::forward<MeshTypes>(meshes))), ...);

    viewer.fitScene();

    viewer.showMaximized();
}

template<MeshConcept MeshTypes>
void showOnMeshViewer(
    int                      argc,
    char**                   argv,
    MeshViewerConcept auto&  viewer,
    std::vector<MeshTypes>&& meshes,
    RenderMode               renderMode = RenderMode::CLASSIC,
    const std::string&       panorama   = "")
{
    for (auto&& mesh : meshes) {
        if constexpr (std::is_base_of_v<vcl::AbstractDrawableMesh, MeshTypes>) {
            viewer.pushDrawableObject(std::move(mesh));
        }
        else {
            viewer.pushDrawableObject(vcl::makeDrawable(std::move(mesh)));
        }
    }

    auto sts = viewer.viewerSettings();

    if (!panorama.empty()) {
        sts.panoramaPath             = panorama;
        sts.imageBasedLighting       = true;
        sts.renderBackgroundPanorama = true;
    }
    sts.renderMode = renderMode;
    viewer.setViewerSettings(sts);

    viewer.fitScene();

    viewer.showMaximized();
}

template<MeshViewerType MVT = MeshViewerType::DEFAULT, MeshConcept... MeshTypes>
int showOnMeshViewer(int argc, char** argv, MeshTypes&&... meshes)
{
    ApplicationT<detail::toApplicationType(MVT)> app(argc, argv);

    MeshViewerT<MVT> viewer;

    pushDefaultEditors(viewer);

    showOnMeshViewer(argc, argv, viewer, std::forward<MeshTypes>(meshes)...);

    if constexpr (MVT == MeshViewerType::HEADLESS) {
        viewer.screenshot("HeadlessMeshViewerScreenShot.png");
    }

    return app.exec();
}

template<
    MeshViewerType   MVT = MeshViewerType::DEFAULT,
    vcl::MeshConcept MeshTypes>
int showOnMeshViewer(
    int                      argc,
    char**                   argv,
    std::vector<MeshTypes>&& meshes,
    RenderMode               renderMode = RenderMode::CLASSIC,
    const std::string&       panorama   = "")
{
    ApplicationT<detail::toApplicationType(MVT)> app(argc, argv);

    MeshViewerT<MVT> viewer;

    pushDefaultEditors(viewer);

    showOnMeshViewer(
        argc, argv, viewer, std::move(meshes), renderMode, panorama);

    if constexpr (MVT == MeshViewerType::HEADLESS) {
        viewer.screenshot("HeadlessMeshViewerScreenShot.png");
    }

    return app.exec();
}

} // namespace vcl

#endif // VCL_RENDER_MESH_VIEWER_H
