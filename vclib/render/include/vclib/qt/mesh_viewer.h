// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_MESH_VIEWER_H
#define VCL_QT_MESH_VIEWER_H

#include "gui/drawable_object_vector_tree.h"
#include "utils.h"

#include <vclib/qt/gui/text_edit_logger.h>
#include <vclib/qt/mesh_viewer_render_app.h>
#include <vclib/render/concepts/drawable_object.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/editors.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

#include <QMainWindow>

namespace vcl::qt {

namespace Ui {
class MeshViewer;
} // namespace Ui

class KeyFilter : public QObject
{
    using QObject::QObject;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

class MeshViewer : public QMainWindow
{
    Q_OBJECT

    enum class RenderMode { CLASSIC = 0, PBR = 1 };

    Ui::MeshViewer* mUI;

    std::shared_ptr<vcl::DrawableObjectVector> mDrawableObjectVector;

    std::shared_ptr<vcl::AxisEditor<MeshViewerRenderApp::ViewerType>>
        mAxisEditor;
    std::shared_ptr<vcl::MeshSelectorEditor<MeshViewerRenderApp::ViewerType>>
        mMeshSelectorEditor;
    std::shared_ptr<vcl::BoundingBoxEditor<MeshViewerRenderApp::ViewerType>>
        mBoundingBoxEditor;
    std::shared_ptr<vcl::SelectionEditor<MeshViewerRenderApp::ViewerType>>
        mSelectionEditor;

protected:
    MeshViewerRenderApp& viewer() const;

    DrawableObjectVectorTree& drawableObjectVectorTree() const;

    void keyPressEvent(QKeyEvent* event) override;

public:
    using EditorType = MeshViewerRenderApp::EditorType;
    using ViewerType = MeshViewerRenderApp::ViewerType;

    explicit MeshViewer(QWidget* parent = nullptr);
    ~MeshViewer();

    /**
     * @brief Returns the ID of the currently selected drawable object.
     */
    uint selectedDrawableObject() const;

    /**
     * @brief Returns a shared pointer to the i-th drawable object.
     * @param i The index of the object.
     */
    std::shared_ptr<vcl::DrawableObject> drawableObject(uint i);

    /**
     * @brief Returns a const shared pointer to the i-th drawable object.
     * @param i The index of the object.
     */
    std::shared_ptr<const vcl::DrawableObject> drawableObject(uint i) const;

    /**
     * @brief Returns the number of drawable objects currently in the scene.
     */
    uint drawableObjectsCount() const;

    /**
     * @brief Returns a const reference to the underlying DrawableObjectVector.
     */
    const vcl::DrawableObjectVector& drawableObjects() const;

    /**
     * @brief Adds a drawable object to the end of the scene.
     * @param[in] obj: The object to add.
     * @return The ID assigned to the new object.
     */
    template<vcl::DrawableObjectConcept ObjType>
    uint pushDrawableObject(ObjType&& obj)
    {
        uint id = viewer().pushDrawableObject(std::forward<ObjType>(obj));
        updateGUI();
        return id;
    }

    /**
     * @brief Adds a shared_ptr to a drawable object to the end of the scene.
     * @param[in] obj: The object to add.
     * @return The ID assigned to the new object.
     */
    uint pushDrawableObject(std::shared_ptr<vcl::DrawableObject> obj)
    {
        uint id = viewer().pushDrawableObject(std::move(obj));
        updateGUI();
        return id;
    }

    /**
     * @brief Removes a drawable object from the scene by its ID.
     * @param[in] id: The ID of the object to remove.
     * @return True if the object was successfully removed, false otherwise.
     */
    bool removeDrawableObject(uint id);

    /**
     * @brief Triggers an update of the drawable object with the given ID.
     * @param[in] id: The ID of the object to update.
     * @return True if the update was successful, false otherwise.
     */
    bool updateDrawableObject(uint id);

    /**
     * @brief Inserts a drawable object at a specific position in the scene.
     * @param[in] pos: The position to insert the object at.
     * @param[in] obj: The object to insert.
     * @return True if the insertion was successful, false otherwise.
     */
    template<vcl::DrawableObjectConcept ObjType>
    bool insertDrawableObject(uint pos, ObjType&& obj)
    {
        bool success =
            viewer().insertDrawableObject(pos, std::forward<ObjType>(obj));
        if (success)
            updateGUI();
        return success;
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
        bool success = viewer().insertDrawableObject(pos, std::move(obj));
        if (success)
            updateGUI();
        return success;
    }

    /**
     * @brief Clears all drawable objects from the scene.
     */
    void clearDrawableObjects();

    template<template<typename> typename EditorT>
    auto pushEditor()
    {
        return viewer().template pushEditor<EditorT>();
    }

    void refreshEditors();

    TextEditLogger& logger();

    void setDrawVectorIconFunction(
        const DrawableObjectVectorTree::IconFunction& f);

    Camera<float> camera() const;

    void setCamera(const Camera<float>& c);

    // void showRenderModeSelector(bool show);

    void setPbrSettings(const PBRViewerSettings& settings);

    const PBRViewerSettings& pbrSettings() const;

    void setPanorama(const std::string& panorama);

public slots:
    void fitScene();

    void fitView();

    void updateGUI();

private slots:
    void visibilityDrawableObjectChanged();

    void selectedDrawableObjectChanged(uint i);

    void meshRenderSettingsUpdated();

    void applyToAllToggled(bool checked);
};

} // namespace vcl::qt

#endif // VCL_QT_MESH_VIEWER_H
