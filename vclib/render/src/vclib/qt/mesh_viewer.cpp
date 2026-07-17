// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/mesh_viewer.h>

#include "ui_mesh_viewer.h"

#include <vclib/qt/gui/screen_shot_dialog.h>
#include <vclib/qt/gui/toolbar_frames.h>
#include <vclib/render/concepts/pbr_viewer.h>
#include <vclib/render/drawable/drawable_mesh.h>

namespace vcl::qt {

bool KeyFilter::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        // Ignore only the Ctrl + S shortcut override, you can customize check
        // for your needs
        if (keyEvent->modifiers().testFlag(Qt::ControlModifier) &&
            keyEvent->key() == 'S') {
            qDebug() << "Ignoring " << keyEvent->modifiers() << " + "
                     << (char) keyEvent->key() << " for " << watched;
            event->ignore();
            return true;
        }
    }

    return QObject::eventFilter(watched, event);
}

/**
 * @brief Returns the MeshViewerRenderApp used by this MeshViewer.
 *
 * @return MeshViewerRenderApp
 */
MeshViewerRenderApp& MeshViewer::viewer() const
{
    return *static_cast<vcl::qt::MeshViewerRenderApp*>(mUI->viewer);
}

DrawableObjectVectorTree& MeshViewer::drawableObjectVectorTree() const
{
    return *mUI->drawVectorTree;
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
        QMainWindow(parent), mUI(new Ui::MeshViewer)
{
    mUI->setupUi(this);

    // give keyboard focus to the viewer widget immediately
    mUI->viewer->setFocus();

    // prevent any widget in the right area from stealing keyboard focus
    mUI->rightArea->setFocusPolicy(Qt::NoFocus);
    std::function<void(QWidget*)> disableFocus = [&disableFocus](QWidget* w) {
        w->setFocusPolicy(Qt::NoFocus);
        for (auto* child : w->findChildren<QWidget*>(
                 QString(), Qt::FindChildrenRecursively)) {
            disableFocus(child);
        }
    };
    disableFocus(mUI->rightArea);

    /** Drawable Object Vector **/

    mDrawableObjectVector = std::make_shared<DrawableObjectVector>();

    // give the vector pointer to the contained widgets
    mUI->viewer->setDrawableObjectVector(mDrawableObjectVector);
    mUI->drawVectorTree->setDrawableObjectVector(mDrawableObjectVector);

    /** Editors **/

    // no toolbar editors
    mMeshSelectorEditor = viewer().pushEditor<vcl::MeshSelectorEditor>();
    mMeshSelectorEditor->setActive(true);
    auto callback = [this](uint id) {
        drawableObjectVectorTree().setSelectedItem(id);
    };
    mMeshSelectorEditor->setOnObjectSelectedFunction(callback);

    // toolbar editors and frames
    mAxisEditor = std::dynamic_pointer_cast<vcl::AxisEditor<ViewerType>>(
        viewer().getEditor(ViewerType::BuiltInEditors::AXIS));
    assert(mAxisEditor);
    AxisEditorFrame<ViewerType>* axisEditorFrame =
        new AxisEditorFrame<ViewerType>(mAxisEditor);
    mUI->toolBar->addWidget(axisEditorFrame);

    auto* trackballFrame = new TrackBallFrame(viewer());
    mUI->toolBar->addWidget(trackballFrame);

    mBoundingBoxEditor = viewer().pushEditor<vcl::BoundingBoxEditor>();
    BoundingBoxEditorFrame<ViewerType>* bboxEditorFrame =
        new BoundingBoxEditorFrame<ViewerType>(mBoundingBoxEditor);
    mUI->toolBar->addWidget(bboxEditorFrame);

    mSelectionEditor = viewer().pushEditor<vcl::SelectionEditor>();
    SelectionEditorFrame<ViewerType>* selectionEditor =
        new SelectionEditorFrame<ViewerType>(mSelectionEditor);
    mUI->toolBar->addWidget(selectionEditor);

    disableFocus(mUI->toolBar);

    /** Render Settings Frame **/

    mUI->viewerRenderSettingsFrame->setViewer(mUI->viewer);

    /** Events **/

    // install the key filter
    mUI->viewer->installEventFilter(new KeyFilter(this));

    // each time that the RenderSettingsFrame updates its settings, we call the
    // meshRenderSettingsUpdated() member function
    connect(
        mUI->meshRenderSettingsFrame,
        SIGNAL(settingsUpdated()),
        this,
        SLOT(meshRenderSettingsUpdated()));

    connect(
        mUI->meshRenderSettingsFrame,
        SIGNAL(applyToAllToggled(bool)),
        this,
        SLOT(applyToAllToggled(bool)));

    // each time that the drawVectorTree changes the visibility of an object,
    // we update the current settings of the RenderSettingsFrame, and we update
    // the glArea
    connect(
        mUI->drawVectorTree,
        SIGNAL(drawableObjectVisibilityChanged()),
        this,
        SLOT(visibilityDrawableObjectChanged()));

    // each time that the selected object is changed in the drawVectorTree, we
    // update the RenderSettingsFrame, updating its settings to the object
    // render settings
    connect(
        mUI->drawVectorTree,
        SIGNAL(drawableObjectSelectionChanged(uint)),
        this,
        SLOT(selectedDrawableObjectChanged(uint)));
}

MeshViewer::~MeshViewer()
{
    delete mUI;
}

uint MeshViewer::selectedDrawableObject() const
{
    return mUI->drawVectorTree->selectedDrawableObject();
}

std::shared_ptr<vcl::DrawableObject> MeshViewer::drawableObject(uint i)
{
    return viewer().drawableObjectVector().at(i);
}

std::shared_ptr<const vcl::DrawableObject> MeshViewer::drawableObject(
    uint i) const
{
    return viewer().drawableObjectVector().at(i);
}

uint MeshViewer::drawableObjectsCount() const
{
    return viewer().drawableObjectVector().size();
}

const vcl::DrawableObjectVector& MeshViewer::drawableObjects() const
{
    return viewer().drawableObjectVector();
}

bool MeshViewer::removeDrawableObject(uint id)
{
    bool res = viewer().removeDrawableObject(id);
    if (res)
        updateGUI();
    return res;
}

void MeshViewer::clearDrawableObjects()
{
    viewer().clearDrawableObjects();
    updateGUI();
}

void MeshViewer::refreshEditors()
{
    viewer().refreshEditors();
}

TextEditLogger& MeshViewer::logger()
{
    return *mUI->logger;
}

void MeshViewer::setDrawVectorIconFunction(
    const DrawableObjectVectorTree::IconFunction& f)
{
    mUI->drawVectorTree->setIconFunction(f);
}

Camera<float> MeshViewer::camera() const
{
    return mUI->viewer->camera();
}

void MeshViewer::setCamera(const Camera<float>& c)
{
    mUI->viewer->setCamera(c);
}

// void MeshViewer::showRenderModeSelector(bool show)
// {
// }

void MeshViewer::setPbrSettings(const PBRViewerSettings& settings)
{
    mUI->viewerRenderSettingsFrame->setPbrSettings(settings);
}

const PBRViewerSettings& MeshViewer::pbrSettings() const
{
    return mUI->viewerRenderSettingsFrame->pbrSettings();
}

void MeshViewer::setPanorama(const std::string& panorama)
{
    mUI->viewerRenderSettingsFrame->setPanorama(panorama);
}

void MeshViewer::keyPressEvent(QKeyEvent* event)
{
    // show screenshot dialog on CTRL + S
    if (event->key() == Qt::Key_S && event->modifiers() & Qt::ControlModifier) {
        vcl::qt::ScreenShotDialog dialog(this);
        if (dialog.exec() && dialog.selectedFiles().size() > 0) {
            auto sf = dialog.selectedFiles();
            mUI->viewer->screenshot(
                sf[0].toStdString(), dialog.screenMultiplierValue());
        }
    }
    else {
        event->ignore();
        QWidget::keyPressEvent(event);
    }
}

void MeshViewer::fitScene()
{
    mUI->viewer->fitScene();
    mUI->viewer->update();
}

void MeshViewer::fitView()
{
    mUI->viewer->fitView();
    mUI->viewer->update();
}

void MeshViewer::updateGUI()
{
    for (auto& obj : *mDrawableObjectVector) {
        auto mesh = std::dynamic_pointer_cast<AbstractDrawableMesh>(obj);
        if (mesh) {
            mesh->setOnSelectionUpdatedCallback([this]() {
                mUI->drawVectorTree->updateSelectionCounters();
            });
        }
    }

    mUI->drawVectorTree->update();

    uint selected = mUI->drawVectorTree->selectedDrawableObject();

    if (selected != UINT_NULL) {
        auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
            mDrawableObjectVector->at(selected));
        if (m) {
            if (!mUI->meshRenderSettingsFrame->isApplyToAllEnabled()) {
                mUI->meshRenderSettingsFrame->setMeshRenderSettings(
                    m->renderSettings(), true);
            }
            mUI->meshRenderSettingsFrame->setEnabled(true);
        }
        else {
            mUI->meshRenderSettingsFrame->setEnabled(false);
        }
    }
    else {
        mUI->meshRenderSettingsFrame->setEnabled(false);
    }
    mUI->viewer->update();
}

/**
 * @brief Slot called when the user changed the visibility of an object in the
 * DrawableObjectVectorTree
 */
void MeshViewer::visibilityDrawableObjectChanged()
{
    // get the selected drawable object
    uint i = mUI->drawVectorTree->selectedDrawableObject();
    if (i != UINT_NULL) {
        auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
            mDrawableObjectVector->at(i));
        // if it is a AbstractDrawableMesh, we must be sure that its render
        // settings are updated accordingly.
        if (m) {
            if (!mUI->meshRenderSettingsFrame->isApplyToAllEnabled()) {
                mUI->meshRenderSettingsFrame->setMeshRenderSettings(
                    m->renderSettings());
            }
        }
        mUI->viewer->update();
    }
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
    mDrawableObjectVector->setSelectedObjectId(i);
    auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
        mDrawableObjectVector->at(i));
    if (m) {
        // if it is a AbstractDrawableMesh, update the RenderSettingsFrame, and
        // set it enabled
        if (!mUI->meshRenderSettingsFrame->isApplyToAllEnabled()) {
            mUI->meshRenderSettingsFrame->setMeshRenderSettings(
                m->renderSettings());
        }
        mUI->meshRenderSettingsFrame->setEnabled(true);
    }
    else {
        // it is not a AbstractDrawableMesh, RenderSettingsFrame must be
        // disabled
        mUI->meshRenderSettingsFrame->setEnabled(false);
    }
    mUI->viewer->update();
}

/**
 * @brief Slot called every time that the MeshRenderSettingsFrame emits
 * 'settingsUpdated()', that is when the user changes render settings of a
 * AbstractDrawableMesh.
 *
 * We need to get the selected AbstractDrawableMesh first, and then update the
 * settings to it.
 */
void MeshViewer::meshRenderSettingsUpdated()
{
    // The user changed the RenderSettings of the ith object.
    uint i = mUI->drawVectorTree->selectedDrawableObject();
    if (i != UINT_NULL && mDrawableObjectVector->size() > 0) {
        // The selected object must always be a AbstractDrawableMesh, because
        // the RenderSettingsFrame (which called this member function) is
        // visible only when the selected Object is a AbstractDrawableMesh
        auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
            mDrawableObjectVector->at(i));

        if (m) {
            bool applyToAll =
                mUI->meshRenderSettingsFrame->isApplyToAllEnabled();
            const auto& newSettings =
                mUI->meshRenderSettingsFrame->meshRenderSettings();

            if (applyToAll) {
                for (uint j = 0; j < mDrawableObjectVector->size(); ++j) {
                    auto mesh = std::dynamic_pointer_cast<AbstractDrawableMesh>(
                        mDrawableObjectVector->at(j));
                    if (mesh) {
                        MeshRenderSettings rs = mesh->renderSettings();
                        rs.updateIfCapable(newSettings);
                        mesh->setRenderSettings(rs);
                    }
                }
            }
            else {
                m->setRenderSettings(newSettings);
            }
            mUI->viewer->update();
        }
    }
}

void MeshViewer::applyToAllToggled(bool checked)
{
    if (!checked) {
        uint i = mUI->drawVectorTree->selectedDrawableObject();
        if (i != UINT_NULL && mDrawableObjectVector->size() > 0) {
            auto m = std::dynamic_pointer_cast<AbstractDrawableMesh>(
                mDrawableObjectVector->at(i));
            if (m) {
                mUI->meshRenderSettingsFrame->setMeshRenderSettings(
                    m->renderSettings());
            }
        }
    }
    else {
        meshRenderSettingsUpdated();
    }
}

} // namespace vcl::qt
