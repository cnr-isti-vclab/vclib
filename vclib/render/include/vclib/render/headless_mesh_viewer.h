// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_HEADLESS_MESH_VIEWER_H
#define VCL_RENDER_HEADLESS_MESH_VIEWER_H

#include <vclib/render/canvas.h>
#include <vclib/render/concepts/drawable_object.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/headless_manager.h>
#include <vclib/render/render_app.h>
#include <vclib/render/settings/viewer_settings.h>

namespace vcl {

/**
 * @brief The HeadlessMeshViewer class.
 *
 * This class provides an interface for manipulating and rendering meshes
 * in headless environments, fully respecting the MeshViewerConcept.
 */
class HeadlessMeshViewer
{
    using ViewerApp = vcl::RenderApp<
        vcl::HeadlessManager,
        vcl::Canvas,
        vcl::ViewerDrawer>;

    ViewerApp mApp;

public:
    using EditorType = ViewerApp::EditorType;
    using ViewerType = ViewerApp::ViewerType;

    explicit HeadlessMeshViewer(
        const std::string& title  = "VCLib Headless Mesh Viewer",
        uint               width  = 1024,
        uint               height = 768) :
            mApp(title, width, height)
    {
        mApp.init();
    }

    /**
     * @brief Resizes the canvas.
     * @param[in] width: The new width in pixels.
     * @param[in] height: The new height in pixels.
     */
    void resize(uint width, uint height) { mApp.resize(width, height); }

    /**
     * @brief Requests a screenshot and saves it to a file. 
     * Auto-concludes by running the render loop.
     */
    void screenshot(const std::string& filename)
    {
        mApp.screenshot(filename);
        mApp.show();
    }

    /**
     * @brief Requests a screenshot and saves it to a vcl::Image. 
     * Auto-concludes by running the render loop.
     */
    void screenshot(vcl::Image& image)
    {
        mApp.screenshot(image);
        mApp.show();
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
     * @param[in] i: The index of the object.
     */
    std::shared_ptr<vcl::DrawableObject> drawableObject(uint i)
    {
        return mApp.drawableObjectVector().at(i);
    }

    /**
     * @brief Returns a const shared pointer to the i-th drawable object.
     * @param[in] i: The index of the object.
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
     * @param[in] obj: The drawable object to add.
     * @return The ID assigned to the new object.
     */
    template<vcl::DrawableObjectConcept ObjType>
    uint pushDrawableObject(ObjType&& obj)
    {
        return mApp.pushDrawableObject(std::forward<ObjType>(obj));
    }

    /**
     * @brief Adds a shared_ptr to a drawable object to the end of the scene.
     * @param[in] obj: The drawable object to add.
     * @return The ID assigned to the new object.
     */
    uint pushDrawableObject(std::shared_ptr<vcl::DrawableObject> obj)
    {
        return mApp.pushDrawableObject(std::move(obj));
    }

    /**
     * @brief Removes a drawable object from the scene by its ID.
     * @param[in] id: The ID of the object to remove.
     * @return True if the object was successfully removed, false otherwise.
     */
    bool removeDrawableObject(uint id) { return mApp.removeDrawableObject(id); }

    /**
     * @brief Inserts a drawable object at a specific position in the scene.
     * @param[in] pos: The position to insert the object at.
     * @param[in] obj: The object to insert.
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
     * @param[in] pos: The position to insert the object at.
     * @param[in] obj: The object to insert.
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
    auto pushEditor(bool active = false)
    {
        return mApp.template pushEditor<EditorT>(active);
    }

    // automatic update... member function here to satisfy the interface, but
    // does nothing for headless backend
    void updateGUI() {}

    void refreshEditors() { mApp.refreshEditors(); }

    Camera<float> camera() const { return mApp.camera(); }

    void setCamera(const Camera<float>& c) { mApp.setCamera(c); }

    void fitScene() { mApp.fitScene(); }

    void fitView() { mApp.fitView(); }

    void show() { mApp.show(); }

    void showMaximized() {}

    void setViewerSettings(const ViewerSettings& settings)
    {
        mApp.setViewerSettings(settings);
    }

    const ViewerSettings& viewerSettings() const
    {
        return mApp.viewerSettings();
    }

    void setPanorama(const std::string& panorama)
    {
        mApp.setPanorama(panorama);
    }
};

} // namespace vcl

#endif // VCL_RENDER_HEADLESS_MESH_VIEWER_H
