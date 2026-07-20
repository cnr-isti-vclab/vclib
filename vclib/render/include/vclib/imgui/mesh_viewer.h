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
#include <vclib/render/settings/viewer_settings.h>

namespace vcl::imgui {

/**
 * @brief The MeshViewer class for the ImGui backend.
 *
 * This class provides a complete GUI for manipulating and rendering meshes
 * using GLFW and ImGui. It manages an internal `RenderApp` instance.
 */
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

    /**
     * @brief Returns the ID of the currently selected drawable object.
     */
    uint selectedDrawableObject() const
    {
        return mApp.drawableObjectVector().selectedObjectId();
    }

    /**
     * @brief Returns a shared pointer to the i-th drawable object.
     * @param i The index of the object.
     */
    std::shared_ptr<vcl::DrawableObject> drawableObject(uint i)
    {
        return mApp.drawableObjectVector().at(i);
    }

    /**
     * @brief Returns a const shared pointer to the i-th drawable object.
     * @param i The index of the object.
     */
    std::shared_ptr<const vcl::DrawableObject> drawableObject(uint i) const
    {
        return mApp.drawableObjectVector().at(i);
    }

    /**
     * @brief Returns the number of drawable objects currently in the scene.
     */
    uint drawableObjectsCount() const
    {
        return mApp.drawableObjectVector().size();
    }

    /**
     * @brief Returns a const reference to the underlying DrawableObjectVector.
     */
    const vcl::DrawableObjectVector& drawableObjects() const
    {
        return mApp.drawableObjectVector();
    }

    /**
     * @brief Adds a drawable object to the end of the scene.
     * @param obj The drawable object to add.
     * @return The ID assigned to the new object.
     */
    template<vcl::DrawableObjectConcept ObjType>
    uint pushDrawableObject(ObjType&& obj)
    {
        return mApp.pushDrawableObject(std::forward<ObjType>(obj));
    }

    /**
     * @brief Adds a shared_ptr to a drawable object to the end of the scene.
     * @param obj The drawable object to add.
     * @return The ID assigned to the new object.
     */
    uint pushDrawableObject(std::shared_ptr<vcl::DrawableObject> obj)
    {
        return mApp.pushDrawableObject(std::move(obj));
    }

    /**
     * @brief Removes a drawable object from the scene by its ID.
     * @param id The ID of the object to remove.
     * @return True if the object was successfully removed, false otherwise.
     */
    bool removeDrawableObject(uint id) { return mApp.removeDrawableObject(id); }

    /**
     * @brief Inserts a drawable object at a specific position in the scene.
     * @param pos The position to insert the object at.
     * @param obj The object to insert.
     * @return True if the insertion was successful, false otherwise.
     */
    template<vcl::DrawableObjectConcept ObjType>
    bool insertDrawableObject(uint pos, ObjType&& obj)
    {
        return mApp.insertDrawableObject(pos, std::forward<ObjType>(obj));
    }

    /**
     * @brief Inserts a shared_ptr to a drawable object at a specific position
     * in the scene.
     * @param pos The position to insert the object at.
     * @param obj The object to insert.
     * @return True if the insertion was successful, false otherwise.
     */
    bool insertDrawableObject(
        uint                                 pos,
        std::shared_ptr<vcl::DrawableObject> obj)
    {
        return mApp.insertDrawableObject(pos, std::move(obj));
    }

    /**
     * @brief Clears all drawable objects from the scene.
     */
    void clearDrawableObjects() { mApp.clearDrawableObjects(); }

    template<template<typename> typename EditorT>
    auto pushEditor()
    {
        return mApp.template pushEditor<EditorT>();
    }

    void refreshEditors() { mApp.refreshEditors(); }

    Camera<float> camera() const { return mApp.camera(); }

    void setCamera(const Camera<float>& c) { mApp.setCamera(c); }

    void fitScene() { mApp.fitScene(); }

    void fitView() { mApp.fitView(); }

    void show() { mApp.show(); }

    void showMaximized() { mApp.showMaximized(); }

    void setViewerSettings(const ViewerSettings& settings);

    ViewerSettings viewerSettings() const;

    void setPanorama(const std::string& panorama);
};

} // namespace vcl::imgui

#endif // VCL_IMGUI_MESH_VIEWER_H
