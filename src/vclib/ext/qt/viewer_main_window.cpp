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

#include "ext/qt/ui_viewer_main_window.h"
#include <vclib/ext/qt/viewer_main_window.h>

#include <vclib/ext/qt/utils/file_format.h>
#include <vclib/render/drawable/drawable_mesh.h>

#include <QFileDialog>

#include <vclib/load_save.h>
#include <vclib/meshes.h>
#include <vclib/processing/actions.h>

namespace vcl::qt {

/**
 * @brief ViewerMainWindow constructor.
 *
 * Creates a MainWindow having the given parent.
 * It will manage initialize an empty vector of DrawableObjects, which owns.
 *
 * @param parent
 */
ViewerMainWindow::ViewerMainWindow(QWidget* parent) :
        QMainWindow(parent), mUI(new Ui::ViewerMainWindow)
{
    mUI->setupUi(this);

    // create the vector of DrawableObjects
    mDrawVector = std::make_shared<vcl::DrawableObjectVector>();

    // give the vector pointer to the contained widgets
    mUI->viewer->setDrawableObjectVector(mDrawVector);
    mUI->drawVectorFrame->setDrawableObjectVector(mDrawVector);

    // populate action manager
    mActionManager.add(proc::vclibActions());

    // each time that the RenderSettingsFrame updates its settings, we call the
    // renderSettingsUpdated() member function
    connect(
        mUI->renderSettingsFrame,
        SIGNAL(settingsUpdated()),
        this,
        SLOT(renderSettingsUpdated()));

    // each time that the drawVectorFrame changes the visibility of an object,
    // we update the current settings of the RenderSettingsFrame, and we update
    // the glArea
    connect(
        mUI->drawVectorFrame,
        SIGNAL(drawableObjectVisibilityChanged()),
        this,
        SLOT(visibilityDrawableObjectChanged()));

    // each time that the selected object is changed in the drawVectorFrame, we
    // update the RenderSettingsFrame, updating its settings to the object
    // render settings
    connect(
        mUI->drawVectorFrame,
        SIGNAL(drawableObjectSelectionChanged(uint)),
        this,
        SLOT(selectedDrawableObjectChanged(uint)));

    mUI->viewer->setFocus();
    mUI->rightArea->setVisible(false);
}

ViewerMainWindow::~ViewerMainWindow()
{
    mDrawVector->clear();
    delete mUI;
}

/**
 * @brief Sets the current DrawableObjectVector, and updates the GUI
 * accordingly.
 * @param v
 */
void ViewerMainWindow::setDrawableObjectVector(
    const std::shared_ptr<DrawableObjectVector>& v)
{
    mDrawVector = v;

    // order here is important: drawVectorFrame must have the drawVector before
    // the renderSettingsFrame!
    mUI->viewer->setDrawableObjectVector(mDrawVector);
    mUI->drawVectorFrame->setDrawableObjectVector(mDrawVector);
    if (mDrawVector->size() > 0) {
        auto m = std::dynamic_pointer_cast<DrawableMeshI>(mDrawVector->at(0));
        if (m) {
            mUI->renderSettingsFrame->setMeshRenderSettings(
                m->renderSettings(), true);
            mUI->renderSettingsFrame->setVisible(true);
        }
        else {
            mUI->renderSettingsFrame->setVisible(false);
        }
        // right area is visible if there is at least one DrawableObject
        mUI->rightArea->setVisible(true);
    }
    else {
        mUI->rightArea->setVisible(false);
    }
}

TextEditLogger& ViewerMainWindow::logger()
{
    return *mUI->logger;
}

/**
 * @brief Slot called when the user changed the visibility of an object in the
 * DrawableObjectVectorFrame
 */
void ViewerMainWindow::visibilityDrawableObjectChanged()
{
    // get the selected drawable object
    uint i = mUI->drawVectorFrame->selectedDrawableObject();
    auto m = std::dynamic_pointer_cast<DrawableMeshI>(mDrawVector->at(i));
    // if it is a DrawableMeshI, we must be sure that its render
    // settings are updated accordingly.
    if (m) {
        mUI->renderSettingsFrame->setMeshRenderSettings(m->renderSettings());
    }
    mUI->viewer->update();
}

/**
 * @brief Slot called when the user selected a different DrawableObject in the
 * DrawableObjectVectorFrame
 * @param i
 */
void ViewerMainWindow::selectedDrawableObjectChanged(uint i)
{
    // take the newly selected DrawableObject and check whether it is a
    // DrawableMeshI
    auto m = std::dynamic_pointer_cast<DrawableMeshI>(mDrawVector->at(i));
    if (m) {
        // if it is a DrawableMeshI, update the RenderSettingsFrame, and
        // set it visible
        mUI->renderSettingsFrame->setMeshRenderSettings(m->renderSettings());
        mUI->renderSettingsFrame->setVisible(true);
    }
    else {
        // it is not a DrawableMeshI, RenderSettingsFrame must be hidden
        mUI->renderSettingsFrame->setVisible(false);
    }
}

/**
 * @brief Slot called every time that the MeshRenderSettingsFrame emits
 * 'settingsUpdated()', that is when the user changes render settings of a
 * GeneriDrawableMesh.
 *
 * We need to get the selected GeneriDrawableMesh first, and then update the
 * settings to it.
 */
void ViewerMainWindow::renderSettingsUpdated()
{
    // The user changed the RenderSettings of the ith object.
    uint i = mUI->drawVectorFrame->selectedDrawableObject();
    if (mDrawVector->size() > 0) {
        // The selected object must always be a DrawableMeshI, because the
        // RenderSettingsFrame (which called this member function) is visible
        // only when the selected Object is a DrawableMeshI
        auto m = std::dynamic_pointer_cast<DrawableMeshI>(mDrawVector->at(i));
        // get RenderSettings from the RenderSettingsFrame, and set it to the
        // DrawableMeshI
        m->setRenderSettings(mUI->renderSettingsFrame->meshRenderSettings());
        mUI->viewer->update();
    }
}

void ViewerMainWindow::on_actionSave_triggered()
{
    std::vector<proc::FileFormat> formats = mActionManager.saveMeshFormats();

    QString filter = filterFormatsToQString(formats);

    QFileDialog* dialog = new QFileDialog(this, "Save Mesh", "", filter);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    if (dialog->exec() == QDialog::Accepted) {
        auto fs   = dialog->selectedFiles();
        auto frmt = dialog->selectedNameFilter();

        std::cerr << frmt.toStdString() << "\n";

        // get format from frmt, which is '(*.stl )'
        std::string format = frmt.toStdString();
        format             = format.substr(2, 4);

        std::string filename = fs.first().toStdString();
        std::string fnext    = FileInfo::extension(filename);
        if (fnext.empty() || fnext != format) {
            filename += "." + format;
        }
        uint i = mUI->drawVectorFrame->selectedDrawableObject();
        std::shared_ptr<DrawableObjectI> d = mDrawVector->at(i);

        std::shared_ptr<vcl::TriMesh> m =
            std::dynamic_pointer_cast<vcl::TriMesh>(d);

        if (m) {
            // todo: use directly m when it will be a proc::TriMesh
            proc::TriMesh t;
            t.enableSameOptionalComponentsOf(*m);
            t.importFrom(*m);
            mActionManager.saveMeshAction(format)->save(filename, t);
        }
    }
}

void ViewerMainWindow::on_actionShow_Right_Bar_triggered(bool checked)
{
    mUI->rightArea->setVisible(checked);

    mUI->actionShow_Mesh_List->setEnabled(checked);
    mUI->actionShow_Mesh_Render_Settings->setEnabled(checked);
    mUI->actionShow_Logger->setEnabled(checked);
}

void ViewerMainWindow::on_actionShow_Logger_triggered(bool checked)
{
    mUI->logger->setVisible(checked);
}

void ViewerMainWindow::on_actionShow_Mesh_Render_Settings_triggered(
    bool checked)
{
    mUI->renderSettingsFrame->setVisible(checked);
}

void ViewerMainWindow::on_actionShow_Mesh_List_triggered(bool checked)
{
    mUI->drawVectorFrame->setVisible(checked);
}

} // namespace vcl::qt
