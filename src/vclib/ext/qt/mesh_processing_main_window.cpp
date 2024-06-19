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

#include "ui_mesh_processing_main_window.h"
#include <vclib/ext/qt/mesh_processing_main_window.h>

#include <QFileDialog>

#include <vclib/ext/qt/gui/processing/action_file_dialog.h>
#include <vclib/ext/qt/utils/file_format.h>
#include <vclib/processing/actions.h>
#include <vclib/render/drawable/drawable_mesh.h>

namespace vcl::qt {

MeshProcessingMainWindow::MeshProcessingMainWindow(QWidget* parent) :
        QMainWindow(parent), mUI(new Ui::MeshProcessingMainWindow)
{
    mUI->setupUi(this);

    mUI->meshViewer->setDrawableObjectVector(mMeshVector);

    // populate action manager
    mActionManager.add(proc::vclibActions());

    connect(
        mUI->actionOpenMesh,
        &QAction::triggered,
        this,
        &MeshProcessingMainWindow::openMesh);

    connect(
        mUI->actionSaveMeshAs,
        &QAction::triggered,
        this,
        &MeshProcessingMainWindow::saveMeshAs);
}

MeshProcessingMainWindow::~MeshProcessingMainWindow()
{
    delete mUI;
}

void MeshProcessingMainWindow::openMesh()
{
    std::vector<proc::FileFormat> formats = mActionManager.loadMeshFormats();

    ActionOpenFileDialog<proc::LoadMeshAction>* dialog =
        new ActionOpenFileDialog<proc::LoadMeshAction>(
            mActionManager.loadMeshActionManager(), "Open Mesh", "", this);

    if (dialog->exec() == QDialog::Accepted) {
        auto fs = dialog->selectedFiles();

        std::string      filename = fs.first().toStdString();
        proc::FileFormat format   = FileInfo::extension(filename);
        auto             params   = dialog->parameters(format);

        auto mesh = mActionManager.loadMeshAction(format)->load(
            filename, params, mUI->meshViewer->logger());
        mMeshVector->pushBack(makeMeshDrawable(mesh));
        mUI->meshViewer->updateGUI();
        mUI->meshViewer->fitScene();
    }
}

void MeshProcessingMainWindow::saveMeshAs()
{
    if (mMeshVector->size() == 0) {
        return;
    }

    std::vector<proc::FileFormat> formats = mActionManager.saveMeshFormats();

    ActionSaveFileDialog<proc::SaveMeshAction>* dialog =
        new ActionSaveFileDialog<proc::SaveMeshAction>(
            mActionManager.saveMeshActionManager(), "Save Mesh", "", this);

    if (dialog->exec() == QDialog::Accepted) {
        auto fs = dialog->selectedFiles();

        std::string      filename = fs.first().toStdString();
        std::string      format   = FileInfo::extension(filename);
        proc::FileFormat f        = dialog->selectedFormat();

        if (f != proc::FileFormat(format)) {
            filename += "." + f.extensions().front();
        }

        uint i = mUI->meshViewer->selectedDrawableObject();
        std::shared_ptr<DrawableObjectI> d = mMeshVector->at(i);

        std::shared_ptr<proc::MeshI> m = toMesh(d);

        if (m) {
            auto params = dialog->parameters(format);
            mActionManager.saveMeshAction(format)->save(
                filename, *m, params, mUI->meshViewer->logger());
        }
    }
}

std::shared_ptr<DrawableObjectI> MeshProcessingMainWindow::makeMeshDrawable(
    const std::shared_ptr<proc::MeshI>& mesh)
{
    switch (mesh->type()) {
    case proc::MeshIType::TRI_MESH:
        return std::make_shared<DrawableMesh<proc::TriMesh>>(
            mesh->as<proc::TriMesh>());
    case proc::MeshIType::POLY_MESH:
        return std::make_shared<DrawableMesh<proc::PolyMesh>>(
            mesh->as<proc::PolyMesh>());
    default: return nullptr;
    }
}

std::shared_ptr<proc::MeshI> MeshProcessingMainWindow::toMesh(
    const std::shared_ptr<DrawableObjectI>& drawable)
{
    return std::dynamic_pointer_cast<proc::MeshI>(drawable);
}

std::shared_ptr<DrawableObjectI> MeshProcessingMainWindow::toDrawableObject(
    const std::shared_ptr<proc::MeshI>& mesh)
{
    return std::dynamic_pointer_cast<DrawableObjectI>(mesh);
}

} // namespace vcl::qt
