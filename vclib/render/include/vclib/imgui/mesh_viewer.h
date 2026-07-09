// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IMGUI_MESH_VIEWER_H
#define VCL_IMGUI_MESH_VIEWER_H

#include <vclib/glfw/window_manager.h>
#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/mesh_viewer_imgui_drawer.h>
#include <vclib/render/canvas.h>
#include <vclib/render/concepts/drawable_object.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/render_app.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

namespace vcl::imgui {

class MeshViewer
{
    using ViewerApp = vcl::RenderApp<
        vcl::glfw::WindowManager,
        vcl::Canvas,
        vcl::imgui::ImGuiDrawer,
        vcl::imgui::MeshViewerDrawerImgui>;

    ViewerApp mApp;

public:
    using EditorType = ViewerApp::EditorType;
    using ViewerType = ViewerApp::ViewerType;

    explicit MeshViewer(const std::string& title = "VCLib Mesh Viewer") :
            mApp(title)
    {
    }

    uint selectedMesh() const
    {
        return mApp.drawableObjectVector().selectedObjectId();
    }

    template<typename MeshType>
    uint pushMesh(MeshType&& mesh)
    {
        uint id = 0;
        if constexpr (vcl::DrawableObjectConcept<MeshType>) {
            id = mApp.pushDrawableObject(std::forward<MeshType>(mesh));
        } else {
            using DType = vcl::DrawableMesh<vcl::RemoveRef<MeshType>>;
            id = mApp.pushDrawableObject(DType(std::forward<MeshType>(mesh)));
        }
        return id;
    }

    bool removeMesh(uint id) { return mApp.removeDrawableObject(id); }

    bool updateMesh(uint id)
    {
        return mApp.updateDrawableObject(id);
    }

    template<typename MeshType>
    bool insertMesh(uint pos, MeshType&& mesh)
    {
        bool success = false;
        if constexpr (vcl::DrawableObjectConcept<MeshType>) {
            success = mApp.insertDrawableObject(pos, std::forward<MeshType>(mesh));
        } else {
            using DType = vcl::DrawableMesh<vcl::RemoveRef<MeshType>>;
            success = mApp.insertDrawableObject(pos, DType(std::forward<MeshType>(mesh)));
        }
        return success;
    }

    void clearMeshes()
    {
        mApp.clearDrawableObjects();
    }

    template<template<typename> typename EditorT>
    auto pushEditor()
    {
        return mApp.template pushEditor<EditorT>();
    }

    void refreshEditors()
    {
        mApp.refreshEditors();
    }

    Camera<float> camera() const { return mApp.camera(); }

    void setCamera(const Camera<float>& c) { mApp.setCamera(c); }

    void fitScene() { mApp.fitScene(); }

    void fitView() { mApp.fitView(); }
    
    void show() { mApp.show(); }

    void showMaximized() { mApp.showMaximized(); }

    void setPbrSettings(const PBRViewerSettings& settings);

    PBRViewerSettings pbrSettings() const;

    void setPanorama(const std::string& panorama);
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_MESH_VIEWER_H
