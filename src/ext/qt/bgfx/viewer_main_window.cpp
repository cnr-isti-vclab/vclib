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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "ext/qt/bgfx/ui_viewer_main_window.h"
#include <vclib/ext/qt/bgfx/viewer_main_window.h>

#include <vclib/render/interfaces/drawable_mesh_i.h>

namespace vcl::qbgf {

/**
 * @brief ViewerMainWindow constructor.
 *
 * Creates a MainWindow having the given parent.
 * It will manage initialize an empty vector of DrawableObjects, which owns.
 *
 * @param parent
 */
ViewerMainWindow::ViewerMainWindow(QWidget* parent) :
        QMainWindow(parent), ui(new Ui::ViewerMainWindow)
{
    ui->setupUi(this);

    // create the viewer inside the viewer container
    viewer              = new MinimalViewerWidget(ui->viewerContainer);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(viewer);
    layout->setContentsMargins(0, 0, 0, 0);
    ui->viewerContainer->setLayout(layout);

    // create the vector of DrawableObjects
    drawVector = std::make_shared<vcl::DrawableObjectVector>();

    // give the vector pointer to the contained widgets
    viewer->setDrawableObjectVector(drawVector);
    ui->drawVectorFrame->setDrawableObjectVector(drawVector);

    // each time that the RenderSettingsFrame updates its settings, we call the
    // renderSettingsUpdated() member function
    connect(
        ui->renderSettingsFrame,
        SIGNAL(settingsUpdated()),
        this,
        SLOT(renderSettingsUpdated()));

    // each time that the drawVectorFrame changes the visibility of an object,
    // we update the current settings of the RenderSettingsFrame, and we update
    // the glArea
    connect(
        ui->drawVectorFrame,
        SIGNAL(drawableObjectVisibilityChanged()),
        this,
        SLOT(visibilityDrawableObjectChanged()));

    // each time that the selected object is changed in the drawVectorFrame, we
    // update the RenderSettingsFrame, updating its settings to the object
    // render settings
    connect(
        ui->drawVectorFrame,
        SIGNAL(drawableObjectSelectionChanged(uint)),
        this,
        SLOT(selectedDrawableObjectChanged(uint)));

    ui->rightArea->setVisible(false);
}

ViewerMainWindow::~ViewerMainWindow()
{
    drawVector->clear();
    delete ui;
}

/**
 * @brief Sets the current DrawableObjectVector, and updates the GUI
 * accordingly.
 * @param v
 */
void ViewerMainWindow::setDrawableObjectVector(
    std::shared_ptr<vcl::DrawableObjectVector> v)
{
    drawVector = v;

    // order here is important: drawVectorFrame must have the drawVector before
    // the renderSettingsFrame!
    viewer->setDrawableObjectVector(drawVector);
    ui->drawVectorFrame->setDrawableObjectVector(drawVector);
    if (drawVector->size() > 0) {
        try {
            DrawableMeshI& m =
                dynamic_cast<DrawableMeshI&>(drawVector->at(0));
            ui->renderSettingsFrame->setMeshRenderSettings(m.renderSettings());
            ui->renderSettingsFrame->setVisible(true);
        }
        catch (std::bad_cast exp) {
            // the current object (the first one) is not a GenericDrawableMesh
            // we hide the RenderSettingsFrame
            ui->renderSettingsFrame->setVisible(false);
        }
        // right area is visible if there is at least one DrawableObject
        ui->rightArea->setVisible(true);
    }
    else {
        ui->rightArea->setVisible(false);
    }
    viewer->fitScene();
}

void ViewerMainWindow::keyPressEvent(QKeyEvent* event)
{
    // sometimes, the viewer does not automatically get key events
    viewer->keyPressEvent(event);
}

void ViewerMainWindow::keyReleaseEvent(QKeyEvent* event)
{
    // sometimes, the viewer does not automatically get key events
    viewer->keyReleaseEvent(event);
}

/**
 * @brief Slot called when the user changed the visibility of an object in the
 * DrawableObjectVectorFrame
 */
void ViewerMainWindow::visibilityDrawableObjectChanged()
{
    // get the selected drawable object
    uint i = ui->drawVectorFrame->selectedDrawableObject();
    try {
        // if it is a DrawableMeshI, we must be sure that its render
        // settings are updated accordingly.
        DrawableMeshI& m =
            dynamic_cast<DrawableMeshI&>(drawVector->at(i));
        ui->renderSettingsFrame->setMeshRenderSettings(m.renderSettings());
    }
    catch (std::bad_cast exp) {
    }
    viewer->update();
}

/**
 * @brief Slot called when the user selected a different DrawableObject in the
 * DrawableObjectVectorFrame
 * @param i
 */
void ViewerMainWindow::selectedDrawableObjectChanged(uint i)
{
    try {
        // take the newly selected DrawableObject and check whether it is a
        // DrawableMeshI
        DrawableMeshI& m =
            dynamic_cast<DrawableMeshI&>(drawVector->at(i));
        // if it is a GenericDrawableMesh, update the RenderSettingsFrame, and
        // set it visible
        ui->renderSettingsFrame->setMeshRenderSettings(m.renderSettings());
        ui->renderSettingsFrame->setVisible(true);
    }
    catch (std::bad_cast exp) {
        // it is not a GenericDrawableMesh, RenderSettingsFrame must be hidden
        ui->renderSettingsFrame->setVisible(false);
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
    uint i = ui->drawVectorFrame->selectedDrawableObject();
    if (drawVector->size() > 0) {
        // The selected object must always be a DrawableMeshI, because the
        // RenderSettingsFrame (which called this member function) is visible
        // only when the selected Object is a DrawableMeshI
        DrawableMeshI& m =
            dynamic_cast<DrawableMeshI&>(drawVector->at(i));
        // get RenderSettings from the RenderSettingsFrame, and set it to the
        // GenericDrawableMesh
        m.setRenderSettings(ui->renderSettingsFrame->meshRenderSettings());
        viewer->update();
    }
}

} // namespace vcl::qbgf
