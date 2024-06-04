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

#include "ui_mesh_render_settings_frame.h"
#include <vclib/ext/qt/gui/mesh_render_settings_frame.h>

#include <QColorDialog>
#include <QStandardItemModel>

#include <vclib/ext/qt/gui/mesh_render_settings_frame/edges_frame.h>
#include <vclib/ext/qt/gui/mesh_render_settings_frame/points_frame.h>
#include <vclib/ext/qt/gui/mesh_render_settings_frame/surface_frame.h>
#include <vclib/ext/qt/gui/mesh_render_settings_frame/wireframe_frame.h>

namespace vcl::qt {

MeshRenderSettingsFrame::MeshRenderSettingsFrame(QWidget* parent) :
        QFrame(parent), mUI(new Ui::MeshRenderSettingsFrame)
{
    mUI->setupUi(this);

    auto* pointsFrame = new PointsFrame(mMRS, this);
    mUI->tabWidget->addTab(pointsFrame, "Points");
    frames.push_back(pointsFrame);

    auto* surfaceFrame = new SurfaceFrame(mMRS, this);
    mUI->tabWidget->addTab(surfaceFrame, "Surface");
    frames.push_back(surfaceFrame);

    auto* wireframeFrame = new WireframeFrame(mMRS, this);
    mUI->tabWidget->addTab(wireframeFrame, "Wireframe");
    frames.push_back(wireframeFrame);

    auto* edgesFrame = new EdgesFrame(mMRS, this);
    mUI->tabWidget->addTab(edgesFrame, "Edges");
    frames.push_back(edgesFrame);

    for (auto* frame : frames) {
        connect(
            frame, SIGNAL(settingsUpdated()), this, SIGNAL(settingsUpdated()));
    }
}

MeshRenderSettingsFrame::~MeshRenderSettingsFrame()
{
    delete mUI;
}

const MeshRenderSettings& MeshRenderSettingsFrame::meshRenderSettings() const
{
    return mMRS;
}

void MeshRenderSettingsFrame::setMeshRenderSettings(
    const MeshRenderSettings& settings,
    bool changeCurrentTab /*= false*/)
{
    mMRS = settings;
    updateGuiFromSettings(changeCurrentTab);
}

void MeshRenderSettingsFrame::updateGuiFromSettings(bool changeCurrentTab)
{
    for (auto* frame : frames) {
        frame->updateFrameFromSettings();
    }

    if (mMRS.canBeVisible()) {
        mUI->tabWidget->setEnabled(true);
    }
    else {
        mUI->tabWidget->setEnabled(false);
    }

    mUI->tabWidget->setTabVisible(POINTS_FRAME, mMRS.canPointCloudBeVisible());
    mUI->tabWidget->setTabVisible(SURFACE_FRAME, mMRS.canSurfaceBeVisible());
    mUI->tabWidget->setTabVisible(WIREFRAME_FRAME, mMRS.canSurfaceBeVisible());
    mUI->tabWidget->setTabVisible(EDGES_FRAME, mMRS.canEdgesBeVisible());

    if (changeCurrentTab) {
        if (mMRS.canSurfaceBeVisible()) { // high priority: surface
            mUI->tabWidget->setCurrentIndex(SURFACE_FRAME);
        }
        else if (mMRS.canEdgesBeVisible()) { // second priority: edges
            mUI->tabWidget->setCurrentIndex(EDGES_FRAME);
        }
        else if (mMRS.canPointCloudBeVisible()) { // third priority: points
            mUI->tabWidget->setCurrentIndex(POINTS_FRAME);
        }

    }
}

} // namespace vcl::qt
