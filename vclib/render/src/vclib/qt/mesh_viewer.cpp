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

#include <vclib/qt/gui/screen_shot_dialog.h>
#include <vclib/qt/mesh_viewer.h>

#include "ui_mesh_viewer.h"

#include <vclib/render/drawable/drawable_mesh.h>

namespace vcl::qt {


bool KeyFilter::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        // Ignore only the Ctrl + S shortcut override, you can customize check for your needs
        if (keyEvent->modifiers().testFlag(Qt::ControlModifier) && keyEvent->key() == 'S') {
            qDebug() << "Ignoring" << keyEvent->modifiers() << "+" << (char)keyEvent->key() << "for" << watched;
            event->ignore();
            return true;
        }
    }

    return QObject::eventFilter(watched, event);
}

/**
 * @brief MeshViewer constructor.
 *
 * Creates a MeshViewer having the given parent.
 * // Todo: document
 *
 * @param parent
 */
MeshViewer::MeshViewer(QWidget* parent) :
        QWidget(parent), mUI(new Ui::MeshViewer)
{
    mUI->setupUi(this);

    mDrawableObjectVector = std::make_shared<DrawableObjectVector>();

    // create the vector of DrawableObjects
    mDrawableObjectVector->resize(2, DrawableObjectVector());

    mListedDrawableObjects = std::dynamic_pointer_cast<DrawableObjectVector>(
        mDrawableObjectVector->at(0));

    mUnlistedDrawableObjects = std::dynamic_pointer_cast<DrawableObjectVector>(
        mDrawableObjectVector->at(1));

    // give the vector pointer to the contained widgets
    mUI->viewer->setDrawableObjectVector(mDrawableObjectVector);
    mUI->drawVectorFrame->setDrawableObjectVector(mListedDrawableObjects);

    // install the key filter
    mUI->viewer->installEventFilter(new KeyFilter(this));

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
}

MeshViewer::~MeshViewer()
{
    delete mUI;
}

/**
 * @brief Sets the current DrawableObjectVector, and updates the GUI
 * accordingly.
 * @param v
 */
void MeshViewer::setDrawableObjectVector(
    const std::shared_ptr<DrawableObjectVector>& v)
{
    mDrawableObjectVector->set(0, v);
    mListedDrawableObjects = v;

    // order here is important: drawVectorFrame must have the drawVector before
    // the renderSettingsFrame!
    mUI->viewer->setDrawableObjectVector(mDrawableObjectVector);
    mUI->drawVectorFrame->setDrawableObjectVector(mListedDrawableObjects);

    updateGUI();
}

void MeshViewer::setUnlistedDrawableObjectVector(
    const std::shared_ptr<DrawableObjectVector>& v)
{
}

uint MeshViewer::selectedDrawableObject() const
{
    return mUI->drawVectorFrame->selectedDrawableObject();
}

TextEditLogger& MeshViewer::logger()
{
    return *mUI->logger;
}

void MeshViewer::setDrawVectorIconFunction(
    const DrawableObjectVectorFrame::IconFunction& f)
{
    mUI->drawVectorFrame->setIconFunction(f);
}

void MeshViewer::keyPressEvent(QKeyEvent* event)
{
    // show screenshot dialog on CTRL + S
    if (event->key() == Qt::Key_S && event->modifiers() & Qt::ControlModifier) {
        vcl::qt::ScreenShotDialog dialog(this);
        if (dialog.exec() && dialog.selectedFiles().size() > 0) {
            mUI->viewer->screenshot(
                dialog.selectedFiles()[0].toStdString(),
                dialog.screenMultiplierValue());
        }
    }
    else {
        event->ignore();
        QWidget::keyPressEvent(event);
    }
}

/**
 * @brief Slot called when the user changed the visibility of an object in the
 * DrawableObjectVectorFrame
 */
void MeshViewer::visibilityDrawableObjectChanged()
{
    // get the selected drawable object
    uint i = mUI->drawVectorFrame->selectedDrawableObject();
    auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
        mListedDrawableObjects->at(i));
    // if it is a AbstractDrawableMesh, we must be sure that its render
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
void MeshViewer::selectedDrawableObjectChanged(uint i)
{
    // take the newly selected DrawableObject and check whether it is a
    // AbstractDrawableMesh
    auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
        mListedDrawableObjects->at(i));
    if (m) {
        // if it is a AbstractDrawableMesh, update the RenderSettingsFrame, and
        // set it enabled
        mUI->renderSettingsFrame->setMeshRenderSettings(m->renderSettings());
        mUI->renderSettingsFrame->setEnabled(true);
    }
    else {
        // it is not a AbstractDrawableMesh, RenderSettingsFrame must be
        // disabled
        mUI->renderSettingsFrame->setEnabled(false);
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
void MeshViewer::renderSettingsUpdated()
{
    // The user changed the RenderSettings of the ith object.
    uint i = mUI->drawVectorFrame->selectedDrawableObject();
    if (mListedDrawableObjects->size() > 0) {
        // The selected object must always be a AbstractDrawableMesh, because
        // the RenderSettingsFrame (which called this member function) is
        // visible only when the selected Object is a AbstractDrawableMesh
        auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
            mListedDrawableObjects->at(i));
        // get RenderSettings from the RenderSettingsFrame, and set it to the
        // AbstractDrawableMesh
        m->setRenderSettings(mUI->renderSettingsFrame->meshRenderSettings());
        mUI->viewer->update();
    }
}

void MeshViewer::fitScene()
{
    mUI->viewer->fitScene();
    mUI->viewer->update();
}

void MeshViewer::updateGUI()
{
    mUI->drawVectorFrame->update();

    uint selected = mUI->drawVectorFrame->selectedDrawableObject();

    if (selected != UINT_NULL) {
        auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
            mListedDrawableObjects->at(selected));
        if (m) {
            mUI->renderSettingsFrame->setMeshRenderSettings(
                m->renderSettings(), true);
            mUI->renderSettingsFrame->setEnabled(true);
        }
        else {
            mUI->renderSettingsFrame->setEnabled(false);
        }
    }
    else {
        mUI->renderSettingsFrame->setEnabled(false);
    }
    mUI->viewer->update();
}

} // namespace vcl::qt
