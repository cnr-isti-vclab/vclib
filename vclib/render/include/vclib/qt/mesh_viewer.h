// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_MESH_VIEWER_H
#define VCL_QT_MESH_VIEWER_H

#include "gui/drawable_object_vector_tree.h"
#include "gui/settings_dialog/settings_dialog_data.h"
#include "gui/text_edit_logger.h"
#include "gui/toolbar_frames.h"

#include <vclib/qt/mesh_viewer_render_app.h>
#include <vclib/render/concepts/drawable_object.h>
#include <vclib/render/drawable/drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/editors.h>
#include <vclib/render/settings.h>
#include <vclib/render/settings/viewer_settings.h>

#include <vclib/base.h>

#include <nlohmann/json.hpp>

#include <QMainWindow>

#include <fstream>

class QDockWidget;

namespace vcl::qt {

namespace Ui {
class MeshViewer;
} // namespace Ui

class ViewerSettingsFrame;

class KeyFilter : public QObject
{
    Q_OBJECT

public:
    KeyFilter(QObject* parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

class MeshViewer : public QMainWindow
{
    Q_OBJECT

public:
    using EditorType = MeshViewerRenderApp::EditorType;
    using ViewerType = MeshViewerRenderApp::ViewerType;

private:
    Ui::MeshViewer* mUI;

    ViewerSettingsFrame* mViewerSettingsFrame      = nullptr;
    QDockWidget*         mViewerSettingsDockWidget = nullptr;

    QAction* mSpacerAction = nullptr;

    std::shared_ptr<vcl::DrawableObjectVector> mDrawableObjectVector;
    SettingsDialogData                         mSettingsData;

protected:
    MeshViewerRenderApp& viewer() const;

    DrawableObjectVectorTree& drawableObjectVectorTree() const;

    void addEditorFrame(QWidget* frame);

    void keyPressEvent(QKeyEvent* event) override;

public:
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
    auto pushEditor(bool active = false)
    {
        nlohmann::json j;
        std::filesystem::path configDir = vcl::appConfigDirectory("vclib");
        std::string filePath = (configDir / vcl::RENDER_SETTINGS_FILE_NAME).string();
        std::ifstream in(filePath);
        if (in.is_open()) {
            try {
                in >> j;
            } catch (...) {
                // Ignore parse errors
            }
        }

        auto editor = viewer().template pushEditor<EditorT>(active, j);

        if constexpr (std::is_same_v<
                          EditorT<ViewerType>,
                          vcl::MeshSelectorEditor<ViewerType>>) {
            editor->setOnObjectSelectedFunction([this](uint id) {
                drawableObjectVectorTree().setSelectedItem(id);
            });
        }

        // Store editor for settings dialog if it has a settings frame
        using SettingsFrame = typename EditorFrameTraits<EditorT, ViewerType>::SettingsFrameType;
        if constexpr (!std::is_same_v<SettingsFrame, void>) {
            mSettingsData.addTab(
                std::make_shared<EditorSettingsTabImpl<EditorT<ViewerType>, SettingsFrame>>(
                    editor, QString::fromStdString(editor->name())
                )
            );
        }

        using ToolbarFrameType =
            typename EditorFrameTraits<EditorT, ViewerType>::ToolbarFrameType;
        if constexpr (!std::is_same_v<ToolbarFrameType, void>) {
            addEditorFrame(new ToolbarFrameType(editor));
        }

        return editor;
    }

    void refreshEditors();

    TextEditLogger& logger();

    void setDrawVectorIconFunction(
        const DrawableObjectVectorTree::IconFunction& f);

    Camera<float> camera() const;

    void setCamera(const Camera<float>& c);

    // void showRenderModeSelector(bool show);

    void setViewerSettings(const ViewerSettings& settings);

    const ViewerSettings& viewerSettings() const;

    void setPanorama(const std::string& panorama);

    /**
     * @brief Changes the current zoom (scale) of the trackball.
     * @param[in] factor: Positive value to zoom in, negative to zoom out.
     */
    void trackballZoom(float factor) { viewer().trackballZoom(factor); }

    /**
     * @brief Pans the current view in the camera coordinate system.
     * @param[in] translation: 3D translation vector.
     */
    void trackballPan(const Point3f& translation)
    {
        viewer().trackballPan(translation);
    }

    /**
     * @brief Rotates the trackball around an arbitrary axis.
     * @param[in] axis: Rotation axis.
     * @param[in] angleRad: Rotation angle in radians.
     */
    void trackballRotate(const Point3f& axis, float angleRad)
    {
        viewer().trackballRotate(axis, angleRad);
    }

    /**
     * @brief Rolls the trackball around the camera view axis.
     * @param[in] angleRad: Rotation angle in radians.
     */
    void trackballRoll(float angleRad) { viewer().trackballRoll(angleRad); }

    /**
     * @brief Sets the background color of the viewer.
     * @param[in] color: The background color.
     */
    void setBackgroundColor(const vcl::Color& color);

    /**
     * @brief Retrieves the current background color.
     * @return The background color.
     */
    const vcl::Color& backgroundColor() const;

private:
    void setupSettingsButton();

public slots:
    void fitScene();

    void fitView();

    void updateGUI();

private slots:
    void visibilityDrawableObjectChanged();

    void selectedDrawableObjectChanged(uint i);

    void meshRenderSettingsUpdated();

    void applyToAllToggled(bool checked);

    void renderModeChanged();

    void openSettings();
};

} // namespace vcl::qt

#endif // VCL_QT_MESH_VIEWER_H
