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

protected:
    MeshViewerRenderApp& viewer() const;

    DrawableObjectVectorTree& drawableObjectVectorTree() const;

    void keyPressEvent(QKeyEvent* event) override;

public:
    using EditorType = MeshViewerRenderApp::EditorType;
    using ViewerType = MeshViewerRenderApp::ViewerType;

    explicit MeshViewer(QWidget* parent = nullptr);
    ~MeshViewer();

private:
    const DrawableObjectVector& drawableObjectVector() const;

    void setDrawableObjectVector(
        const std::shared_ptr<vcl::DrawableObjectVector>& v);

public:
    /**
     * @brief Returns the ID of the currently selected mesh.
     */
    uint selectedMesh() const;

    /**
     * @brief Adds a mesh to the end of the scene.
     * @param[in] mesh: The mesh to add.
     * @return The ID assigned to the new mesh.
     */
    template<typename MeshType>
    uint pushMesh(MeshType&& mesh)
    {
        uint id = 0;
        if constexpr (vcl::DrawableObjectConcept<MeshType>) {
            id = viewer().pushDrawableObject(std::forward<MeshType>(mesh));
        } else {
            using DType = vcl::DrawableMesh<vcl::RemoveRef<MeshType>>;
            id = viewer().pushDrawableObject(DType(std::forward<MeshType>(mesh)));
        }
        updateGUI();
        return id;
    }

    /**
     * @brief Removes a mesh from the scene by its ID.
     * @param[in] id: The ID of the mesh to remove.
     * @return True if the mesh was successfully removed, false otherwise.
     */
    bool removeMesh(uint id);

    /**
     * @brief Triggers an update of the mesh with the given ID.
     * @param[in] id: The ID of the mesh to update.
     * @return True if the update was successful, false otherwise.
     */
    bool updateMesh(uint id);

    /**
     * @brief Inserts a mesh at a specific position in the scene.
     * @param[in] pos: The position to insert the mesh at.
     * @param[in] mesh: The mesh to insert.
     * @return True if the insertion was successful, false otherwise.
     */
    template<typename MeshType>
    bool insertMesh(uint pos, MeshType&& mesh)
    {
        bool success = false;
        if constexpr (vcl::DrawableObjectConcept<MeshType>) {
            success = viewer().insertDrawableObject(pos, std::forward<MeshType>(mesh));
        } else {
            using DType = vcl::DrawableMesh<vcl::RemoveRef<MeshType>>;
            success = viewer().insertDrawableObject(pos, DType(std::forward<MeshType>(mesh)));
        }
        if (success) updateGUI();
        return success;
    }

    /**
     * @brief Clears all meshes from the scene.
     */
    void clearMeshes();

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
    void visibilityDrawableObjectChanged();

    void selectedDrawableObjectChanged(uint i);

    void meshRenderSettingsUpdated();

    void crossSectionSettingsUpdated();

    void fitScene();

    void fitView();

    void updateGUI();
};

} // namespace vcl::qt

#endif // VCL_QT_MESH_VIEWER_H
