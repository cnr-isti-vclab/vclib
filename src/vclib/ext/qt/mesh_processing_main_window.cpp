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

#include <vclib/ext/qt/mesh_processing_main_window.h>
#include "ui_mesh_processing_main_window.h"

#include <QFileDialog>

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
}

MeshProcessingMainWindow::~MeshProcessingMainWindow()
{
    delete mUI;
}

void MeshProcessingMainWindow::openMesh()
{
    std::vector<proc::FileFormat> formats = mActionManager.loadMeshFormats();
    QString filter = filterFormatsToQString(formats);

    QFileDialog* dialog = new QFileDialog(this, "Open Mesh", "", filter);
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    if (dialog->exec() == QDialog::Accepted) {
        auto fs   = dialog->selectedFiles();
        auto frmt = dialog->selectedNameFilter();

        std::string format = frmt.toStdString();
        format             = format.substr(2, 4);

        std::string filename = fs.first().toStdString();
        std::string fnext    = FileInfo::extension(filename);
        if (fnext.empty() || fnext != format) {
            filename += "." + format;
        }

        auto mesh = mActionManager.loadMeshAction(format)->load(filename);
        mMeshVector->pushBack(meshToDrawableObject(mesh));
        mUI->meshViewer->update();
    }
}

void MeshProcessingMainWindow::saveMesh()
{
    // std::vector<proc::FileFormat> formats = mActionManager.saveMeshFormats();

    // QString filter = filterFormatsToQString(formats);

    // QFileDialog* dialog = new QFileDialog(this, "Save Mesh", "", filter);
    // dialog->setAcceptMode(QFileDialog::AcceptSave);
    // if (dialog->exec() == QDialog::Accepted) {
    //     auto fs   = dialog->selectedFiles();
    //     auto frmt = dialog->selectedNameFilter();

    //     std::cerr << frmt.toStdString() << "\n";

    //            // get format from frmt, which is '(*.stl )'
    //     std::string format = frmt.toStdString();
    //     format             = format.substr(2, 4);

    //     std::string filename = fs.first().toStdString();
    //     std::string fnext    = FileInfo::extension(filename);
    //     if (fnext.empty() || fnext != format) {
    //         filename += "." + format;
    //     }
    //     uint i = mUI->drawVectorFrame->selectedDrawableObject();
    //     std::shared_ptr<DrawableObjectI> d = mDrawVector->at(i);

    //     std::shared_ptr<vcl::TriMesh> m =
    //         std::dynamic_pointer_cast<vcl::TriMesh>(d);

    //     if (m) {
    //         // todo: use directly m when it will be a proc::TriMesh
    //         proc::TriMesh t;
    //         t.enableSameOptionalComponentsOf(*m);
    //         t.importFrom(*m);
    //         mActionManager.saveMeshAction(format)->save(filename, t);
    //     }
    // }
}


std::shared_ptr<DrawableObjectI> MeshProcessingMainWindow::meshToDrawableObject(
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

} // namespace vcl::qt
