/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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
#include <vclib/qt/mesh_processing_main_window.h>

#include <QFileDialog>

#include <vclib/processing/actions.h>
#include <vclib/qt/gui/processing/action_file_dialog.h>
#include <vclib/qt/gui/processing/filter_mesh_dock_widget.h>
#include <vclib/qt/utils/file_format.h>
#include <vclib/render/drawable/drawable_mesh.h>

namespace vcl::qt {

MeshProcessingMainWindow::MeshProcessingMainWindow(QWidget* parent) :
        QMainWindow(parent), mUI(new Ui::MeshProcessingMainWindow)
{
    mUI->setupUi(this);

    mUI->meshViewer->setDrawableObjectVector(mMeshVector);

    // populate action manager
    mActionManager.add(proc::vclibActions());

    populateFilterMenu();

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
    std::vector<FileFormat> formats = mActionManager.loadMeshFormats();

    ActionOpenFileDialog<proc::LoadMeshAction>* dialog =
        new ActionOpenFileDialog<proc::LoadMeshAction>(
            mActionManager.loadMeshActionManager(), "Open Mesh", "", this);

    if (dialog->exec() == QDialog::Accepted) {
        auto fs = dialog->selectedFiles();

        double tTime = 0.0;

        for (const auto& f : fs) {
            std::string filename = f.toStdString();
            std::string pfn      = FileInfo::fileNameWithExtension(filename);
            FileFormat  format   = FileInfo::extension(filename);
            auto        params   = dialog->parameters(format);

            logger().startTimer();
            auto mesh = mActionManager.loadMeshAction(format)->load(
                filename, params, logger());
            logger().stopTimer();
            tTime += logger().time();
            logger().log(
                TextEditLogger::MESSAGE_LOG,
                pfn + " loaded in " + std::to_string(logger().time()) +
                    " seconds.");
            mMeshVector->pushBack(makeMeshDrawable(mesh));
        }

        logger().log(
            TextEditLogger::MESSAGE_LOG,
            "All meshes loaded in " + std::to_string(tTime) + " seconds.");

        mUI->meshViewer->updateGUI();
        mUI->meshViewer->fitScene();
    }
}

void MeshProcessingMainWindow::saveMeshAs()
{
    if (mMeshVector->size() == 0) {
        return;
    }

    std::vector<FileFormat> formats = mActionManager.saveMeshFormats();

    ActionSaveFileDialog<proc::SaveMeshAction>* dialog =
        new ActionSaveFileDialog<proc::SaveMeshAction>(
            mActionManager.saveMeshActionManager(), "Save Mesh", "", this);

    if (dialog->exec() == QDialog::Accepted) {
        auto fs = dialog->selectedFiles();

        std::string filename = fs.first().toStdString();
        std::string pfn      = FileInfo::fileNameWithExtension(filename);
        std::string format   = FileInfo::extension(filename);
        FileFormat  f        = dialog->selectedFormat();

        if (f != FileFormat(format)) {
            filename += "." + f.extensions().front();
        }

        uint i = mUI->meshViewer->selectedDrawableObject();
        std::shared_ptr<DrawableObject> d = mMeshVector->at(i);

        std::shared_ptr<proc::MeshI> m = toMesh(d);

        if (m) {
            auto params = dialog->parameters(format);

            logger().startTimer();
            mActionManager.saveMeshAction(format)->save(
                filename, *m, params, logger());
            logger().stopTimer();
            logger().log(
                TextEditLogger::MESSAGE_LOG,
                pfn + " saved in " + std::to_string(logger().time()) +
                    " seconds.");
        }
    }
}

void MeshProcessingMainWindow::openFilterDialog(bool)
{
    QAction* sender = qobject_cast<QAction*>(QObject::sender());

    std::string filterId =
        sender->property("filter_id").toString().toStdString();
    auto filter = mActionManager.filterMeshActionById(filterId);

    if (filter) {
        openFilterDialog(filter);
    }
}

void MeshProcessingMainWindow::applyFilter(
    const std::shared_ptr<proc::FilterMeshAction>& action,
    const proc::ParameterVector&                   params)
{
    proc::MeshVector                          inputMeshes;
    std::vector<std::shared_ptr<proc::MeshI>> inputOutputMeshes;
    proc::MeshVector                          outputMeshes;

    uint niMeshes  = action->inputMeshParameters().size();
    uint nioMeshes = action->inputOutputMeshParameters().size();

    if (niMeshes + nioMeshes == 1) {
        auto m =
            toMesh(mMeshVector->at(mUI->meshViewer->selectedDrawableObject()));
        if (niMeshes == 1) {
            inputMeshes.pushBack(m);
        }
        else {
            inputOutputMeshes.push_back(m);
        }
    }

    logger().startTimer();

    action->applyFilter(
        inputMeshes, inputOutputMeshes, outputMeshes, params, logger());

    logger().stopTimer();

    logger().log(
        TextEditLogger::MESSAGE_LOG,
        action->name() + " applied in " + std::to_string(logger().time()) +
            " seconds.");

    for (const auto& m : inputOutputMeshes) {
        toAbstractDrawableMesh(m)->updateBuffers();
    }

    for (const auto& m : outputMeshes) {
        mMeshVector->pushBack(makeMeshDrawable(m));
    }

    mUI->meshViewer->updateGUI();
    mUI->meshViewer->fitScene();
}

TextEditLogger& MeshProcessingMainWindow::logger()
{
    return mUI->meshViewer->logger();
}

void MeshProcessingMainWindow::populateFilterMenu()
{
    auto filters = mActionManager.filterMeshActions();

    std::array<QMenu*, proc::FilterMeshAction::N_CATEGORIES> menus;
    menus[proc::FilterMeshAction::CREATE] =
        new QMenu("Create", mUI->menuFilter);
    menus[proc::FilterMeshAction::CLEANING_AND_REPAIRING] =
        new QMenu("Cleaning and Repairing", mUI->menuFilter);
    menus[proc::FilterMeshAction::RECONSTRUCTION] =
        new QMenu("Reconstruction", mUI->menuFilter);
    menus[proc::FilterMeshAction::SMOOTHING] =
        new QMenu("Smoothing", mUI->menuFilter);

    for (uint i = 0; i < proc::FilterMeshAction::N_CATEGORIES; ++i) {
        mUI->menuFilter->addMenu(menus[i]);
    }

    for (const std::shared_ptr<proc::FilterMeshAction>& f : filters) {
        QAction* action = new QAction(f->name().c_str(), mUI->menuFilter);
        action->setProperty(
            "filter_id", QVariant(QString::fromStdString(f->identifier())));

        for (uint i = 0; i < proc::FilterMeshAction::N_CATEGORIES; ++i) {
            if (f->categories()[i]) {
                menus[i]->addAction(action);
            }
        }

        connect(
            action,
            SIGNAL(triggered(bool)),
            this,
            SLOT(openFilterDialog(bool)));
    }
}

void MeshProcessingMainWindow::openFilterDialog(
    const std::shared_ptr<proc::FilterMeshAction>& action)
{
    FilterMeshDockWidget* dock = new FilterMeshDockWidget(action, this);

    connect(
        dock,
        &FilterMeshDockWidget::applyFilter,
        this,
        &MeshProcessingMainWindow::applyFilter);

    dock->show();
}

std::shared_ptr<DrawableObject> MeshProcessingMainWindow::makeMeshDrawable(
    const std::shared_ptr<proc::MeshI>& mesh)
{
    switch (mesh->type()) {
    case proc::MeshIType::TRI_MESH: {
        auto m = std::make_shared<DrawableMesh<proc::TriMesh>>(
            mesh->as<proc::TriMesh>());
        setMeshInfo(*m);
        return m;
    }
    case proc::MeshIType::POLY_MESH: {
        auto m = std::make_shared<DrawableMesh<proc::PolyMesh>>(
            mesh->as<proc::PolyMesh>());
        setMeshInfo(*m);
        return m;
    }
    default: return nullptr;
    }
}

std::shared_ptr<proc::MeshI> MeshProcessingMainWindow::toMesh(
    const std::shared_ptr<DrawableObject>& drawable)
{
    return std::dynamic_pointer_cast<proc::MeshI>(drawable);
}

std::shared_ptr<AbstractDrawableMesh> MeshProcessingMainWindow::toAbstractDrawableMesh(
    const std::shared_ptr<proc::MeshI>& mesh)
{
    return std::dynamic_pointer_cast<AbstractDrawableMesh>(mesh);
}

} // namespace vcl::qt
