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
#include <vclib/render/input.h>
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
    using ViewerApp =
        vcl::RenderApp<vcl::HeadlessManager, vcl::Canvas, vcl::ViewerDrawer>;

    ViewerApp mApp;

public:
    using EditorType = ViewerApp::EditorType;
    using ViewerType = ViewerApp::ViewerType;

    explicit HeadlessMeshViewer(
        const std::string& title  = "VCLib Headless Mesh Viewer",
        uint               width  = 1024,
        uint               height = 768) : mApp(title, width, height)
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

    /**
     * @brief Pushes an editor.
     * @param[in] active: Whether the editor should be active upon pushing.
     */
    template<template<typename> typename EditorT>
    auto pushEditor(bool active = false)
    {
        return mApp.template pushEditor<EditorT>(active);
    }

    /**
     * @brief A dummy update GUI method required to satisfy the
     * MeshViewerConcept.
     */
    void updateGUI() {}

    /**
     * @brief Refreshes the active editors.
     */
    void refreshEditors() { mApp.refreshEditors(); }

    /**
     * @brief Retrieves the current camera object.
     * @return The current camera.
     */
    Camera<float> camera() const { return mApp.camera(); }

    /**
     * @brief Sets the camera.
     * @param[in] c: The new camera object.
     */
    void setCamera(const Camera<float>& c) { mApp.setCamera(c); }

    /**
     * @brief Adjusts the view to fit the whole scene.
     */
    void fitScene() { mApp.fitScene(); }

    /**
     * @brief Adjusts the view to fit the current selected drawable objects.
     */
    void fitView() { mApp.fitView(); }

    /**
     * @brief Shows the viewer window and starts the render loop.
     */
    void show() { mApp.show(); }

    /**
     * @brief Shows the viewer window maximized.
     * In headless mode, this does nothing but is kept to satisfy the concept.
     */
    void showMaximized() {}

    /**
     * @brief Sets the viewer settings.
     * @param[in] settings: The new viewer settings.
     */
    void setViewerSettings(const ViewerSettings& settings)
    {
        mApp.setViewerSettings(settings);
    }

    /**
     * @brief Retrieves the current viewer settings.
     * @return The viewer settings.
     */
    const ViewerSettings& viewerSettings() const
    {
        return mApp.viewerSettings();
    }

    /**
     * @brief Sets the panorama image to be used as background.
     * @param[in] panorama: The filename/path of the panorama image.
     */
    void setPanorama(const std::string& panorama)
    {
        mApp.setPanorama(panorama);
    }

    /**
     * @brief Changes the current zoom (scale) of the trackball.
     * @param[in] factor: Positive value to zoom in, negative to zoom out.
     */
    void trackballZoom(float factor) { mApp.trackballZoom(factor); }

    /**
     * @brief Pans the current view in the camera coordinate system.
     * @param[in] translation: 3D translation vector.
     */
    void trackballPan(const Point3f& translation)
    {
        mApp.trackballPan(translation);
    }

    /**
     * @brief Rotates the trackball around an arbitrary axis.
     * @param[in] axis: Rotation axis.
     * @param[in] angleRad: Rotation angle in radians.
     */
    void trackballRotate(const Point3f& axis, float angleRad)
    {
        mApp.trackballRotate(axis, angleRad);
    }

    /**
     * @brief Rolls the trackball around the camera view axis.
     * @param[in] angleRad: Rotation angle in radians.
     */
    void trackballRoll(float angleRad) { mApp.trackballRoll(angleRad); }

    /**
     * @brief Sets the background color of the viewer.
     * @param[in] color: The background color.
     */
    void setBackgroundColor(const vcl::Color& color)
    {
        mApp.setBackgroundColor(color);
    }

    /**
     * @brief Retrieves the current background color.
     * @return The background color.
     */
    const vcl::Color& backgroundColor() const { return mApp.backgroundColor(); }

    /**
     * @brief Simulates a key press event.
     */
    void simulateKeyPress(vcl::Key::Enum key) { mApp.simulateKeyPress(key); }

    /**
     * @brief Simulates a key release event.
     */
    void simulateKeyRelease(vcl::Key::Enum key)
    {
        mApp.simulateKeyRelease(key);
    }

    /**
     * @brief Simulates a mouse move event.
     */
    void simulateMouseMove(double x, double y) { mApp.simulateMouseMove(x, y); }

    /**
     * @brief Simulates a mouse press event.
     */
    void simulateMousePress(vcl::MouseButton::Enum button, double x, double y)
    {
        mApp.simulateMousePress(button, x, y);
    }

    /**
     * @brief Simulates a mouse release event.
     */
    void simulateMouseRelease(vcl::MouseButton::Enum button, double x, double y)
    {
        mApp.simulateMouseRelease(button, x, y);
    }

    /**
     * @brief Simulates a mouse double click event.
     */
    void simulateMouseDoubleClick(
        vcl::MouseButton::Enum button,
        double                 x,
        double                 y)
    {
        mApp.simulateMouseDoubleClick(button, x, y);
    }

    /**
     * @brief Simulates a mouse scroll event.
     */
    void simulateMouseScroll(double x, double y)
    {
        mApp.simulateMouseScroll(x, y);
    }

    /**
     * @brief Simulates setting the key modifiers.
     */
    void simulateSetModifiers(const vcl::KeyModifiers& modifiers)
    {
        mApp.simulateSetModifiers(modifiers);
    }
};

} // namespace vcl

#endif // VCL_RENDER_HEADLESS_MESH_VIEWER_H
