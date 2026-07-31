// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/qt/gui/mesh_render_settings_frame.h>

#include "ui_mesh_render_settings_frame.h"

#include <vclib/qt/gui/mesh_render_settings_frame/edges_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/points_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/surface_frame.h>
#include <vclib/qt/gui/mesh_render_settings_frame/wireframe_frame.h>

#include <QCheckBox>
#include <QColorDialog>
#include <QLabel>
#include <QTabBar>

namespace vcl::qt {

MeshRenderSettingsFrame::MeshRenderSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::MeshRenderSettingsFrame)
{
    mUI->setupUi(this);

    mUI->tabWidget->tabBar()->setExpanding(false);

    auto setupTab = [this](
                        int                             index,
                        GenericMeshRenderSettingsFrame* frame,
                        const QString&                  title) {
        mUI->tabWidget->addTab(frame, title);
        QCheckBox* tabCb = new QCheckBox();
        tabCb->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        mUI->tabWidget->tabBar()->setTabButton(index, QTabBar::LeftSide, tabCb);

        QCheckBox* origCb = frame->visibilityCheckBox();
        origCb->hide();

        connect(origCb, &QCheckBox::toggled, tabCb, [tabCb](bool checked) {
            if (tabCb->isChecked() != checked)
                tabCb->setChecked(checked);
        });
        connect(tabCb, &QCheckBox::toggled, origCb, [origCb](bool checked) {
            if (origCb->isChecked() != checked)
                origCb->setChecked(checked);
        });

        frames.push_back(frame);
    };

    setupTab(0, new PointsFrame(mMRS, this), "Points");
    setupTab(1, new SurfaceFrame(mMRS, this), "Surface");
    setupTab(2, new WireframeFrame(mMRS, this), "Wireframe");
    setupTab(3, new EdgesFrame(mMRS, this), "Edges");

    for (auto* frame : frames) {
        connect(
            frame, SIGNAL(settingsUpdated()), this, SIGNAL(settingsUpdated()));
    }

    connect(
        mUI->applyToAllCheckBox,
        SIGNAL(toggled(bool)),
        this,
        SLOT(onApplyToAllToggled(bool)));
}

MeshRenderSettingsFrame::~MeshRenderSettingsFrame()
{
    delete mUI;
}

const MeshRenderSettings& MeshRenderSettingsFrame::meshRenderSettings() const
{
    return mMRS;
}

bool MeshRenderSettingsFrame::isApplyToAllEnabled() const
{
    return mUI->applyToAllCheckBox->isChecked();
}

void MeshRenderSettingsFrame::onApplyToAllToggled(bool checked)
{
    if (checked) {
        mMRS.setAllCapabilities(true);
        updateGuiFromSettings(false);
    }
    emit applyToAllToggled(checked);
}

void MeshRenderSettingsFrame::setMeshRenderSettings(
    const MeshRenderSettings& settings,
    bool                      changeCurrentTab /*= false*/)
{
    mMRS = settings;
    if (isApplyToAllEnabled()) {
        mMRS.setAllCapabilities(true);
    }
    updateGuiFromSettings(changeCurrentTab);
}

void MeshRenderSettingsFrame::updateGuiFromSettings(bool changeCurrentTab)
{
    using MRI = MeshRenderInfo;

    for (auto* frame : frames) {
        frame->updateFrameFromSettings();
    }

    if (mMRS.canBeVisible()) {
        mUI->tabWidget->setEnabled(true);
    }
    else {
        mUI->tabWidget->setEnabled(false);
    }

    mUI->tabWidget->setTabVisible(
        POINTS_FRAME, mMRS.canPoints(MRI::Points::VISIBLE));
    mUI->tabWidget->setTabVisible(
        SURFACE_FRAME, mMRS.canSurface(MRI::Surface::VISIBLE));
    mUI->tabWidget->setTabVisible(
        WIREFRAME_FRAME, mMRS.canWireframe(MRI::Wireframe::VISIBLE));
    mUI->tabWidget->setTabVisible(
        EDGES_FRAME, mMRS.canEdges(MRI::Edges::VISIBLE));

    if (changeCurrentTab) {
        if (mMRS.canSurface(MRI::Surface::VISIBLE)) { // high priority: surface
            mUI->tabWidget->setCurrentIndex(SURFACE_FRAME);
        }
        else if (mMRS.canEdges(MRI::Edges::VISIBLE)) { // second priority: edges
            mUI->tabWidget->setCurrentIndex(EDGES_FRAME);
        }
        else if (mMRS.canPoints(MRI::Points::VISIBLE)) { // lowest: points
            mUI->tabWidget->setCurrentIndex(POINTS_FRAME);
        }
    }
}

} // namespace vcl::qt
