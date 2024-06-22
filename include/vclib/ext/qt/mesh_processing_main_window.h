/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_EXT_QT_MESH_PROCESSING_MAIN_WINDOW_H
#define VCL_EXT_QT_MESH_PROCESSING_MAIN_WINDOW_H

#include <QMainWindow>

#include <vclib/ext/qt/gui/text_edit_logger.h>
#include <vclib/processing/action_manager.h>
#include <vclib/render/drawable/drawable_object_vector.h>

namespace vcl::qt {

namespace Ui {
class MeshProcessingMainWindow;
} // namespace Ui

class MeshProcessingMainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MeshProcessingMainWindow* mUI;

    proc::ActionManager mActionManager;

    std::shared_ptr<vcl::DrawableObjectVector> mMeshVector =
        std::make_shared<vcl::DrawableObjectVector>();

public:
    explicit MeshProcessingMainWindow(QWidget* parent = nullptr);
    ~MeshProcessingMainWindow();

public slots:
    void openMesh();

    void saveMeshAs();

    void openFilterDialog(bool);

    void applyFilter(
        const std::shared_ptr<proc::FilterMeshAction>& action,
        const proc::ParameterVector&                   params);

private:
    TextEditLogger& logger();

    void populateFilterMenu();

    void openFilterDialog(
        const std::shared_ptr<proc::FilterMeshAction>& action);

    static std::shared_ptr<vcl::DrawableObjectI> makeMeshDrawable(
        const std::shared_ptr<proc::MeshI>& mesh);

    static std::shared_ptr<vcl::proc::MeshI> toMesh(
        const std::shared_ptr<vcl::DrawableObjectI>& drawable);

    static std::shared_ptr<vcl::DrawableObjectI> toDrawableObject(
        const std::shared_ptr<vcl::proc::MeshI>& mesh);

    template<MeshConcept MeshType>
    static void setMeshInfo(MeshType& mesh)
    {
        std::string info;
        if constexpr (HasTriangles<MeshType>) {
            info += "TriMesh\n";
        }
        else if constexpr (HasPolygons<MeshType>) {
            info += "PolyMesh\n";
        }

        info += "Vertices: " + std::to_string(mesh.vertexNumber()) + "\n";
        if constexpr (HasFaces<MeshType>) {
            info += "Faces: " + std::to_string(mesh.faceNumber()) + "\n";
        }
        mesh.info() = info;
    }
};

} // namespace vcl::qt

#endif // VCL_EXT_QT_MESH_PROCESSING_MAIN_WINDOW_H
